#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <fstream>
#include <json.hpp>
#include "utility/stopwatch.h"
#include "../src/order_book.hpp"



TEST_CASE("Parsing speed test")
{
    using namespace std::chrono_literals;
    Stopwatch sw;
    OrderBook book;
    std::ifstream input("binance-btcusdt.jsonl"); // Please fix the filepath if it differs on your system
    nlohmann::json j;
    // TODO: try a faster json parser implementation
    std::string line;
    while (std::getline(input, line)) {
        j = nlohmann::json::parse(line).at("data");
        auto &bids = j.at("b");
        for (const auto &value : bids) {
            book.set_bid(std::stod(value.at(0).get<std::string>()), std::stod(value.at(1).get<std::string>()));
        }
        auto &asks = j.at("a");
        for (const auto &value : asks) {
            book.set_ask(std::stod(value.at(0).get<std::string>()), std::stod(value.at(1).get<std::string>()));
        }
    }
    REQUIRE(sw.elapsed() < 1500ms);
}

TEST_CASE("OrderBook test")
{
    OrderBook book;
    REQUIRE(book.is_empty());

    for (int i = 0; i < 20; ++i) {
        book.set_ask(1000 + i, 1000 + i);
        book.set_bid(10 + i, 2000 + i);
    }
    REQUIRE(!book.is_empty());

    SECTION("straight order") {
        std::stringstream ss;
        ss << book;
        REQUIRE(ss.str() == R"({"a":[[1019,1019],[1018,1018],[1017,1017],[1016,1016],[1015,1015],[1014,1014],[1013,1013],[1012,1012],[1011,1011],[1010,1010],[1009,1009],[1008,1008],[1007,1007],[1006,1006],[1005,1005],[1004,1004],[1003,1003],[1002,1002],[1001,1001],[1000,1000]],"b":[[29,2019],[28,2018],[27,2017],[26,2016],[25,2015],[24,2014],[23,2013],[22,2012],[21,2011],[20,2010],[19,2009],[18,2008],[17,2007],[16,2006],[15,2005],[14,2004],[13,2003],[12,2002],[11,2001],[10,2000]]})");
    }

    SECTION("reverse order") {
        book.set_reverse(true);
        std::stringstream ss;
        ss << book;
        REQUIRE(ss.str() == R"({"b":[[10,2000],[11,2001],[12,2002],[13,2003],[14,2004],[15,2005],[16,2006],[17,2007],[18,2008],[19,2009],[20,2010],[21,2011],[22,2012],[23,2013],[24,2014],[25,2015],[26,2016],[27,2017],[28,2018],[29,2019]],"a":[[1000,1000],[1001,1001],[1002,1002],[1003,1003],[1004,1004],[1005,1005],[1006,1006],[1007,1007],[1008,1008],[1009,1009],[1010,1010],[1011,1011],[1012,1012],[1013,1013],[1014,1014],[1015,1015],[1016,1016],[1017,1017],[1018,1018],[1019,1019]]})");
    }

    SECTION("update price level") {
        book.set_ask(1019, 88);
        book.set_bid(29, 99);
        std::stringstream ss;
        ss << book;
        REQUIRE(ss.str() == R"({"a":[[1019,88],[1018,1018],[1017,1017],[1016,1016],[1015,1015],[1014,1014],[1013,1013],[1012,1012],[1011,1011],[1010,1010],[1009,1009],[1008,1008],[1007,1007],[1006,1006],[1005,1005],[1004,1004],[1003,1003],[1002,1002],[1001,1001],[1000,1000]],"b":[[29,99],[28,2018],[27,2017],[26,2016],[25,2015],[24,2014],[23,2013],[22,2012],[21,2011],[20,2010],[19,2009],[18,2008],[17,2007],[16,2006],[15,2005],[14,2004],[13,2003],[12,2002],[11,2001],[10,2000]]})");
    }

    SECTION("remove price level") {
        book.set_ask(1018, 0);
        book.set_bid(28, 0);
        std::stringstream ss;
        ss << book;
        REQUIRE(ss.str() == R"({"a":[[1019,1019],[1017,1017],[1016,1016],[1015,1015],[1014,1014],[1013,1013],[1012,1012],[1011,1011],[1010,1010],[1009,1009],[1008,1008],[1007,1007],[1006,1006],[1005,1005],[1004,1004],[1003,1003],[1002,1002],[1001,1001],[1000,1000]],"b":[[29,2019],[27,2017],[26,2016],[25,2015],[24,2014],[23,2013],[22,2012],[21,2011],[20,2010],[19,2009],[18,2008],[17,2007],[16,2006],[15,2005],[14,2004],[13,2003],[12,2002],[11,2001],[10,2000]]})");
    }

    SECTION("depth limit test") {
        book.set_depth(10);
        std::stringstream ss;
        ss << book;
        REQUIRE(ss.str() == R"({"a":[[1009,1009],[1008,1008],[1007,1007],[1006,1006],[1005,1005],[1004,1004],[1003,1003],[1002,1002],[1001,1001],[1000,1000]],"b":[[29,2019],[28,2018],[27,2017],[26,2016],[25,2015],[24,2014],[23,2013],[22,2012],[21,2011],[20,2010]]})");
        book.set_ask(1019, 88);
        book.set_bid(10, 99);
        std::stringstream ss2;
        ss2 << book;
        REQUIRE(ss.str() == ss2.str());
    }
}
