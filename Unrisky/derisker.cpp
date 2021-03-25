#pragma once

#include "derisker.h"
#include "json11.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

void Derisker::buildMarketListJson(const std::stringstream& buffer, std::vector<Market>* const markets_out) {
  if (markets_out == nullptr) {
    return;
  }

  std::vector<Market> markets;
  using Json = json11::Json;
  Json json;
  std::string error;
  json = Json::parse(buffer.str(), error);

  const Json::array markets_json = json["markets"].array_items();

  for (const auto& market_json : markets_json) {
    Market market;
    market.id = market_json["id"].int_value();
    market.name = market_json["name"].string_value();

    float buy_no_cost_sum = 0.0f;

    const Json::array contracts_json = market_json["contracts"].array_items();
    for (const auto& contract_json : contracts_json) {
      // TODO: be more careful here to ensure ID values are present and unique
      Contract contract;
      contract.id = contract_json["id"].int_value();
      Json best_buy_no_cost_json = contract_json["bestBuyNoCost"];
      switch (best_buy_no_cost_json.type()) {
        case Json::NUL:
          // This branch is hit when there are no shares available to buy.
          contract.best_buy_no_cost = 1.00f;
          break;
        case Json::NUMBER:
          contract.best_buy_no_cost = static_cast<float>(best_buy_no_cost_json.number_value());
          break;
        default:
          contract.best_buy_no_cost = 1.00f;
          std::cout << "Unknown bestBuyNoCost type." << std::endl;
          break;
      }
      buy_no_cost_sum += contract.best_buy_no_cost;
      market.contracts.push_back(contract);
    }

    market.advantage = market.contracts.size() - 1 - buy_no_cost_sum;
    MarketOwnership ownership;  // NOTE: We don't use this at all right now.
    getIdealMarketOwnership(market, 850.00f, 0.10f, &ownership, &market.risk);
    markets.emplace_back(market);
  }

  *markets_out = markets;
}

void Derisker::displaySortedMarketList(const std::vector<Market>& markets, const float risk_threshold) {
  // Reduce the markets to those that qualify. (Use a new vector to build this.)
  std::vector<Market> qualifying_markets;
  for (const auto& m : markets) {
    if (m.risk > risk_threshold) {
      qualifying_markets.push_back(m);
    }
  }

  // Sort the remaining markets by their amount of risk in descending order.
  std::sort(qualifying_markets.begin(), qualifying_markets.end(),
    [](const Market& a, const Market& b) {return a.risk > b.risk; });

  printf("Displaying markets with risk >= $%.2f...", risk_threshold);
  std::cout << std::endl;
  for (const Market& qm : qualifying_markets) {
    if (qm.risk < risk_threshold) {
      break;
    }
    char buf[1000];
    snprintf(buf, 1000, "$%6.2f [%+5.2f] @ #%d: %s", qm.risk, qm.advantage, qm.id, qm.name.c_str());
    std::cout << buf << std::endl;
  }

  if (qualifying_markets.empty()) {
    std::cout << "(No markets meet the criteria.)" << std::endl;
  }
}

void Derisker::processJson(const char* const file_name, const float min_risk) {
  if (file_name == nullptr) {
    return;
  }

  std::ifstream t(file_name);
  std::stringstream buffer;
  buffer << t.rdbuf();

  std::vector<Market> markets;
  buildMarketListJson(buffer, &markets);
  if (markets.empty()) {
    std::cout << "No markets can be found in the JSON file. (Is PredictIt down?)" << std::endl;
  } else {
    displaySortedMarketList(markets, min_risk);
  }
  std::cout << std::endl;
}

void Derisker::getIdealMarketOwnership(const Market& market, const float max_stake, const float fee_ratio,
    MarketOwnership* const market_ownership_out, float* risk_out) {
  if (market_ownership_out == nullptr || risk_out == nullptr) {
    return;
  }

  if (market.contracts.empty()) {
    return;
  }

  MarketOwnership& ownership = *market_ownership_out;
  ownership = MarketOwnership();

  // Populate ownership's stakes with the contracts in the sorted market, ordered from most to
  // least expensive.
  Market market_temp = market;
  market_temp.contracts.sort([](Contract& a, Contract& b) {
    return a.best_buy_no_cost > b.best_buy_no_cost;
    });
  for (const Contract& contract : market_temp.contracts) {
    ownership.stakes.push_back({contract, 0});
  }

  // Now comes the fun part... We want to buy expensive shares until we max out; then for each
  // subsequent option, buy shares until just before we would decrease our risk.
  auto it = ownership.stakes.begin();
  it->num_shares =
    static_cast<int>(floor(max_stake / ownership.stakes.front().contract.best_buy_no_cost));
  int last_shares = it->num_shares;
  float risk = ownership.getRisk(fee_ratio);
  float last_risk = risk;
  ++it;
  while (it != ownership.stakes.end()) {
    // Calculate starting values...
    it->num_shares = last_shares;
    risk = ownership.getRisk(fee_ratio);
    last_risk = risk;
    bool max_found = false;
    // Now iterate.
    while (!max_found) {
      ++(it->num_shares);
      risk = ownership.getRisk(fee_ratio);
      if (risk < last_risk) {
        // Back up by one, since this was the first instance in which we lost money.
        --(it->num_shares);
        last_shares = it->num_shares;
        max_found = true;
      } else {
        last_risk = risk;
      }
    }
    ++it;  // Increment iterator.
  }

  *risk_out = last_risk;

  return;
}
