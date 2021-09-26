#include "lib/hello_time.h"
#include "main/hello_greet.h"
#include <iostream>
#include <string>

int main(int argc, char **argv) {
  std::string who = "world";
  if (argc > 1) {
    who = argv[1];
  }
  std::cout << fep::main::get_greet(who) << std::endl;
  fep::lib::print_localtime();
  return 0;
}
