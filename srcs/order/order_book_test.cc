#include "srcs/order/order_book.h"

#include <set>

#include "gtest/gtest.h"
#include "lib/price4.h"

namespace fep::srcs::order
{

  namespace
  {

    using ::fep::lib::Price4;

    class OrderBookTest : public testing::Test
    {
    protected:
      OrderBookEntry entry1_{.timestamp_sec = 50, .order_id = 1, .price = Price4(1)};
      OrderBookEntry entry2_{.timestamp_sec = 40, .order_id = 2, .price = Price4(2)};
      OrderBookEntry entry3_{.timestamp_sec = 30, .order_id = 3, .price = Price4(3)};
      OrderBookEntry entry4_{.timestamp_sec = 21, .order_id = 4, .price = Price4(4)};
      OrderBookEntry entry5_{.timestamp_sec = 20, .order_id = 5, .price = Price4(4)};
      OrderBookEntry entry6_{.timestamp_sec = 10, .order_id = 6, .price = Price4(6)};
    };

    TEST_F(OrderBookTest, BidBook)
    {
      BidOrderBook bid_book;
      bid_book.AddEntry(entry1_);
      EXPECT_EQ(bid_book.Size(), 1);
      EXPECT_EQ(bid_book.FirstEntry().order_id, 1);
      bid_book.AddEntry(entry2_);
      EXPECT_EQ(bid_book.Size(), 2);
      EXPECT_EQ(bid_book.FirstEntry().order_id, 2);
      bid_book.AddEntry(entry3_);
      EXPECT_EQ(bid_book.Size(), 3);
      EXPECT_EQ(bid_book.FirstEntry().order_id, 3);
      bid_book.AddEntry(entry5_);
      EXPECT_EQ(bid_book.Size(), 4);
      EXPECT_EQ(bid_book.FirstEntry().order_id, 5);
      bid_book.AddEntry(entry4_);
      EXPECT_EQ(bid_book.Size(), 5);
      EXPECT_EQ(bid_book.FirstEntry().order_id, 5);
      bid_book.AddEntry(entry6_);
      EXPECT_EQ(bid_book.Size(), 6);
      EXPECT_EQ(bid_book.FirstEntry().order_id, 6);

      bid_book.RemoveFirstEntry();
      EXPECT_EQ(bid_book.Size(), 5);
      EXPECT_EQ(bid_book.FirstEntry().order_id, 5);
      bid_book.RemoveFirstEntry();
      EXPECT_EQ(bid_book.Size(), 4);
      EXPECT_EQ(bid_book.FirstEntry().order_id, 4);
      bid_book.RemoveEntry(entry4_);
      EXPECT_EQ(bid_book.Size(), 3);
      EXPECT_EQ(bid_book.FirstEntry().order_id, 3);
      OrderBookEntry entry7{.timestamp_sec = 10, .order_id = 3, .price = Price4(3)};
      bid_book.RemoveEntry(entry7);
      EXPECT_EQ(bid_book.Size(), 3);
      OrderBookEntry entry8{.timestamp_sec = 30, .order_id = 3, .price = Price4(3)};
      bid_book.RemoveEntry(entry8);
      EXPECT_EQ(bid_book.Size(), 2);
    }

    TEST_F(OrderBookTest, AskBook)
    {
      AskOrderBook ask_book;
      ask_book.AddEntry(entry1_);
      EXPECT_EQ(ask_book.Size(), 1);
      EXPECT_EQ(ask_book.FirstEntry().order_id, 1);
      ask_book.AddEntry(entry2_);
      EXPECT_EQ(ask_book.Size(), 2);
      EXPECT_EQ(ask_book.FirstEntry().order_id, 1);
      ask_book.AddEntry(entry3_);
      EXPECT_EQ(ask_book.Size(), 3);
      EXPECT_EQ(ask_book.FirstEntry().order_id, 1);
      ask_book.AddEntry(entry5_);
      EXPECT_EQ(ask_book.Size(), 4);
      EXPECT_EQ(ask_book.FirstEntry().order_id, 1);
      ask_book.AddEntry(entry4_);
      EXPECT_EQ(ask_book.Size(), 5);
      EXPECT_EQ(ask_book.FirstEntry().order_id, 1);
      ask_book.AddEntry(entry6_);
      EXPECT_EQ(ask_book.Size(), 6);
      EXPECT_EQ(ask_book.FirstEntry().order_id, 1);

      EXPECT_FALSE(ask_book.Empty());

      ask_book.RemoveFirstEntry();
      EXPECT_EQ(ask_book.Size(), 5);
      EXPECT_EQ(ask_book.FirstEntry().order_id, 2);
      ask_book.RemoveFirstEntry();
      EXPECT_EQ(ask_book.Size(), 4);
      EXPECT_EQ(ask_book.FirstEntry().order_id, 3);
      ask_book.RemoveFirstEntry();
      EXPECT_EQ(ask_book.Size(), 3);
      EXPECT_EQ(ask_book.FirstEntry().order_id, 5);
      ask_book.RemoveFirstEntry();
      EXPECT_EQ(ask_book.Size(), 2);
      EXPECT_EQ(ask_book.FirstEntry().order_id, 4);
      ask_book.RemoveFirstEntry();
      EXPECT_EQ(ask_book.Size(), 1);
      EXPECT_EQ(ask_book.FirstEntry().order_id, 6);
      OrderBookEntry entry7{.timestamp_sec = 10, .order_id = 5, .price = Price4(6)};
      ask_book.RemoveEntry(entry7);
      EXPECT_EQ(ask_book.Size(), 1);
      OrderBookEntry entry8{.timestamp_sec = 10, .order_id = 6, .price = Price4(6)};
      ask_book.RemoveEntry(entry8);
      EXPECT_EQ(ask_book.Size(), 0);
      EXPECT_TRUE(ask_book.Empty());
    }

    TEST_F(OrderBookTest, VisibleQueue)
    {
      OrderBookEntry entry1{.timestamp_sec = 40, .order_id = 2, .price = Price4(1), .visible = true};
      OrderBookEntry entry2{.timestamp_sec = 50, .order_id = 1, .price = Price4(1), .visible = false};
      AskOrderBook ask_book;
      ask_book.AddEntry(entry1);
      ask_book.AddEntry(entry2);
      EXPECT_EQ(ask_book.FirstEntry().order_id, 2);

      BidOrderBook bid_book;
      bid_book.AddEntry(entry1);
      bid_book.AddEntry(entry2);
      EXPECT_EQ(bid_book.FirstEntry().order_id, 2);
    }

  } // namespace
} // namespace fep::srcs::order