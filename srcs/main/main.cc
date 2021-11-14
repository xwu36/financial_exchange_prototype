#include <condition_variable>
#include <chrono>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include "absl/flags/flag.h"
#include "absl/time/time.h"
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
using ::fep::srcs::market_data_publisher::MarketDataPublisher;
using ::fep::srcs::market_data_publisher::MarketDataPublisherFactory;
using ::fep::srcs::market_data_publisher::MarketDataPublisherImpl;
using ::fep::srcs::matching_engine::MatchingEngine;
using ::fep::srcs::matching_engine::MatchingEngineFactory;
using ::fep::srcs::matching_engine::MatchingEngineImpl;
using ::fep::srcs::order::Order;
using ::nlohmann::json;

constexpr char kGtcOrdersFileName[] = "unmatched_gtc_orders.jsonl";

ABSL_FLAG(std::string, order_file_path_prefix, "srcs/main/data/", "Directory which holds the order files");
ABSL_FLAG(std::string, order_file_debug_path, "srcs/main/data/orders.jsonl",
          "Input order path for debugging purpose");

std::mutex g_mutex;
std::condition_variable g_cv;
bool g_is_market_on = false;
int g_data = 0;

void MarketStatusChecker()
{
  absl::TimeZone utc = absl::UTCTimeZone();
  while (true)
  {
    std::this_thread::sleep_for(std::chrono::seconds(60));

    const std::string now_str = absl::FormatTime("%H:%M", absl::Now(), utc);
    LOG(INFO) << "Checking market status at " << now_str << "...";

    std::unique_lock<std::mutex> ul(g_mutex);

    g_is_market_on = (now_str == "09:00");

    ul.unlock();

    if (!g_is_market_on)
    {
      continue;
    }
    LOG(INFO) << "Market is open!";

    g_cv.notify_one();
    ul.lock();
    g_cv.wait(ul, []()
              { return g_is_market_on == false; });
  }
}

void RunMatchingEngine(MatchingEngine &matching_engine, const MarketDataPublisher &market_data_publiser)
{
  absl::TimeZone utc = absl::UTCTimeZone();
  while (true)
  {
    LOG(INFO) << "Start processing offer for today...";
    std::unique_lock<std::mutex> ul(g_mutex);

    g_cv.wait(ul, []()
              { return g_is_market_on == true; });

    g_is_market_on = false;

    ul.unlock();

    g_cv.notify_one();
    ul.lock();

    const std::string now_str = absl::FormatTime("%Y-%m-%d/", absl::Now(), utc);
    const std::string one_day_ago_str = absl::FormatTime("%Y-%m-%d/", absl::Now() - absl::Hours(24), utc);
    const std::string order_file_path_prefix = absl::GetFlag(FLAGS_order_file_path_prefix);
    const std::string orders_to_process_today = absl::StrCat(order_file_path_prefix, now_str, "orders.jsonl");
    const std::string gtc_orders_today = absl::StrCat(
        order_file_path_prefix, now_str, kGtcOrdersFileName);
    const std::string gtc_orders_a_day_ago = absl::StrCat(
        order_file_path_prefix, one_day_ago_str, kGtcOrdersFileName);
    LOG(INFO) << "The path of all offers to be processed today: " << orders_to_process_today;
    LOG(INFO) << "The path of unmatched GTC offers for today: " << gtc_orders_today;
    LOG(INFO) << "The path of unmatched GTC offers from yesterday: " << gtc_orders_a_day_ago;

    // Read yesterday's unmatched GTC orders.
    absl::Status init_stats = matching_engine.InitOnMarketStarts(gtc_orders_a_day_ago);
    if (!init_stats.ok())
    {
      LOG(ERROR) << init_stats.message();
    }

    // Process today's orders.
    std::vector<Order> orders = fep::srcs::order::ReadOrdersFromPath(absl::GetFlag(FLAGS_order_file_debug_path));
    // Loop through all the offers and process each of them.
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

      // Clear the matching engine and output today's unmatched GTC orders.
      absl::Status clear_status = matching_engine.ClearOnMarketEnds(gtc_orders_today);
    }

    LOG(INFO) << "Market is closed!";
  }
}

int main(int argc, char *argv[])
{
  // google::InitGoogleLogging(argv[0]);

  // Create a matching_engine object.
  const MatchingEngineFactory matching_engine_factory;
  MatchingEngineImpl matching_engine = matching_engine_factory.ProduceMatchingEngine();

  // Create a market data publiser.
  const MarketDataPublisherFactory market_data_publiser_factory;
  const MarketDataPublisherImpl market_data_publiser = market_data_publiser_factory.ProduceMarketDataPublisher();

  std::thread market_status_thread(MarketStatusChecker);
  std::thread run_matching_engine_thread(RunMatchingEngine, std::ref(matching_engine), std::ref(market_data_publiser));

  market_status_thread.join();
  run_matching_engine_thread.join();
}