#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

#include "engine.h"
#include "colors.h"

void clearScreen() {
    std::cout << "\033[2J\033[H";
}

template<typename PQ>
std::vector<Order*> toVector(PQ pq) {
    std::vector<Order*> v;
    while (!pq.empty()) {
        v.push_back(pq.top());
        pq.pop();
    }
    return v;
}

void showBook(MatchingEngine& engine, const std::string& symbol) {
    const OrderBook& book = engine.getOrderBook(symbol);

    auto bids = toVector(book.getBids());
    auto asks = toVector(book.getAsks());

    std::cout << Color::CYAN << Color::BOLD
              << "Order book for " << symbol
              << Color::RESET << "\n\n";

    std::cout << Color::GREEN << "BUY ORDERS" << Color::RESET << "\n";
    std::cout << std::left
              << std::setw(6)  << "ID"
              << std::setw(10) << "PRICE"
              << std::setw(10) << "QTY"
              << "\n";

    if (bids.empty()) {
        std::cout << "(no buy orders)\n";
    } else {
        for (Order* o : bids) {
            std::cout << std::setw(6)  << o->getId()
                      << std::setw(10) << std::fixed << std::setprecision(2) << o->getPrice()
                      << std::setw(10) << o->getQuantity()
                      << "\n";
        }
    }

    std::cout << "\n";

    std::cout << Color::RED << "SELL ORDERS" << Color::RESET << "\n";
    std::cout << std::left
              << std::setw(6)  << "ID"
              << std::setw(10) << "PRICE"
              << std::setw(10) << "QTY"
              << "\n";

    if (asks.empty()) {
        std::cout << "(no sell orders)\n";
    } else {
        for (Order* o : asks) {
            std::cout << std::setw(6)  << o->getId()
                      << std::setw(10) << std::fixed << std::setprecision(2) << o->getPrice()
                      << std::setw(10) << o->getQuantity()
                      << "\n";
        }
    }

    std::cout << "\n";
}

int main() {
    MatchingEngine engine;
    const std::string symbol = "ABC";

    while (true) {
        clearScreen();
        showBook(engine, symbol);

        std::cout << Color::BOLD
                  << "New order (B = buy, S = sell, Q = quit)\n"
                  << Color::RESET;
        std::cout << "Side (B/S/Q): ";

        char sideChar;
        std::cin >> sideChar;
        if (!std::cin) {
            break;
        }

        if (sideChar == 'Q' || sideChar == 'q') {
            break;
        }

        Side side;
        if (sideChar == 'B' || sideChar == 'b') {
            side = Side::BUY;
        } else if (sideChar == 'S' || sideChar == 's') {
            side = Side::SELL;
        } else {
            continue;
        }

        double price;
        int qty;

        std::cout << "Price: ";
        std::cin >> price;
        std::cout << "Quantity: ";
        std::cin >> qty;

        if (!std::cin || qty <= 0 || price <= 0.0) {
            std::cout << "Invalid input. Exiting.\n";
            break;
        }

        Order* o = engine.createOrder(symbol, side, price, qty);
        engine.submitOrder(o);
    }

    std::cout << "Exiting...\n";
    return 0;
}
