cmake_minimum_required(VERSION 3.16...3.31)
project(xtd VERSION 1.0.0 LANGUAGES CXX C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(XTD_SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(XTD_INC_DIR ${CMAKE_SOURCE_DIR}/include)

add_executable(xtd
)

target_include_directories(xtd
    PUBLIC ${XTD_INC_DIR}/include
)

target_sources(xtd
    PRIVATE ${XTD_SRC_DIR}/main.cpp
)


