#include "order.h"

Order::Order(std::uint64_t id,
             const std::string& symbol,
             Side side,
             double price,
             int quantity,
             std::uint64_t timestamp)
    : id(id),
      symbol(symbol),
      side(side),
      price(price),
      quantity(quantity),
      timestamp(timestamp)
{}

std::uint64_t Order::getId() const {
    return id;
}

const std::string& Order::getSymbol() const {
    return symbol;
}

Side Order::getSide() const {
    return side;
}

double Order::getPrice() const {
    return price;
}

int Order::getQuantity() const {
    return quantity;
}

std::uint64_t Order::getTimestamp() const {
    return timestamp;
}

void Order::setQuantity(int q) {
    quantity = q;
}
