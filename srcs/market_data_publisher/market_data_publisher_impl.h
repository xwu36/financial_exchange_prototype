#ifndef SRCS_MARKET_DATA_PUBLISHER_MARKET_DATA_PUBLISHER_IMPL_H_
#define SRCS_MARKET_DATA_PUBLISHER_MARKET_DATA_PUBLISHER_IMPL_H_

#include "glog/logging.h"
#include "srcs/market_data_publisher/abstract/market_data_publisher.h"

namespace fep::srcs::market_data_publisher
{

  class MarketDataPublisherImpl : public MarketDataPublisher
  {
  public:
    MarketDataPublisherImpl() = default;

    void Publish(const fep::lib::TradeMessage &message) const override
    {
      LOG(INFO) << message.to_str() << std::endl;
    }
  };

} // namespace fep::srcs::
#endif