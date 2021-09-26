#include "main/hello_greet.h"
#include <string>

namespace fep::main {

std::string get_greet(const std::string &who) { return "Hello " + who; }

} // namespace fep::main