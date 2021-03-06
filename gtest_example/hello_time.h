#ifndef GTEST_EXAMPLE_HELLO_TIME_H_
#define GTEST_EXAMPLE_HELLO_TIME_H_

#include <string>

#include "absl/strings/string_view.h"

namespace fep::gtest_example {

void print_localtime();

class HelloTime {
public:
  inline absl::string_view GetName() const { return kName; }
  inline int GetCount() const { return 33; }

private:
  const std::string kName = "Hello Time";
};

} // namespace fep::gtest_example

#endif