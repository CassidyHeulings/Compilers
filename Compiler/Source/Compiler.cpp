// language
#include <iostream>
#include <fstream>
using namespace std;
// headers
#include "../Headers/Logger.hpp"

int main() {
  // construct each class instance
  Logger logger(true);

  // collect input
  std::ifstream inputFile("Compiler/Input.txt");
  std::string line;
  std::string programs;
  // make sure text file is found
  if (!inputFile.is_open()) {
      logger.info("Error: Input.txt not found.");
      return 1;
  }
  // read the file
  while (std::getline(inputFile, line)) {
      programs += line;
  }

  inputFile.close(); // Close the file stream

  // start the program
  logger.info("Reading programs.");
  logger.debug("Input: " + programs);

  return 0;
}