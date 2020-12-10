#ifndef APPLICATION_H
#define APPLICATION_H

#include "order_book.hpp"
#include <string>
#include <mutex>



class Application
{
public:
    ~Application() = default;
    Application(const Application &) = delete;
    Application &operator=(const Application &) = delete;

    static Application &instance()
    {
        static Application app;
        return app;
    }

public:
    bool parse_commandline_arguments(int argc, char **argv);
    void run();
    static void signal_handler(int signal);

private:
    Application() = default;

private:
    std::string _filename;
    OrderBook _book;
    std::mutex _book_mutex;
};



#endif // APPLICATION_H
