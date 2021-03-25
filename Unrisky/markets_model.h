#pragma once

#include "derisker.h"

class MarketsModel {
  public:
    // Temporarily public until add accessors, etc.
    std::list<Market> markets_{};
};
