#include "order_book.hpp"
#include <iostream>
#include <iomanip>



bool OrderBook::is_empty() const
{
    return _asks.empty() && _bids.empty();
}

void OrderBook::set_bid(double price, double amount)
{
    if (amount) {
        _bids[price] = amount;
    }
    else {
        _bids.erase(price);
    }
    if (_depth) {
        fix_bid_depth();
    }
}

void OrderBook::set_ask(double price, double amount)
{
    if (amount) {
        _asks[price] = amount;
    }
    else {
        _asks.erase(price);
    }
    if (_depth) {
        fix_ask_depth();
    }
}

void OrderBook::set_update_ts(uint64_t update_ts)
{
    _update_ts = update_ts;
}

void OrderBook::set_depth(uint64_t depth)
{
    if ((_depth = depth)) {
        fix_ask_depth();
        fix_bid_depth();
    }
}

void OrderBook::fix_ask_depth()
{
    while (_asks.size() > _depth) {
        _asks.erase(std::prev(_asks.end()));
    }
}

void OrderBook::fix_bid_depth()
{
    while (_bids.size() > _depth) {
        _bids.erase(_bids.begin());
    }
}

void OrderBook::set_reverse(bool is_reverse)
{
    _is_reverse = is_reverse;
}

std::ostream &operator<<(std::ostream &os, const OrderBook &book)
{
    auto print_map = [&](char name, auto it, auto end) {
        os << "\"" << name << "\":[";
        while (it != end) {
            os << '[' << it->first << ',' << it->second << ']';
            if (++it != end) {
                os << ',';
            }
        }
        os << ']';
    };
    os << std::setprecision(8) << std::fixed << '{';
    if (book._is_reverse) {
        print_map('b', book._bids.begin(), book._bids.end());
        os << ',';
        print_map('a', book._asks.begin(), book._asks.end());
    }
    else {
        print_map('a', book._asks.rbegin(), book._asks.rend());
        os << ',';
        print_map('b', book._bids.rbegin(), book._bids.rend());
    }
    os << '}';
    return os;
}
