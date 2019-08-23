#pragma once

#include "json11.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>
#include <sstream>
#include <algorithm>

struct MarketData {
  int id = -1;
  std::string name;
  float advantage = 0.0f;
};

class Derisker {
public:
  static void buildMarketListJson(const std::stringstream& buffer, std::vector<MarketData>* const markets_out) {
    std::vector<MarketData> markets;
    using Json = json11::Json;
    Json json;
    std::string error;
    json = Json::parse(buffer.str(), error);

    const Json::array markets_json = json["markets"].array_items();

    for (const auto& market_json : markets_json) {
      MarketData data;
      data.id = market_json["id"].int_value();
      data.name = market_json["name"].string_value();

      float buy_no_cost_sum = 0.0f;

      const Json::array contracts_json = market_json["contracts"].array_items();
      for (const auto& contract_json : contracts_json) {
        Json best_buy_no_cost = contract_json["bestBuyNoCost"];
        switch (best_buy_no_cost.type()) {
        case Json::NUL:
          buy_no_cost_sum += 1.00f;
          break;
        case Json::NUMBER:
          buy_no_cost_sum += static_cast<float>(best_buy_no_cost.number_value());
          break;
        default:
          buy_no_cost_sum += 1.00f;
          std::cout << "Unknown bestBuyNoCost type." << std::endl;
          break;
        }
      }

      data.advantage = contracts_json.size() - 1 - buy_no_cost_sum;

      markets.emplace_back(data);
    }

    *markets_out = markets;
  }

  static void displaySortedMarketList(std::vector<MarketData>& markets, const float advantage_threshold) {
    // Sort the markets by the advantage they offer. This puts them in ascending order.
    std::sort(markets.begin(), markets.end(),
        [](const MarketData& a, const MarketData& b) {return a.advantage < b.advantage;});

    // Flip the order so that we're in descending order, which is more useful to us.
    std::reverse(markets.begin(), markets.end());

    std::cout << "Displaying markets with advantage >= " << advantage_threshold << "..." << std::endl;
    for (const MarketData& market : markets) {
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

    std::vector<MarketData> markets;
    buildMarketListJson(buffer, &markets);
    if (markets.empty()) {
      std::cout << "No markets can be found in the JSON file. (Is PredictIt down?)" << std::endl;
    } else {
      displaySortedMarketList(markets, 0.05f);
    }
    std::cout << std::endl;
  }
};
