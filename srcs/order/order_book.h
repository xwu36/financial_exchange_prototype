#ifndef SRCS_ORDER_ORDER_BOOK_H_
#define SRCS_ORDER_ORDER_BOOK_H_

#include <set>

#include "srcs/order/order_book_entry.h"

namespace fep::srcs::order
{
  struct BidComparator
  {
    bool operator()(const OrderBookEntry &lhs, const OrderBookEntry &rhs)
        const
    {
      if (lhs.price == rhs.price)
      {
        if (lhs.timestamp_sec == rhs.timestamp_sec)
        {
          return lhs.order_id < rhs.order_id;
        }
        return lhs.timestamp_sec < rhs.timestamp_sec;
      }
      return lhs.price > rhs.price;
    }
  };

  struct AskComparator
  {
    bool operator()(const OrderBookEntry &lhs, const OrderBookEntry &rhs)
        const
    {
      if (lhs.price == rhs.price)
      {
        if (lhs.timestamp_sec == rhs.timestamp_sec)
        {
          return lhs.order_id < rhs.order_id;
        }
        return lhs.timestamp_sec < rhs.timestamp_sec;
      }
      return lhs.price < rhs.price;
    }
  };

  template <class T>
  class OrderBook
  {
  public:
    const bool Empty()
    {
      return order_book_entries_.empty();
    }

    const size_t Size()
    {
      return order_book_entries_.size();
    }

    // Returns the first entry. Must be called after checking if the book is empty.
    const OrderBookEntry &FirstEntry()
    {
      return *order_book_entries_.begin();
    }

    // Removes the first entry.
    void RemoveFirstEntry()
    {
      order_book_entries_.erase(order_book_entries_.begin());
    }

    // Removes the given entry in the book.
    void RemoveEntry(const OrderBookEntry &order_entry)
    {
      order_book_entries_.erase(order_entry);
    }

    // Adds an order to book.
    void AddEntry(const OrderBookEntry &order_entry)
    {
      order_book_entries_.emplace(order_entry);
    }

  private:
    std::set<OrderBookEntry, T> order_book_entries_;
  };

} // namespace fep::srcs::order

#endif