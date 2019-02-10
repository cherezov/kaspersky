cmake_minimum_required (VERSION 2.8.12)
project(kaspersky)

set(CMAKE_CXX_FLAGS "-std=c++1y -Wno-invalid-offsetof -pthread")

file(GLOB _src "*.cpp" "*.h")

add_executable(kaspersky ${_src} )
