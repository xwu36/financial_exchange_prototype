#include "srcs/matching_engine/matching_engine_impl.h"

#include "gtest/gtest.h"
#include "srcs/order/order.h"

namespace fep::srcs::matching_engine
{
    namespace
    {
        using ::fep::lib::TickSizeRule;
        using ::fep::srcs::order::Order;
        using ::nlohmann::json;

        class MatchingEngineImplTest : public testing::Test
        {
        public:
            void SetUp() override
            {
                json j = {
                    {{"from_price", "0"}, {"to_price", "1"}, {"tick_size", 0.0001}},
                    {{"from_price", "1"}, {"tick_size", 0.01}},
                };
                rule_.FromJson(j);
            }

        protected:
            TickSizeRule rule_;
        };

        TEST_F(MatchingEngineImplTest, ProcessLimitOrders)
        {
            MatchingEngineImpl engine(rule_, /*lot_size=*/10);
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
            const auto &status1 = engine.Process(std::move(order8));
            ASSERT_TRUE(status1.ok());
            EXPECT_EQ(status1->to_str(), "[{\"type\":\"DEPTH_UPDATE\"},{\"bid\":[\"{\\\"action\\\":\\\"ADD\\\",\\\"price\\\":\\\"9.90\\\",\\\"quantity\\\":500}\"]},{\"ask\":null}]");

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
            const auto &status2 = engine.Process(std::move(order9));
            ASSERT_TRUE(status2.ok());
            EXPECT_EQ(status2->to_str(), "[{\"type\":\"DEPTH_UPDATE\"},{\"bid\":[\"{\\\"action\\\":\\\"ADD\\\",\\\"price\\\":\\\"9.99\\\",\\\"quantity\\\":500}\"]},{\"ask\":null}]");

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
            const auto &status3 = engine.Process(std::move(order5));
            ASSERT_TRUE(status3.ok());
            EXPECT_EQ(status3->to_str(), "[{\"type\":\"DEPTH_UPDATE\"},{\"bid\":[\"{\\\"action\\\":\\\"ADD\\\",\\\"price\\\":\\\"10.00\\\",\\\"quantity\\\":500}\"]},{\"ask\":null}]");

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
            const auto &status4 = engine.Process(std::move(order10));
            ASSERT_TRUE(status4.ok());
            EXPECT_EQ(status4->to_str(), "[{\"type\":\"DEPTH_UPDATE\"},{\"bid\":[\"{\\\"action\\\":\\\"ADD\\\",\\\"price\\\":\\\"10.01\\\",\\\"quantity\\\":200}\"]},{\"ask\":null}]");

            const json data1 = {{"time", 9},
                                {"type", "NEW"},
                                {"order_id", 1},
                                {"symbol", "AAPL"},
                                {"side", "BUY"},
                                {
                                    "quantity",
                                    100,
                                },
                                {"limit_price", "10.01"},
                                {"order_type", "LIMIT"}};
            std::unique_ptr<Order> order1 = std::make_unique<Order>(data1);
            const auto &status5 = engine.Process(std::move(order1));
            ASSERT_TRUE(status5.ok());
            EXPECT_EQ(status5->to_str(), "[{\"type\":\"DEPTH_UPDATE\"},{\"bid\":[\"{\\\"action\\\":\\\"MODIFY\\\",\\\"price\\\":\\\"10.01\\\",\\\"quantity\\\":300}\"]},{\"ask\":null}]");

            const json data12 = {{"time", 10},
                                 {"type", "NEW"},
                                 {"order_id", 12},
                                 {"symbol", "AAPL"},
                                 {"side", "SELL"},
                                 {
                                     "quantity",
                                     100,
                                 },
                                 {"limit_price", "10.02"},
                                 {"order_type", "LIMIT"}};
            std::unique_ptr<Order> order12 = std::make_unique<Order>(data12);
            const auto &status6 = engine.Process(std::move(order12));
            ASSERT_TRUE(status6.ok());
            EXPECT_EQ(status6->to_str(), "[{\"type\":\"DEPTH_UPDATE\"},{\"bid\":null},{\"ask\":[\"{\\\"action\\\":\\\"ADD\\\",\\\"price\\\":\\\"10.02\\\",\\\"quantity\\\":100}\"]}]");

            const json data2 = {{"time", 9},
                                {"type", "NEW"},
                                {"order_id", 2},
                                {"symbol", "AAPL"},
                                {"side", "SELL"},
                                {
                                    "quantity",
                                    500,
                                },
                                {"limit_price", "10.03"},
                                {"order_type", "LIMIT"}};
            std::unique_ptr<Order> order2 = std::make_unique<Order>(data2);
            const auto &status7 = engine.Process(std::move(order2));
            ASSERT_TRUE(status7.ok());
            EXPECT_EQ(status7->to_str(), "[{\"type\":\"DEPTH_UPDATE\"},{\"bid\":null},{\"ask\":[\"{\\\"action\\\":\\\"ADD\\\",\\\"price\\\":\\\"10.03\\\",\\\"quantity\\\":500}\"]}]");

            const json data3 = {{"time", 10},
                                {"type", "NEW"},
                                {"order_id", 3},
                                {"symbol", "AAPL"},
                                {"side", "SELL"},
                                {
                                    "quantity",
                                    200,
                                },
                                {"limit_price", "10.03"},
                                {"order_type", "LIMIT"}};
            std::unique_ptr<Order> order3 = std::make_unique<Order>(data3);
            const auto &status8 = engine.Process(std::move(order3));
            ASSERT_TRUE(status8.ok());
            EXPECT_EQ(status8->to_str(), "[{\"type\":\"DEPTH_UPDATE\"},{\"bid\":null},{\"ask\":[\"{\\\"action\\\":\\\"MODIFY\\\",\\\"price\\\":\\\"10.03\\\",\\\"quantity\\\":700}\"]}]");

            const json data14 = {{"time", 9},
                                 {"type", "NEW"},
                                 {"order_id", 14},
                                 {"symbol", "AAPL"},
                                 {"side", "SELL"},
                                 {
                                     "quantity",
                                     1000,
                                 },
                                 {"limit_price", "10.04"},
                                 {"order_type", "LIMIT"}};
            std::unique_ptr<Order> order14 = std::make_unique<Order>(data14);
            const auto &status9 = engine.Process(std::move(order14));
            ASSERT_TRUE(status9.ok());
            EXPECT_EQ(status9->to_str(), "[{\"type\":\"DEPTH_UPDATE\"},{\"bid\":null},{\"ask\":[\"{\\\"action\\\":\\\"ADD\\\",\\\"price\\\":\\\"10.04\\\",\\\"quantity\\\":1000}\"]}]");

            // Case 1 with Order15.
            const json data15 = {{"time", 20},
                                 {"type", "NEW"},
                                 {"order_id", 15},
                                 {"symbol", "AAPL"},
                                 {"side", "BUY"},
                                 {
                                     "quantity",
                                     200,
                                 },
                                 {"limit_price", "10.03"},
                                 {"order_type", "LIMIT"}};
            std::unique_ptr<Order> order15 = std::make_unique<Order>(data15);
            const auto &status10 = engine.Process(std::move(order15));
            ASSERT_TRUE(status10.ok());
            EXPECT_EQ(status10->to_str(), "{\"price\":\"10.02\",\"quantity\":100,\"type\":\"TRADE\"}/n{\"price\":\"10.03\",\"quantity\":100,\"type\":\"TRADE\"}/n[{\"type\":\"DEPTH_UPDATE\"},{\"bid\":null},{\"ask\":[\"{\\\"action\\\":\\\"DELETE\\\",\\\"price\\\":\\\"10.02\\\",\\\"quantity\\\":0}\",\"{\\\"action\\\":\\\"MODIFY\\\",\\\"price\\\":\\\"10.03\\\",\\\"quantity\\\":600}\"]}]");

            // Case 2 with Order16.
            const json data16 = {{"time", 21},
                                 {"type", "NEW"},
                                 {"order_id", 16},
                                 {"symbol", "AAPL"},
                                 {"side", "SELL"},
                                 {
                                     "quantity",
                                     600,
                                 },
                                 {"limit_price", "10.02"},
                                 {"order_type", "LIMIT"}};
            std::unique_ptr<Order> order16 = std::make_unique<Order>(data16);
            const auto &status11 = engine.Process(std::move(order16));
            ASSERT_TRUE(status11.ok());
            EXPECT_EQ(status11->to_str(), "[{\"type\":\"DEPTH_UPDATE\"},{\"bid\":null},{\"ask\":[\"{\\\"action\\\":\\\"ADD\\\",\\\"price\\\":\\\"10.02\\\",\\\"quantity\\\":600}\"]}]");

            // Case 3 with Order17.
            const json data17 = {{"time", 22},
                                 {"type", "NEW"},
                                 {"order_id", 17},
                                 {"symbol", "AAPL"},
                                 {"side", "SELL"},
                                 {
                                     "quantity",
                                     1000,
                                 },
                                 {"limit_price", "10.00"},
                                 {"order_type", "LIMIT"}};
            std::unique_ptr<Order> order17 = std::make_unique<Order>(data17);
            const auto &status12 = engine.Process(std::move(order17));
            ASSERT_TRUE(status12.ok());
            EXPECT_EQ(status12->to_str(), "{\"price\":\"10.01\",\"quantity\":100,\"type\":\"TRADE\"}/n{\"price\":\"10.01\",\"quantity\":200,\"type\":\"TRADE\"}/n{\"price\":\"10.00\",\"quantity\":500,\"type\":\"TRADE\"}/n[{\"type\":\"DEPTH_UPDATE\"},{\"bid\":[\"{\\\"action\\\":\\\"DELETE\\\",\\\"price\\\":\\\"10.00\\\",\\\"quantity\\\":0}\",\"{\\\"action\\\":\\\"DELETE\\\",\\\"price\\\":\\\"10.01\\\",\\\"quantity\\\":0}\"]},{\"ask\":[\"{\\\"action\\\":\\\"ADD\\\",\\\"price\\\":\\\"10.00\\\",\\\"quantity\\\":200}\"]}]");

            // Case 4 with Order18.
            const json data18 = {{"time", 23},
                                 {"type", "CANCEL"},
                                 {"order_id", 2}};
            std::unique_ptr<Order> order18 = std::make_unique<Order>(data18);
            const auto &status13 = engine.Process(std::move(order18));
            ASSERT_TRUE(status13.ok());
            EXPECT_EQ(status13->to_str(), "[{\"type\":\"DEPTH_UPDATE\"},{\"bid\":null},{\"ask\":[\"{\\\"action\\\":\\\"MODIFY\\\",\\\"price\\\":\\\"10.03\\\",\\\"quantity\\\":200}\"]}]");
        }

        TEST_F(MatchingEngineImplTest, ProcessMarketOrders)
        {
            MatchingEngineImpl engine(rule_, /*lot_size=*/10);
            const json data8 = {{"time", 10},
                                {"type", "NEW"},
                                {"order_id", 8},
                                {"symbol", "AAPL"},
                                {"side", "BUY"},
                                {
                                    "quantity",
                                    500,
                                },
                                {"order_type", "MARKET"}};
            std::unique_ptr<Order> order8 = std::make_unique<Order>(data8);
            const auto &status1 = engine.Process(std::move(order8));
            ASSERT_TRUE(status1.ok());
            EXPECT_EQ(status1->to_str(), "[{\"type\":\"DEPTH_UPDATE\"},{\"bid\":null},{\"ask\":null}]");

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
            const auto &status2 = engine.Process(std::move(order9));
            ASSERT_TRUE(status2.ok());
            EXPECT_EQ(status2->to_str(), "[{\"type\":\"DEPTH_UPDATE\"},{\"bid\":[\"{\\\"action\\\":\\\"ADD\\\",\\\"price\\\":\\\"9.99\\\",\\\"quantity\\\":500}\"]},{\"ask\":null}]");

            const json data5 = {{"time", 10},
                                {"type", "NEW"},
                                {"order_id", 5},
                                {"symbol", "AAPL"},
                                {"side", "SELL"},
                                {
                                    "quantity",
                                    200,
                                },
                                {"order_type", "MARKET"}};
            std::unique_ptr<Order> order5 = std::make_unique<Order>(data5);
            const auto &status3 = engine.Process(std::move(order5));
            ASSERT_TRUE(status3.ok());
            EXPECT_EQ(status3->to_str(), "{\"price\":\"9.99\",\"quantity\":200,\"type\":\"TRADE\"}/n[{\"type\":\"DEPTH_UPDATE\"},{\"bid\":[\"{\\\"action\\\":\\\"MODIFY\\\",\\\"price\\\":\\\"9.99\\\",\\\"quantity\\\":300}\"]},{\"ask\":null}]");

            const json data10 = {{"time", 10},
                                 {"type", "NEW"},
                                 {"order_id", 10},
                                 {"symbol", "AAPL"},
                                 {"side", "SELL"},
                                 {
                                     "quantity",
                                     1000,
                                 },
                                 {"order_type", "MARKET"}};
            std::unique_ptr<Order> order10 = std::make_unique<Order>(data10);
            const auto &status4 = engine.Process(std::move(order10));
            ASSERT_TRUE(status4.ok());
            EXPECT_EQ(status4->to_str(), "{\"price\":\"9.99\",\"quantity\":300,\"type\":\"TRADE\"}/n[{\"type\":\"DEPTH_UPDATE\"},{\"bid\":[\"{\\\"action\\\":\\\"DELETE\\\",\\\"price\\\":\\\"9.99\\\",\\\"quantity\\\":0}\"]},{\"ask\":null}]");

            const json data1 = {{"time", 9},
                                {"type", "NEW"},
                                {"order_id", 1},
                                {"symbol", "AAPL"},
                                {"side", "BUY"},
                                {
                                    "quantity",
                                    100,
                                },
                                {"order_type", "MARKET"}};
            std::unique_ptr<Order> order1 = std::make_unique<Order>(data1);
            const auto &status5 = engine.Process(std::move(order1));
            ASSERT_TRUE(status5.ok());
            EXPECT_EQ(status5->to_str(), "[{\"type\":\"DEPTH_UPDATE\"},{\"bid\":null},{\"ask\":null}]");
        }
    }
} // namespace fep::srcs::matching_engine