cmake_minimum_required(VERSION 3.5)

project(binance_order_book LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")

add_subdirectory(test)

add_executable(${PROJECT_NAME}
    src/application.cpp
    src/application.hpp
    src/main.cpp
    src/order_book.cpp
    src/order_book.hpp
    src/utility/debugger_tools.cpp
    src/utility/debugger_tools.hpp
)

target_include_directories(${PROJECT_NAME}
    PRIVATE ./
)
