#include "lib/trade_message.h"

#include "gtest/gtest.h"

namespace fep::lib
{
  namespace
  {

    TEST(TradeMessageTest, TradeResultToString)
    {
      const TradeResult result(Price4("12.32"), 200);
      EXPECT_EQ(result.to_str(), std::string("{\"price\":\"12.32\",\"quantity\":200,\"type\":\"TRADE\"}"));
    }

    TEST(TradeMessageTest, AddActionToString)
    {
      const TradeAction result(Price4("12.32"), 200, kAddAction);
      EXPECT_EQ(result.to_str(), std::string("{\"action\":\"ADD\",\"price\":\"12.32\",\"quantity\":200}"));
    }

    TEST(TradeMessageTest, TradeUpdate)
    {
      TradeUpdate update;
      update.bids.push_back(TradeAction(Price4("12.32"), 200, kAddAction));
      update.bids.push_back(TradeAction(Price4("11.32"), 100, kAddAction));
      update.asks.push_back(TradeAction(Price4("13.32"), 300, kDeleteAction));
      update.asks.push_back(TradeAction(Price4("14.32"), 400, kModifyAction));
      EXPECT_EQ(update.to_str(), std::string("[{\"type\":\"DEPTH_UPDATE\"},{\"ask\":[\"{\\\"action\\\":\\\"DELETE\\\",\\\"price\\\":\\\"13.32\\\",\\\"quantity\\\":300}\",\"{\\\"action\\\":\\\"MODIFY\\\",\\\"price\\\":\\\"14.32\\\",\\\"quantity\\\":400}\"],\"bid\":[\"{\\\"action\\\":\\\"ADD\\\",\\\"price\\\":\\\"12.32\\\",\\\"quantity\\\":200}\",\"{\\\"action\\\":\\\"ADD\\\",\\\"price\\\":\\\"11.32\\\",\\\"quantity\\\":100}\"]},null]"));
    }

    TEST(TradeMessageTest, TradeMessage)
    {
      TradeMessage message;
      message.trade_results.push_back(TradeResult(Price4("12.32"), 200));
      message.trade_results.push_back(TradeResult(Price4("11.32"), 100));
      message.trade_update.bids.push_back(TradeAction(Price4("11.32"), 100, kAddAction));
      message.trade_update.bids.push_back(TradeAction(Price4("14.32"), 400, kModifyAction));
      EXPECT_EQ(message.to_str(), std::string("{\"price\":\"12.32\",\"quantity\":200,\"type\":\"TRADE\"}/n{\"price\":\"11.32\",\"quantity\":100,\"type\":\"TRADE\"}/n[{\"type\":\"DEPTH_UPDATE\"},{\"bid\":[\"{\\\"action\\\":\\\"ADD\\\",\\\"price\\\":\\\"11.32\\\",\\\"quantity\\\":100}\",\"{\\\"action\\\":\\\"MODIFY\\\",\\\"price\\\":\\\"14.32\\\",\\\"quantity\\\":400}\"]},null]"));
    }

  } // namespace
} // namespace fep::lib