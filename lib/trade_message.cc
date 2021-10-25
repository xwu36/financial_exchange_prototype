#include "lib/trade_message.h"

#include "absl/strings/str_join.h"
#include "nlohmann/json.hpp"

namespace fep::lib
{

  using ::nlohmann::json;
  
  std::string TradeResult::to_str() const
  {
    json jobject;
    jobject["type"] = this->kType;
    jobject["price"] = this->price_.to_str();
    jobject["quantity"] = this->quantity_;

    return jobject.dump();
  }

  std::string TradeAction::to_str() const{
    json jobject;
    jobject["action"] = this->action_;
    jobject["price"] = this->price_.to_str();
    jobject["quantity"] = this->quantity_;

    return jobject.dump();
  }

  std::string TradeUpdate::to_str() const{
    json joutput;
    json jtype;
    jtype["type"] = this->kType;
    
    json jbid;
    for(const auto& bid : this->bids) {
      jbid["bid"].push_back(bid.to_str());
    }

    json jask;
    for(const auto& ask : this->asks) {
      jbid["ask"].push_back(ask.to_str());
    }
    joutput.push_back(jtype);
    joutput.push_back(jbid);
    joutput.push_back(jask);

    return joutput.dump();
  }

  std::string TradeMessage::to_str() const {
    std::vector<std::string> trade_results;
    for(const auto& result : this->trade_results) {
      trade_results.push_back(result.to_str());
    }
    std::string results = absl::StrJoin(trade_results.begin(), trade_results.end(), "/n");
    return absl::StrCat(results, "/n", this->trade_update.to_str());
  }

} // namespace fep::lib