#ifndef ENGINE_H
#define ENGINE_H

#include <map>
#include <string>
#include <vector>
#include <cstdint>
#include "book.h"

struct Trade {
    std::string symbol;
    std::uint64_t buyId;
    std::uint64_t sellId;
    int quantity;
    double price;
};

class MatchingEngine {
public:
    MatchingEngine();

    Order* createOrder(const std::string& symbol,
                       Side side,
                       double price,
                       int quantity);

    void submitOrder(Order* order);

    const OrderBook& getOrderBook(const std::string& symbol);
    const std::vector<Trade>& getRecentTrades() const;

    double getLastMatchPrice() const;
    std::uint64_t getLastMatchTick() const;
    std::uint64_t getTotalOrdersSubmitted() const;
    long long getTotalExecutedVolume() const;

private:
    std::map<std::string, OrderBook> books;
    std::vector<Trade> recentTrades;
    std::uint64_t nextOrderId;
    std::uint64_t timeCounter;

    double lastMatchPrice;
    std::uint64_t lastMatchTick;
    std::uint64_t totalOrdersSubmitted;
    long long totalExecutedVolume;

    OrderBook& getOrCreateBook(const std::string& symbol);

    void onTrade(const std::string& symbol,
                 const Order* buy,
                 const Order* sell,
                 int quantity,
                 double price);
};

#endif // ENGINE_H
