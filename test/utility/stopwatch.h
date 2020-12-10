#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <iostream>


class Stopwatch
{
public:
    Stopwatch(const std::string &name = "Stopwatch") :
        _name(name), _ts(std::chrono::steady_clock::now())
    {

    }

    ~Stopwatch()
    {
        const auto now = std::chrono::steady_clock::now();
        std::cout << _name << ": "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(now - _ts).count()
                  << "ms" << std::endl;
    }

    std::chrono::milliseconds elapsed() const
    {
        const auto now = std::chrono::steady_clock::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>(now - _ts);
    }

private:
    std::string _name;
    std::chrono::steady_clock::time_point _ts;
};


#endif // STOPWATCH_H
