#include "main/hello_greet.h"
#include "gtest/gtest.h"

namespace fep::main {
namespace {

TEST(HelloTest, GetGreet) { EXPECT_EQ(get_greet("Bazel"), "Hello Bazel"); }

} // namespace
} // namespace fep::main