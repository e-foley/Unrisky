#pragma once

#include "contract.h"

// Represents ownership of a single contract.
struct Stake {
  Contract contract;
  int num_shares = 0;

  // TODO: Consider C-style static function equivalent for this and other derived stats (e.g. getRisk()).
  // NOTE: Experimental implementation to get hang of handling output parameters.
  float getValue() const {
    float ret;
    if (!tryGetValue(&ret)) {
      return 0.0f;
    }

    return ret;
  }

  // Experimental alternative syntax.
  bool tryGetValue(float* const out_value) const {
    if (out_value == nullptr) {
      return false;
    }

    *out_value = num_shares * contract.best_buy_no_cost;
    return true;
  }
};
