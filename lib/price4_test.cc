#include "lib/price4.h"

#include "gtest/gtest.h"

namespace fep::lib {
namespace {

TEST(Price4Test, ParsedFromStringAllDigits) {
  const Price4 price("1234");
  EXPECT_EQ(price.unscaled(), 12340000);
}

TEST(Price4Test, ParsedFromStringWithAPeriod) {
  const Price4 price("12.34");
  EXPECT_EQ(price.unscaled(), 123400);
}

TEST(Price4Test, ParsedFromStringFailedDueToTwoPeriods) {
  const Price4 price("12.3.4");
  EXPECT_EQ(price.unscaled(), 0);
}

TEST(Price4Test, ParsedFromStringFailedDueToAColon) {
  const Price4 price("12:34");
  EXPECT_EQ(price.unscaled(), 0);
}

TEST(Price4Test, ToString) {
  const Price4 price(99000);
  EXPECT_EQ(price.unscaled(), 99000);
  EXPECT_EQ(price.to_str(), "9.90");
}

TEST(Price4Test, CompareOperators) {
  const Price4 price1(1);
  const Price4 price2(2);
  EXPECT_TRUE(price1 == price1);
  EXPECT_TRUE(price1 != price2);
  EXPECT_TRUE(price1 <= price1);
  EXPECT_TRUE(price1 <= price2);
  EXPECT_TRUE(price1 < price2);
  EXPECT_TRUE(price2 > price1);
  EXPECT_TRUE(price2 >= price1);
  EXPECT_TRUE(price2 >= price2);
}

} // namespace
} // namespace fep::lib