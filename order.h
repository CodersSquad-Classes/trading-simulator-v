#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <cstdint>

enum class Side {
    BUY,
    SELL
};

class Order {
public:
    Order(std::uint64_t id,
          const std::string& symbol,
          Side side,
          double price,
          int quantity,
          std::uint64_t timestamp);

    std::uint64_t getId() const;
    const std::string& getSymbol() const;
    Side getSide() const;
    double getPrice() const;
    int getQuantity() const;
    std::uint64_t getTimestamp() const;

    void setQuantity(int q);

private:
    std::uint64_t id;
    std::string symbol;
    Side side;
    double price;
    int quantity;
    std::uint64_t timestamp;
};

#endif // ORDER_H
