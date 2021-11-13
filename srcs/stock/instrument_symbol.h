#ifndef SRCS_STOCK_INSTRUMENT_SYMBOL_H_
#define SRCS_STOCK_INSTRUMENT_SYMBOL_H_

#include <unordered_map>

namespace fep::srcs::stock
{

  constexpr char kUnknown[] = "UNKNOWN";
  constexpr char kAAPL[] = "AAPL";
  constexpr char kGOOG[] = "GOOG";
  constexpr char kTSLA[] = "TSLA";

  enum class Symbol
  {
    UNKNOWN,
    AAPL,
    GOOG,
    TSLA
  };

  const std::unordered_map<std::string, Symbol> SymbolStringToEnum{
      {kUnknown, Symbol::UNKNOWN},
      {kAAPL, Symbol::AAPL},
      {kGOOG, Symbol::GOOG},
      {kTSLA, Symbol::TSLA}};

  const std::unordered_map<Symbol, std::string> SymbolEnumToString{
      {Symbol::UNKNOWN, kUnknown},
      {Symbol::AAPL, kAAPL},
      {Symbol::GOOG, kGOOG},
      {Symbol::TSLA, kTSLA}};

} // namespace fep::srcs::order

#endif