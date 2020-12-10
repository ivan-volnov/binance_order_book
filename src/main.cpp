#include <iostream>
#include "application.hpp"
#include "utility/debugger_tools.hpp"



int main(int argc, char **argv)
{
#ifndef NDEBUG
    if (Tools::am_I_being_debugged()) {
        auto &app = Application::instance();
        if (app.parse_commandline_arguments(argc, argv)) {
            app.run();
        }
        return 0;
    }
#endif
    try {
        auto &app = Application::instance();
        if (app.parse_commandline_arguments(argc, argv)) {
            app.run();
        }
    }
    catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}
