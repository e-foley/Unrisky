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
}
