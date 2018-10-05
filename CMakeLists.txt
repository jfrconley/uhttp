cmake_minimum_required(VERSION 3.0)

project(runner)

add_library(uhttp lib/uhttp.cpp)
include_directories(node_modules/node-addon-api /home/john/.node-gyp/10.11.0/include/node)