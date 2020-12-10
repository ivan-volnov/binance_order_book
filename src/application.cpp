#include "application.hpp"
#include "libs/json.hpp"
#include <csignal>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <thread>



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
        if (std::strcmp("--load", *it) == 0 && it + 1 != end) {
            _filename = *++it;
            continue;
        }
        std::cout << "Usage: " << std::filesystem::path(argv[0]).filename().string()
                << " [options]\n\n"
                   "Optional arguments:\n"
                   "--help             show this help message and exit\n"
                   "--reverse          show Order Book in reverse order\n"
                   "--depth            limit Order Book depth (min 10)\n"
                   "--load             load Binance depth stream from jsonl file\n"
                << std::flush;
        return false;
    }
    return true;
}

void Application::run()
{
    std::signal(SIGUSR1, Application::signal_handler);
    if (_filename.empty()) {
        throw std::runtime_error("Please specify input file name");
    }
    std::ifstream input(_filename);
    if (!input.is_open()) {
        throw std::runtime_error("Can't open input file name: " + _filename);
    }
    nlohmann::json j;
    std::string line;
    while (std::getline(input, line)) {
        j = nlohmann::json::parse(line).at("data");
        auto &bids = j.at("b");
        auto &asks = j.at("a");
        std::lock_guard guard(_book_mutex);
        for (const auto &value : bids) {
            _book.set_bid(std::stod(value.at(0).get<std::string>()), std::stod(value.at(1).get<std::string>()));
        }
        for (const auto &value : asks) {
            _book.set_ask(std::stod(value.at(0).get<std::string>()), std::stod(value.at(1).get<std::string>()));
        }
    }
    std::cout << "Waiting for SIGUSR1 signal for 1 minute..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(60));
    std::cout << "The time is up. Bye" << std::endl;
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
