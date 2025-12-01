#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <random>
#include <thread>
#include <chrono>

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

void showDashboard(MatchingEngine& engine, const std::string& symbol) {
    showBook(engine, symbol);

    std::cout << Color::YELLOW << Color::BOLD
              << "RECENT TRADES"
              << Color::RESET << "\n";

    const auto& trades = engine.getRecentTrades();
    if (trades.empty()) {
        std::cout << "(no trades yet)\n\n";
    } else {
        std::cout << std::left
                  << std::setw(6)  << "BUY"
                  << std::setw(6)  << "SELL"
                  << std::setw(10) << "PRICE"
                  << std::setw(10) << "QTY"
                  << "\n";

        for (const auto& t : trades) {
            std::cout << std::setw(6)  << t.buyId
                      << std::setw(6)  << t.sellId
                      << std::setw(10) << std::fixed << std::setprecision(2) << t.price
                      << std::setw(10) << t.quantity
                      << "\n";
        }
        std::cout << "\n";
    }

    std::cout << Color::BOLD << "STATS" << Color::RESET << "\n";
    std::cout << "Last match price:      ";
    if (engine.getLastMatchTick() == 0) {
        std::cout << "n/a\n";
    } else {
        std::cout << std::fixed << std::setprecision(2)
                  << engine.getLastMatchPrice() << "\n";
    }
    std::cout << "Total orders submitted: " << engine.getTotalOrdersSubmitted() << "\n";
    std::cout << "Total executed volume:  " << engine.getTotalExecutedVolume() << "\n\n";
}

void generateRandomOrders(MatchingEngine& engine,
                          const std::string& symbol,
                          std::mt19937& rng)
{
    std::uniform_int_distribution<int> countDist(0, 3);
    int n = countDist(rng);

    std::bernoulli_distribution sideDist(0.5);
    std::uniform_real_distribution<double> priceDist(95.0, 105.0);
    std::uniform_int_distribution<int> qtyDist(1, 100);

    for (int i = 0; i < n; ++i) {
        Side side = sideDist(rng) ? Side::BUY : Side::SELL;
        double price = priceDist(rng);
        int qty = qtyDist(rng);

        Order* o = engine.createOrder(symbol, side, price, qty);
        engine.submitOrder(o);
    }
}

int main() {
    MatchingEngine engine;
    const std::string symbol = "ABC";

    std::random_device rd;
    std::mt19937 rng(rd());

    while (true) {
        generateRandomOrders(engine, symbol, rng);

        clearScreen();
        showDashboard(engine, symbol);

        std::cout << "Press Ctrl+C to stop...\n";

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    return 0;
}
