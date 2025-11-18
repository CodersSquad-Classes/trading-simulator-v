#include "book.h"
#include <algorithm>

OrderBook::OrderBook(const std::string& symbol)
    : symbol(symbol)
{}

const std::string& OrderBook::getSymbol() const {
    return symbol;
}

const std::priority_queue<Order*, std::vector<Order*>, BuyComparator>&
OrderBook::getBids() const {
    return bids;
}

const std::priority_queue<Order*, std::vector<Order*>, SellComparator>&
OrderBook::getAsks() const {
    return asks;
}

void OrderBook::addOrder(
    Order* order,
    std::function<void(const Order*, const Order*, int, double)> onTrade)
{
    if (order->getSide() == Side::BUY) {
        matchBuy(order, onTrade);
    } else {
        matchSell(order, onTrade);
    }
}

void OrderBook::matchBuy(
    Order* incoming,
    std::function<void(const Order*, const Order*, int, double)> onTrade)
{
    while (incoming->getQuantity() > 0 && !asks.empty()) {
        Order* bestAsk = asks.top();

        if (incoming->getPrice() < bestAsk->getPrice()) {
            break;
        }

        asks.pop();
        int tradedQty = std::min(incoming->getQuantity(), bestAsk->getQuantity());
        double tradePrice = bestAsk->getPrice();

        onTrade(incoming, bestAsk, tradedQty, tradePrice);

        incoming->setQuantity(incoming->getQuantity() - tradedQty);
        bestAsk->setQuantity(bestAsk->getQuantity() - tradedQty);

        if (bestAsk->getQuantity() > 0) {
            asks.push(bestAsk);
        } else {
            delete bestAsk;
        }
    }

    if (incoming->getQuantity() > 0) {
        bids.push(incoming);
    } else {
        delete incoming;
    }
}

void OrderBook::matchSell(
    Order* incoming,
    std::function<void(const Order*, const Order*, int, double)> onTrade)
{
    while (incoming->getQuantity() > 0 && !bids.empty()) {
        Order* bestBid = bids.top();

        if (incoming->getPrice() > bestBid->getPrice()) {
            break;
        }

        bids.pop();
        int tradedQty = std::min(incoming->getQuantity(), bestBid->getQuantity());
        double tradePrice = bestBid->getPrice();

        onTrade(bestBid, incoming, tradedQty, tradePrice);

        incoming->setQuantity(incoming->getQuantity() - tradedQty);
        bestBid->setQuantity(bestBid->getQuantity() - tradedQty);

        if (bestBid->getQuantity() > 0) {
            bids.push(bestBid);
        } else {
            delete bestBid;
        }
    }

    if (incoming->getQuantity() > 0) {
        asks.push(incoming);
    } else {
        delete incoming;
    }
}
