#include "json_example/hello_greet.h"
#include <string>

namespace fep::json_example {

std::string get_greet(const std::string &who) { return "Hello " + who; }

} // namespace fep::json_example