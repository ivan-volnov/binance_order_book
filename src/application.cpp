#include "application.hpp"
#include "libs/json.hpp"
#include <csignal>
#include <iostream>



bool Application::parse_commandline_arguments(int argc, char **argv)
{
    for (char **it = argv + 1, **end = argv + argc; it != end; ++it) {
        if (std::strcmp("--reverse", *it) == 0) {
            _book.set_reverse(true);
            continue;
        }
        if (std::strcmp("--depth", *it) == 0 && it + 1 != end) {
            const auto depth = std::atoll(*++it);
            if (depth >= 10) {
                _book.set_depth(depth);
                continue;
            }
        }
        std::cout << "Usage: binance_order_book [options]\n\n"
                   "Optional arguments:\n"
                   "--help             show this help message and exit\n"
                   "--reverse          show Order Book in reverse order\n"
                   "--depth            limit Order Book depth (min 10)\n"
                << std::flush;
        return false;
    }
    return true;
}

void Application::run()
{
    std::signal(SIGUSR1, Application::signal_handler);
    nlohmann::json json;
    uint64_t U, u{};
    std::string line;
    while (std::getline(std::cin, line)) {
        json = nlohmann::json::parse(line).at("data");
        U = json.at("U").get<uint64_t>();
        if (!u) {
            u = U - 1; // TODO: use lastUpdateId from depth snapshot
        }
        if (u + 1 != U) {
            throw std::runtime_error("Wrong stream data update ID");
        }
        u = json.at("u").get<uint64_t>();
        auto &bids = json.at("b");
        auto &asks = json.at("a");
        std::lock_guard guard(_book_mutex);
        for (const auto &value : bids) {
            _book.set_bid(std::stod(value.at(0).get<std::string>()), std::stod(value.at(1).get<std::string>()));
        }
        for (const auto &value : asks) {
            _book.set_ask(std::stod(value.at(0).get<std::string>()), std::stod(value.at(1).get<std::string>()));
        }
        _book.set_update_ts(json.at("E").get<uint64_t>());
    }
}

void Application::signal_handler(int signal)
{
    switch (signal) {
    case SIGUSR1:
    {
        auto &app = Application::instance();
        std::lock_guard guard(app._book_mutex);
        std::cout << app._book << std::endl;
        break;
    }
    default:
        break;
    }
}
