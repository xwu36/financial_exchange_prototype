#ifndef LIB_TRADE_MESSAGE_H_
#define LIB_TRADE_MESSAGE_H_

#include <string>
#include <vector>

#include "lib/price4.h"

namespace fep::lib

{

  static constexpr char kAddAction[] = "ADD";
  static constexpr char kDeleteAction[] = "DELETE";
  static constexpr char kModifyAction[] = "MODIFY";

  class TradeResult
  {
  public:
    TradeResult(const Price4 &price, const uint64_t quantity) : price_(price),
                                                                quantity_(quantity) {}

    std::string to_str() const;

  protected:
    static constexpr char kType[] = "TRADE";
    Price4 price_;
    uint64_t quantity_ = 0;
  };

  class TradeAction
  {
  public:
    TradeAction(const Price4 &price, uint64_t quantity, const std::string &action) : price_(price),
                                                                                     quantity_(quantity),
                                                                                     action_(action) {}
    std::string to_str() const;

  protected:
    Price4 price_;
    uint64_t quantity_ = 0;
    std::string action_ = "UNKNOWN";
  };

  struct TradeUpdate
  {
    static constexpr char kType[] = "DEPTH_UPDATE";
    std::vector<TradeAction> bids;
    std::vector<TradeAction> asks;

    std::string to_str() const;
  };

  struct TradeMessage
  {
    std::vector<TradeResult> trade_results;
    TradeUpdate trade_update;

    std::string to_str() const;
  };

} // namespace fep::lib

#endif