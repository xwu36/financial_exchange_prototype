#include <iostream>
#include <fstream>
#include <vector>

#include "absl/flags/flag.h"
#include "external/com_google_absl/absl/status/statusor.h"
#include "glog/logging.h"
#include "glog/stl_logging.h"
#include "lib/tick_size_rule.h"
#include "lib/trade_message.h"
#include "nlohmann/json.hpp"
#include "srcs/order/order.h"
#include "srcs/order/order_utils.h"
#include "srcs/market_data_publisher/market_data_publisher_factory.h"
#include "srcs/matching_engine/matching_engine_factory.h"

using ::fep::lib::TickSizeRule;
using ::fep::lib::TradeMessage;
using ::fep::srcs::market_data_publisher::MarketDataPublisherFactory;
using ::fep::srcs::market_data_publisher::MarketDataPublisherImpl;
using ::fep::srcs::matching_engine::MatchingEngineFactory;
using ::fep::srcs::matching_engine::MatchingEngineImpl;
using ::fep::srcs::order::Order;
using ::nlohmann::json;

ABSL_FLAG(uint32_t, lot_size, 100, "A lot size which shares must be a multiple of ");

int main(int argc, char *argv[])
{
  // google::InitGoogleLogging(argv[0]);

  // Read tick_size rule from a file.
  std::ifstream tick_size_f("srcs/main/data/tick_size_rule.json");
  json tick_size_j;
  tick_size_f >> tick_size_j;
  TickSizeRule tick_size_rule;
  tick_size_rule.FromJson(tick_size_j);

  const MatchingEngineFactory matching_engine_factory(tick_size_rule, absl::GetFlag(FLAGS_lot_size));
  const MarketDataPublisherFactory market_data_publiser_factory;

  MatchingEngineImpl matching_engine = matching_engine_factory.ProduceMatchingEngine();
  const MarketDataPublisherImpl market_data_publiser = market_data_publiser_factory.ProduceMarketDataPublisher();

  std::vector<Order> orders = fep::srcs::order::ReadOrdersFromPath("srcs/main/data/orders.jsonl");

  for (const auto &order : orders)
  {
    absl::StatusOr<TradeMessage> message = matching_engine.Process(std::make_unique<Order>(order));
    if (message.ok())
    {
      market_data_publiser.Publish(message.value());
    }
    else
    {
      LOG(ERROR) << message.value().to_str() << std::endl;
    }
  }
}