#include "derisker.h"
#include <iostream>

int main(int argc, char* argv[]) {
  switch (argc) {
    default:
    case 0:
    case 1:
      std::cout << "Usage: Unrisky.exe json_file [min_risk]" << std::endl;
      break;
    case 2:
      Derisker::processJson(argv[1], 0.00f);
      break;
    case 3:
      Derisker::processJson(argv[1], static_cast<float>(atof(argv[2])));
      break;
  }

  // Make a pretend market...
  /*Market market;
  market.id = 123;
  market.name = "My market";
  market.contracts.push_back({ 1, 0.91f });
  market.contracts.push_back({ 2, 0.91f });
  market.contracts.push_back({ 3, 0.88f });
  market.contracts.push_back({ 4, 0.85f });
  market.contracts.push_back({ 5, 0.82f });
  market.contracts.push_back({ 6, 0.84f });
  market.contracts.push_back({ 7, 0.70f });

  MarketOwnership ownership;
  std::cout << "hello" << std::endl;
  Derisker::getIdealMarketOwnership(market, 850.00f, 0.10f, &ownership);
  ownership.printOwnership();
  std::cout << ownership.getRisk(0.10f) << std::endl;*/
}
