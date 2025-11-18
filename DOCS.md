## Introduction

The homework imitates the basic behavior of a Continuous Limit Order Book (CLOB), like the ones used in electronic stock exchanges.

The idea is simple: there is one stock symbol (I use `ABC`) and the user can place buy and sell orders. Each order has a side (BUY or SELL), a price, a quantity, and an internal time of arrival. When a new order enters the system, the matching engine checks if it can be matched with an order on the opposite side. If the prices are compatible, a trade happens; if not, the new order is added to the book and waits.

The state of the book is shown in the terminal. Buy orders are printed in green and sell orders in red, so the user can see at a glance what is happening on each side of the market.

---

## Research on stocks exchange

A stock exchange is a marketplace where people and institutions buy and sell shares and other financial instruments. Traders send orders to the exchange, and the exchange must connect buyers and sellers in a way that is transparent, fair, and fast. This is usually done by a central matching engine.

In this context, a limit order is an instruction such as “buy 10 shares at up to 100.00” or “sell 5 shares at at least 102.00”. The trader controls the price, but the trade will only happen if someone is willing to take the other side at that price. This is different from a market order, which says “buy or sell now at the best available price.” For this project I chose to work only with limit orders, because they are easier to reason about and they map directly to the structure of a limit order book.

A Continuous Limit Order Book keeps two lists: bids (buy orders) and asks (sell orders). New orders arrive continuously and the book is updated order by order. Two priority rules are essential:

- Price priority: for buy orders, a higher price has priority; for sell orders, a lower price has priority.
- Time priority: if two orders have the same price, the one that arrived earlier goes first.

My simulator follows these rules using data structures from the C++ standard library.

---

## Software Design and Implementation

I organised the project into small, focused files:

- `order.h`, `order.cpp`: represent a single order.
- `book.h`, `book.cpp`: manage the order book for one symbol.
- `engine.h`, `engine.cpp`: implement the matching engine.
- `colors.h`: define simple terminal color codes.
- `main.cpp`: provide the interactive loop with the user.

Conceptually the design can be described like this:

- The **MatchingEngine** owns a map from symbol to **OrderBook**.
- Each **OrderBook** owns two priority queues of pointers to **Order** objects: one for bids and one for asks.
- The **Order** class is just a container for the data of one order.

The `Order` class stores: an id, a symbol, a side (BUY or SELL), a price, a quantity, and a timestamp that acts as arrival time. It exposes simple getter functions and a method to update the remaining quantity after a trade.

The `OrderBook` class is responsible for the actual matching logic. It keeps:

- a priority queue of buy orders, sorted so that higher price and then earlier time come first;
- a priority queue of sell orders, sorted so that lower price and then earlier time come first.

When a new order arrives, the book tries to match it against the opposite side. For a buy order, the algorithm is:

- While the incoming buy still has quantity and there are sell orders:
    - Look at the best sell (top of the ask queue).
    - If the buy price is lower than this ask price, stop; there is no trade.
    - Otherwise, trade a quantity equal to the minimum of both order sizes, at the ask price.
    - Reduce the quantities of both orders accordingly.
    - If the opposite order still has quantity left, push it back into the queue; if not, delete it.
- If the incoming buy still has remaining quantity after this, push it into the bid queue; otherwise, delete it.

The logic for an incoming sell order is symmetric, but working against the bid queue. Every time a trade happens, the `OrderBook` calls a callback function provided by the `MatchingEngine`. This callback receives the buy order, the sell order, the traded quantity, and the trade price.

The `MatchingEngine` manages all books and keeps track of statistics and trade history. It uses a map from `std::string` to `OrderBook` so that, in principle, multiple symbols could be supported. It generates unique ids and timestamps for each new order, and exposes methods like:
