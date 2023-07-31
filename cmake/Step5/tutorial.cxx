// A simple program that computes the square root of a number
#include <cmath>
#include <iostream>
#include <string>

#include "MathFunctions.h"
#include "TutorialConfig.h"

int main(int argc, char *argv[])
{
  // report version
#if (defined Tutorial_VERSION_MAJOR && defined Tutorial_VERSION_MINOR)
  std::cout << "Version " << Tutorial_VERSION_MAJOR << "."
            << Tutorial_VERSION_MINOR << std::endl;
#endif

#ifdef BUILD_TIME
  std::cout << "timestamp is " << BUILD_TIME << std::endl;
#endif

#ifdef GIT_INFO
  std::cout << "git is " << GIT_INFO << std::endl;
#endif

  // convert input to double
  const double inputValue = std::stod(argv[1]);

  const double outputValue = mathfunctions::sqrt(inputValue);

  std::cout << "The square root of " << inputValue << " is " << outputValue
            << std::endl;
  return 0;
}
