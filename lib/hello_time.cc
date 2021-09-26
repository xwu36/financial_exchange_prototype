#include "lib/hello_time.h"

#include <ctime>
#include <iostream>

namespace fep::lib {

void print_localtime() {
  std::time_t result = std::time(nullptr);
  std::cout << std::asctime(std::localtime(&result));
}

} // namespace fep::lib
