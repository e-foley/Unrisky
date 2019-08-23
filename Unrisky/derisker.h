#pragma once

#include "json11.hpp"
//#include "tinyxml2.h"
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

  //static void process(const char* const file_name) {
  //  std::vector<MarketData> markets;
  //  tinyxml2::XMLDocument document;
  //  const tinyxml2::XMLError result = document.LoadFile(file_name);

  //  if (result != tinyxml2::XML_SUCCESS) {
  //    std::cout << "Could not load file " << file_name << ". Code " << static_cast<int>(result) << "\n";
  //  }

  //  tinyxml2::XMLHandle document_handle(&document);
  //  tinyxml2::XMLElement* market_element = document_handle.FirstChildElement("MarketList").FirstChildElement("Markets").FirstChildElement("MarketData").ToElement();

  //  while (market_element != nullptr) {
  //    MarketData data;
  //    tinyxml2::XMLElement* id_element = market_element->FirstChildElement("ID");
  //    if (id_element == nullptr) {
  //      std::cout << "Could not find ID element in MarketData.\n";
  //    } else if (id_element->QueryIntText(&data.id) != tinyxml2::XML_SUCCESS) {
  //      std::cout << "Could not parse ID in MarketData.\n";
  //    }

  //    tinyxml2::XMLElement* name_element = market_element->FirstChildElement("Name");
  //    if (name_element == nullptr) {
  //      std::cout << "Could not find Name element in MarketData.\n";
  //    } else {
  //      const char* name_text = name_element->GetText();
  //      if (name_text == nullptr) {
  //        std::cout << "Empty Name element in MarketData.\n";
  //      }
  //      else {
  //        data.name = name_text;
  //      }
  //    }
  //    
  //    // Enumerate all the contracts within, noting the cumulative cost of buying all the nos.
  //    float no_sum = 0.0f;
  //    int num_contracts = 0;
  //    tinyxml2::XMLElement* contract_element = tinyxml2::XMLHandle(market_element).FirstChildElement("Contracts").FirstChildElement("MarketContract").ToElement();

  //    while (contract_element != nullptr) {
  //      ++num_contracts;
  //      tinyxml2::XMLElement* best_buy_no_cost_element = contract_element->FirstChildElement("BestBuyNoCost");
  //      if (best_buy_no_cost_element == nullptr) {
  //        std::cout << "Could not find BustBuyNoCost within MarketContract\n";
  //      } else {
  //        float no_cost = 0.0f;
  //        if (best_buy_no_cost_element->QueryFloatText(&no_cost) != tinyxml2::XML_SUCCESS) {
  //          // No text is present when the value is "N/A." For us, this is same as buying at 100 cents.
  //          no_cost = 1.0f;
  //        }
  //        no_sum += no_cost;
  //      }
  //      contract_element = contract_element->NextSiblingElement("MarketContract");
  //    }

  //    // Advantage is what we get (in worst case) less what we pay.
  //    data.advantage = num_contracts - 1 - no_sum;
  //    markets.push_back(data);

  //    market_element = market_element->NextSiblingElement("MarketData");
  //  }

  //  // At this point, we've gone through all the markets.
  //  for (const MarketData& data : markets) {
  //    std::cout << data.advantage << " for " << data.id << ": " << data.name.c_str() << "\n";
  //  }
  //}
};
