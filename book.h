#ifndef BOOK_H
#define BOOK_H

#include <queue>
#include <vector>
#include <string>
#include <functional>
#include "order.h"

struct BuyComparator {
    bool operator()(const Order* a, const Order* b) const {
        if (a->getPrice() == b->getPrice()) {
            return a->getTimestamp() > b->getTimestamp();
        }
        return a->getPrice() < b->getPrice();
    }
};

struct SellComparator {
    bool operator()(const Order* a, const Order* b) const {
        if (a->getPrice() == b->getPrice()) {
            return a->getTimestamp() > b->getTimestamp();
        }
        return a->getPrice() > b->getPrice();
    }
};

class OrderBook {
public:
    explicit OrderBook(const std::string& symbol);

    const std::string& getSymbol() const;

    void addOrder(Order* order,
                  std::function<void(const Order*, const Order*, int, double)> onTrade);

    const std::priority_queue<Order*, std::vector<Order*>, BuyComparator>& getBids() const;
    const std::priority_queue<Order*, std::vector<Order*>, SellComparator>& getAsks() const;

private:
    std::string symbol;

    std::priority_queue<Order*, std::vector<Order*>, BuyComparator> bids;
    std::priority_queue<Order*, std::vector<Order*>, SellComparator> asks;

    void matchBuy(Order* incoming,
                  std::function<void(const Order*, const Order*, int, double)> onTrade);
    void matchSell(Order* incoming,
                   std::function<void(const Order*, const Order*, int, double)> onTrade);
};

#endif // BOOK_H
