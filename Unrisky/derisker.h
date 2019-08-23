#pragma once

#include "json11.hpp"
#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include <iomanip>
#include <vector>
#include <sstream>
#include <algorithm>

// Information about a proposition that may resolve "yes" or "no."
struct Contract {
  int id = -1;
  float best_buy_no_cost = 0.00f;
};

// Information about a market, or a collection of linked contracts.
struct Market {
  int id = -1;
  std::string name;
  float advantage = 0.0f;  // TODO: Retire me?
  std::list<Contract> contracts;
};

class Derisker {
public:
  static void buildMarketListJson(const std::stringstream& buffer, std::vector<Market>* const markets_out) {
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

      markets.emplace_back(market);
    }

    *markets_out = markets;
  }

  static void displaySortedMarketList(std::vector<Market>& markets, const float advantage_threshold) {
    // Sort the markets by the advantage they offer. This puts them in ascending order.
    std::sort(markets.begin(), markets.end(),
        [](const Market& a, const Market& b) {return a.advantage < b.advantage;});

    // Flip the order so that we're in descending order, which is more useful to us.
    std::reverse(markets.begin(), markets.end());

    std::cout << "Displaying markets with advantage >= " << advantage_threshold << "..." << std::endl;
    for (const Market& market : markets) {
      if (market.advantage < advantage_threshold) {
        break;
      }
      char buf[1000];
      snprintf(buf, 1000, "%+5.2f @ #%d: %s", market.advantage, market.id, market.name.c_str());
      std::cout << buf << std::endl;
    }
    
    if (markets.empty()) {
      std::cout << "(No markets meet the criteria.)" << std::endl;
    }
  }

  static void processJson(const char* const file_name) {
    std::ifstream t(file_name);
    std::stringstream buffer;
    buffer << t.rdbuf();

    std::vector<Market> markets;
    buildMarketListJson(buffer, &markets);
    if (markets.empty()) {
      std::cout << "No markets can be found in the JSON file. (Is PredictIt down?)" << std::endl;
    } else {
      displaySortedMarketList(markets, 0.05f);
    }
    std::cout << std::endl;
  }

  static void calculateProfitMin(const Market& market, const float fee_pct, float* const profit_min_out) {
    if (profit_min_out == nullptr) {
      return;
    }

    *profit_min_out = 0;

    return;
  }
};
