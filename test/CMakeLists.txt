cmake_minimum_required(VERSION 3.5)

project(test_binance_order_book LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")

add_executable(${PROJECT_NAME}
    ../src/order_book.cpp
    ../src/order_book.hpp
    unittest.cpp
    utility/stopwatch.h
)

target_include_directories(${PROJECT_NAME}
    PRIVATE ./ ../libs
)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE TESTING
)
