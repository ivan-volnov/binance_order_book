#ifndef ORDER_BOOK_H
#define ORDER_BOOK_H

#include <map>



class OrderBook
{
public:
    friend std::ostream &operator<<(std::ostream &os, const OrderBook &book);

public:
    bool is_empty() const;
    void set_bid(double price, double amount);
    void set_ask(double price, double amount);

    void set_depth(uint64_t depth);
    void set_reverse(bool is_reverse);

private:
    void fix_ask_depth();
    void fix_bid_depth();

private:
    std::map<double, double> _asks, _bids;
    bool _is_reverse = false;
    uint64_t _depth = 0;
};



#endif // ORDER_BOOK_H
