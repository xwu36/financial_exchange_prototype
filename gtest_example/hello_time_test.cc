#include "gtest_example/hello_time.h"

#include "gtest/gtest.h"

namespace fep::gtest_example {
namespace {

TEST(HelloTimeTest, GetName) {
  const HelloTime hello_time;
  EXPECT_EQ(hello_time.GetName(), "Hello Time");
}

} // namespace
} // namespace fep::gtest_example