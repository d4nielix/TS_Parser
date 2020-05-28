cmake_minimum_required(VERSION 3.16)
project(TS_Parser)

set(CMAKE_CXX_STANDARD 14)

add_executable(TS_Parser TS_parser.cpp tsTransportStream.cpp tsCommon.h tsTransportStream.h)