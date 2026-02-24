// language
#include <iostream>
using namespace std;
// headers
#include "../Headers/Logger.hpp"

int main() {
  // construct each class instance
  Logger logger(true);

  // start the program
  logger.debug("This is a debug statement.");
  logger.info("This is an info statement.");

  return 0;
}