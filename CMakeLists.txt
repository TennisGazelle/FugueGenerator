cmake_minimum_required(VERSION 3.6)
project(FugueGenerator)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

INCLUDE_DIRECTORIES(
        "include"
)

set(SOURCE_FILES
    include/rapidxml/rapidxml.hpp
    include/rapidxml/rapidxml_iterators.hpp
    include/rapidxml/rapidxml_print.hpp
    include/rapidxml/rapidxml_utils.hpp
    include/tinyxml2.h
    src/fugue_gen.cpp
    src/tinyxml2.cpp)

add_executable(FugueGenerator ${SOURCE_FILES})