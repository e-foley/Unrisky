#pragma once

#include "stake.h"
#include <iostream>
#include <list>

// Represents ownership of multiple contracts within one market.
struct MarketOwnership {
  std::list<Stake> stakes;

  float getRisk(const float fee_ratio) const {
    if (stakes.empty()) {
      std::cout << "BAD" << std::endl;
      return 0.00f;
    }

    // Utility struct to link a stake with other data.
    struct RiskHelper {
      Stake stake;
      float value = 0.00f;
      float no_result_worth = 0.00f;
      float risk = 0.00f;
    };

    std::list<RiskHelper> risk_helpers;

    // Build risk helpers by working through stakes twice:
    // 1) Copy stake, copy value, and calculate worth in case contract resolves "no."
    // 2) Calculate risk.
    float value_sum = 0.00f;
    float no_result_worth_sum = 0.00f;
    for (const Stake& stake : stakes) {
      RiskHelper risk_helper;
      risk_helper.stake = stake;
      risk_helper.value = stake.getValue();
      // You maintain the original value and receive a portion of your profit.
      risk_helper.no_result_worth = risk_helper.value +
        (1.0f - fee_ratio) * stake.num_shares * (1.00f - stake.contract.best_buy_no_cost);

      risk_helpers.push_back(risk_helper);
      value_sum += risk_helper.value;
      no_result_worth_sum += risk_helper.no_result_worth;
    }

    float min_risk = INFINITY;
    for (RiskHelper& risk_helper : risk_helpers) {
      // Risk is what you would win less what you have paid total if contract resolves "yes."
      risk_helper.risk = no_result_worth_sum - risk_helper.no_result_worth - value_sum;
      if (risk_helper.risk < min_risk) {
        min_risk = risk_helper.risk;
      }
    }

    return min_risk;
  }

  void printOwnership() const {
    std::list<Stake>::const_reverse_iterator it;
    for (it = stakes.crbegin(); it != stakes.crend(); ++it) {
      char buf[1000];
      snprintf(buf, 1000, "#%d (%4.2f) x %d", it->contract.id, it->contract.best_buy_no_cost, it->num_shares);
      std::cout << buf << std::endl;
    }
  }
};
