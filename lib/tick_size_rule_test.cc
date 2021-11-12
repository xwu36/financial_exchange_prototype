#include "lib/tick_size_rule.h"

#include "gtest/gtest.h"

namespace fep::lib
{

  using ::fep::lib::Price4;
  using ::nlohmann::json;

  TEST(TickSizeRuleTest, CorrectInput)
  {
    json j = {
        {{"from_price", "0"}, {"to_price", "1"}, {"tick_size", 0.0001}},
        {{"from_price", "3"}, {"tick_size", 1}},
        {{"from_price", "1"}, {"to_price", "3"}, {"tick_size", 0.01}},
    };
    TickSizeRule rule;
    EXPECT_TRUE(rule.FromJson(j));
    const auto &ticks = rule.GetTicks();
    EXPECT_EQ(ticks.size(), 3);
    EXPECT_EQ(ticks[0].from_price, Price4("0"));
    EXPECT_EQ(ticks[0].to_price, Price4("1"));
    EXPECT_EQ(ticks[0].tick_size, 0.0001);

    EXPECT_EQ(ticks[1].from_price, Price4("1"));
    EXPECT_EQ(ticks[1].to_price, Price4("3"));
    EXPECT_EQ(ticks[1].tick_size, 0.01);

    EXPECT_EQ(ticks[2].from_price, Price4("3"));
    EXPECT_EQ(ticks[2].to_price, Price4("0"));
    EXPECT_EQ(ticks[2].tick_size, 1);
  }

  TEST(TickSizeRuleTest, NotConsecutiveIntervals)
  {
    json j = {
        {{"from_price", "0"}, {"to_price", "1"}, {"tick_size", 0.0001}},
        {{"from_price", "3"}, {"tick_size", 1}},
        {{"from_price", "2"}, {"to_price", "3"}, {"tick_size", 0.01}},
    };
    TickSizeRule rule;
    EXPECT_FALSE(rule.FromJson(j));
  }

  TEST(TickSizeRuleTest, FromPriceGreaterThanToPrice)
  {
    json j = {
        {{"from_price", "2"}, {"to_price", "1"}, {"tick_size", 0.0001}},
        {{"from_price", "3"}, {"tick_size", 1}},
    };
    TickSizeRule rule;
    EXPECT_FALSE(rule.FromJson(j));
  }

} // namespace fep::libs