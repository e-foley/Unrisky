#pragma once

#include "contract.h"
#include <list>
#include <string>

// Information about a market, or a collection of linked contracts.
struct Market {
  int id = -1;
  std::string name;
  float advantage = 0.0f;  // TODO: Move me?
  float risk = 0.00f;  // TODO: Move me?
  std::list<Contract> contracts;
};
