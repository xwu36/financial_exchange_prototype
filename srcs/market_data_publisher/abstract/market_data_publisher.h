#ifndef SRCS_MARKET_DATA_PUBLISHER_ABSTRACT_MARKET_DATA_PUBLISHER_H_
#define SRCS_MARKET_DATA_PUBLISHER_ABSTRACT_MARKET_DATA_PUBLISHER_H_

#include "lib/trade_message.h"

namespace fep::srcs::market_data_publisher
{

  class MarketDataPublisher
  {
  public:
    MarketDataPublisher() = default;
    virtual ~MarketDataPublisher() = default;
    virtual void Publish(const fep::lib::TradeMessage &message) const = 0;
  };

} // namespace fep::srcs::
#endif