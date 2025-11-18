#include "engine.h"

MatchingEngine::MatchingEngine()
    : nextOrderId(1),
      timeCounter(1),
      lastMatchPrice(0.0),
      lastMatchTick(0),
      totalOrdersSubmitted(0),
      totalExecutedVolume(0)
{}

OrderBook& MatchingEngine::getOrCreateBook(const std::string& symbol) {
    auto it = books.find(symbol);
    if (it == books.end()) {
        it = books.emplace(symbol, OrderBook(symbol)).first;
    }
    return it->second;
}

Order* MatchingEngine::createOrder(const std::string& symbol,
                                   Side side,
                                   double price,
                                   int quantity)
{
    std::uint64_t id  = nextOrderId++;
    std::uint64_t ts  = timeCounter++;
    totalOrdersSubmitted++;
    return new Order(id, symbol, side, price, quantity, ts);
}

void MatchingEngine::submitOrder(Order* order) {
    OrderBook& book = getOrCreateBook(order->getSymbol());
    std::string sym = order->getSymbol();
    book.addOrder(order,
                  [this, sym](const Order* buy, const Order* sell, int q, double p) {
                      onTrade(sym, buy, sell, q, p);
                  });
}

const OrderBook& MatchingEngine::getOrderBook(const std::string& symbol) {
    return getOrCreateBook(symbol);
}

const std::vector<Trade>& MatchingEngine::getRecentTrades() const {
    return recentTrades;
}

void MatchingEngine::onTrade(const std::string& symbol,
                             const Order* buy,
                             const Order* sell,
                             int quantity,
                             double price)
{
    Trade t;
    t.symbol   = symbol;
    t.buyId    = buy->getId();
    t.sellId   = sell->getId();
    t.quantity = quantity;
    t.price    = price;

    recentTrades.push_back(t);
    const std::size_t MAX_TRADES = 10;
    if (recentTrades.size() > MAX_TRADES) {
        recentTrades.erase(recentTrades.begin());
    }

    lastMatchPrice = price;
    lastMatchTick  = timeCounter;
    totalExecutedVolume += quantity;
}

double MatchingEngine::getLastMatchPrice() const {
    return lastMatchPrice;
}

std::uint64_t MatchingEngine::getLastMatchTick() const {
    return lastMatchTick;
}

std::uint64_t MatchingEngine::getTotalOrdersSubmitted() const {
    return totalOrdersSubmitted;
}

long long MatchingEngine::getTotalExecutedVolume() const {
    return totalExecutedVolume;
}
