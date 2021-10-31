#include "srcs/matching_engine/matching_engine.h"

#include "gtest/gtest.h"
#include "srcs/order/order.h"

namespace fep::srcs::matching_engine
{
  namespace
  {
    using ::nlohmann::json;
    using ::fep::srcs::order::Order;

    TEST(MatchingEngineTest, SellOrder)
    {
      MatchingEngine engine;
      const json data8 = {{"time", 10},
                                   {"type", "NEW"},
                                   {"order_id", 8},
                                   {"symbol", "AAPL"},
                                   {"side", "BUY"},
                                   {
                                       "quantity",
                                       500,
                                   },
                                   {"limit_price", "9.90"},
                                   {"order_type", "LIMIT"}};
      std::unique_ptr<Order> order8 = std::make_unique<Order>(data8);
      const auto& status1 = engine.Process(std::move(order8));
      ASSERT_TRUE(status1.ok());
      EXPECT_EQ(status1->to_str(), "/n[{\"type\":\"DEPTH_UPDATE\"},{\"bid\":[\"{\\\"action\\\":\\\"ADD\\\",\\\"price\\\":\\\"9.90\\\",\\\"quantity\\\":500}\"]},null]");

      const json data9 = {{"time", 10},
                                   {"type", "NEW"},
                                   {"order_id", 9},
                                   {"symbol", "AAPL"},
                                   {"side", "BUY"},
                                   {
                                       "quantity",
                                       500,
                                   },
                                   {"limit_price", "9.99"},
                                   {"order_type", "LIMIT"}};
      std::unique_ptr<Order> order9 = std::make_unique<Order>(data9);
      const auto& status2 = engine.Process(std::move(order9));
      ASSERT_TRUE(status2.ok());
      EXPECT_EQ(status2->to_str(), "/n[{\"type\":\"DEPTH_UPDATE\"},{\"bid\":[\"{\\\"action\\\":\\\"ADD\\\",\\\"price\\\":\\\"9.99\\\",\\\"quantity\\\":500}\"]},null]");

      const json data5 = {{"time", 10},
                                   {"type", "NEW"},
                                   {"order_id", 5},
                                   {"symbol", "AAPL"},
                                   {"side", "BUY"},
                                   {
                                       "quantity",
                                       500,
                                   },
                                   {"limit_price", "10.00"},
                                   {"order_type", "LIMIT"}};
      std::unique_ptr<Order> order5 = std::make_unique<Order>(data5);
      const auto& status3 = engine.Process(std::move(order5));
      ASSERT_TRUE(status3.ok());
      EXPECT_EQ(status3->to_str(), "/n[{\"type\":\"DEPTH_UPDATE\"},{\"bid\":[\"{\\\"action\\\":\\\"ADD\\\",\\\"price\\\":\\\"10.00\\\",\\\"quantity\\\":500}\"]},null]");

      const json data10 = {{"time", 10},
                                   {"type", "NEW"},
                                   {"order_id", 10},
                                   {"symbol", "AAPL"},
                                   {"side", "BUY"},
                                   {
                                       "quantity",
                                       200,
                                   },
                                   {"limit_price", "10.01"},
                                   {"order_type", "LIMIT"}};
      std::unique_ptr<Order> order10 = std::make_unique<Order>(data10);
      const auto& status4 = engine.Process(std::move(order10));
      ASSERT_TRUE(status4.ok());
      EXPECT_EQ(status4->to_str(), "/n[{\"type\":\"DEPTH_UPDATE\"},{\"bid\":[\"{\\\"action\\\":\\\"ADD\\\",\\\"price\\\":\\\"10.01\\\",\\\"quantity\\\":200}\"]},null]");

      const json data1 = {{"time", 10},
                                   {"type", "NEW"},
                                   {"order_id", 10},
                                   {"symbol", "AAPL"},
                                   {"side", "BUY"},
                                   {
                                       "quantity",
                                       100,
                                   },
                                   {"limit_price", "10.01"},
                                   {"order_type", "LIMIT"}};
      std::unique_ptr<Order> order1 = std::make_unique<Order>(data1);
      const auto& status5 = engine.Process(std::move(order1));
      ASSERT_TRUE(status5.ok());
      EXPECT_EQ(status5->to_str(), "/n[{\"type\":\"DEPTH_UPDATE\"},{\"bid\":[\"{\\\"action\\\":\\\"ADD\\\",\\\"price\\\":\\\"10.01\\\",\\\"quantity\\\":200}\"]},null]");
    }

  }

} // namespace fep::srcs::matching_engine