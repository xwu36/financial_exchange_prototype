#include <fstream>
#include <iostream>
#include <string>

#include "lib/hello_time.h"
#include "main/hello_greet.h"
#include "nlohmann/json.hpp"

int main(int argc, char **argv) {
  std::string who = "world";
  if (argc > 1) {
    who = argv[1];
  }
  std::cout << fep::main::get_greet(who) << std::endl;
  fep::lib::print_localtime();

  std::ifstream ifs("main/hello_world_message.json");
  std::string line; 
  nlohmann::json j = nlohmann::json::parse(ifs);
  std::cout << "Message from json message is: " << j << std::endl;
}
