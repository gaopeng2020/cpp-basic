// A simple program that computes the square root of a number
#include <cmath>
#include <iostream>
#include <string>

#include "MathFunctions.h"
#include "TutorialConfig.h"

int main(int argc, char* argv[])
{
  // Check if command-line argument count is sufficient
  if (argc < 2) {
    std::cout << argv[0] << " Version " << Tutorial_VERSION_MAJOR << "." 
              << Tutorial_VERSION_MINOR << std::endl;
    std::cout << "Usage: " << argv[0] << " number" << std::endl;
    return 1;
  }

  // Convert input string to double
  const double inputValue = std::stod(argv[1]);

  // Calculate square root using provided function
  const double outputValue = mathfunctions::sqrt(inputValue);

  // Output the result
  std::cout << "The square root of " << inputValue << " is " << outputValue 
            << std::endl;
  return 0;
}

