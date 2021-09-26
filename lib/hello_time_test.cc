#include "lib/hello_time.h"

#include "gtest/gtest.h"

namespace fep::lib {
namespace {

TEST(HelloTimeTest, GetName) {
  const HelloTime hello_time;
  EXPECT_EQ(hello_time.GetName(), "Hello Time");
}

} // namespace
} // namespace fep::lib