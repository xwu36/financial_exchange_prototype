#include <iostream>

#include "srcs/market_data_publisher/market_data_publisher_factory.h"
#include "srcs/matching_engine/matching_engine_factory.h"

int main()
{
  const fep::srcs::matching_engine::MatchingEngineFactory matching_engine_factory;
  const fep::srcs::market_data_publisher::MarketDataPublisherFactory market_data_publiser_factory;

  const fep::srcs::matching_engine::MatchingEngineImpl matching_engine = matching_engine_factory.ProduceMatchingEngine();
  const fep::srcs::market_data_publisher::MarketDataPublisherImpl market_data_publiser = market_data_publiser_factory.ProduceMarketDataPublisher();
}