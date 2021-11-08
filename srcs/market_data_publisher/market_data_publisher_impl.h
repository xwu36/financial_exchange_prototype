#ifndef SRCS_MARKET_DATA_PUBLISHER_MARKET_DATA_PUBLISHER_IMPL_H_
#define SRCS_MARKET_DATA_PUBLISHER_MARKET_DATA_PUBLISHER_IMPL_H_

#include <iostream>

#include "srcs/market_data_publisher/abstract/market_data_publisher.h"

namespace fep::srcs::market_data_publisher
{

  class MarketDataPublisherImpl : public MarketDataPublisher
  {
  public:
    MarketDataPublisherImpl() = default;

    void Publish(const fep::lib::TradeMessage &message) override
    {
      std::cout << message.to_str();
    }
  };

} // namespace fep::srcs::
#endif