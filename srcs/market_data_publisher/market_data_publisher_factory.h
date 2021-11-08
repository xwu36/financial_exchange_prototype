#ifndef SRCS_MARKET_DATA_PUBLISHER_MARKET_DATA_PUBLISHER_FACTORY_H_
#define SRCS_MARKET_DATA_PUBLISHER_MARKET_DATA_PUBLISHER_FACTORY_H_

#include "srcs/market_data_publisher/market_data_publisher_impl.h"

namespace fep::srcs::market_data_publisher
{

  class MarketDataPublisherFactory
  {
  public:
    MarketDataPublisherFactory() = default;
    fep::srcs::market_data_publisher::MarketDataPublisherImpl ProduceMarketDataPublisher() const
    {
      return fep::srcs::market_data_publisher::MarketDataPublisherImpl();
    }
  };

} // namespace fep::srcs::market_data_publisher
#endif