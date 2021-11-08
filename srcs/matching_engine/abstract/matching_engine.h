#ifndef SRCS_MATCHING_ENGINE_ABSTRACT_MATCHING_ENGINE_H_
#define SRCS_MATCHING_ENGINE_ABSTRACT_MATCHING_ENGINE_H_

#include <unordered_map>
#include <vector>

#include "external/com_google_absl/absl/status/statusor.h"
#include "lib/tick_size_rule.h"
#include "lib/trade_message.h"
#include "srcs/order/order.h"
#include "srcs/order/order_book.h"
#include "srcs/order/order_pool.h"
#include "srcs/stock/instrument_symbol.h"

namespace fep::srcs::matching_engine
{

  class MatchingEngine
  {
  public:
    MatchingEngine(const fep::lib::TickSizeRule &tick_size_rule,
                   const uint32_t lot_size) : tick_size_rule_(tick_size_rule), lot_size_(lot_size) {}
    virtual ~MatchingEngine() = default;
    virtual absl::StatusOr<fep::lib::TradeMessage> Process(std::unique_ptr<fep::srcs::order::Order> order) = 0;

  protected:
    virtual absl::StatusOr<fep::lib::TradeMessage> Sell(std::unique_ptr<fep::srcs::order::Order> order) = 0;
    virtual absl::StatusOr<fep::lib::TradeMessage> Buy(std::unique_ptr<fep::srcs::order::Order> order) = 0;
    virtual absl::StatusOr<fep::lib::TradeMessage> Cancel(std::unique_ptr<fep::srcs::order::Order> order) = 0;

    fep::srcs::order::OrderPool bid_order_pool_;
    fep::srcs::order::OrderPool ask_order_pool_;
    std::unordered_map<fep::srcs::stock::Symbol, fep::srcs::order::OrderBook<fep::srcs::order::BidComparator>> bid_order_books_;
    std::unordered_map<fep::srcs::stock::Symbol, fep::srcs::order::OrderBook<fep::srcs::order::AskComparator>> ask_order_books_;
    const fep::lib::TickSizeRule tick_size_rule_;
    const uint32_t lot_size_;
  };

} // namespace fep::srcs::matching_engine
#endif