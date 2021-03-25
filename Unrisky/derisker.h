#pragma once

//#include "contract.h"
#include "market.h"
#include "market_ownership.h"
//#include "stake.h"
//#include "json11.hpp"
//#include <iostream>
//#include <fstream>
//#include <list>
//#include <string>
//#include <iomanip>
#include <vector>
//#include <sstream>
//#include <algorithm>

class Derisker {
  public:
    static void buildMarketListJson(const std::stringstream& buffer, std::vector<Market>* const markets_out);

    static void displaySortedMarketList(const std::vector<Market>& markets, const float risk_threshold);

    static void processJson(const char* const file_name, const float min_risk);

    static void getIdealMarketOwnership(const Market& market, const float max_stake, const float fee_ratio,
        MarketOwnership* const market_ownership_out, float* risk_out);
};
