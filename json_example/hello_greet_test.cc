#include "json_example/hello_greet.h"
#include "gtest/gtest.h"

namespace fep::json_example {
namespace {

TEST(HelloTest, GetGreet) { EXPECT_EQ(get_greet("Bazel"), "Hello Bazel"); }

} // namespace
} // namespace fep::json_example