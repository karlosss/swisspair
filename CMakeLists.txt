cmake_minimum_required(VERSION 3.29)
project(swisspair)

set(CMAKE_CXX_STANDARD 20)

add_link_options(-lgmp)

find_library(GMPWRAP_LIB libgmpwrap.so ../gmpwrap/cmake-build-debug ../gmpwrap)
find_library(MINCOSTPMATCH_LIB libmincostpmatch.so ../Minimum-Cost-Perfect-Matching/cmake-build-debug ../Minimum-Cost-Perfect-Matching)

add_library(swisspair SHARED src/models/Player.cpp src/models/Player.h src/models/Match.h src/models/Match.cpp
        src/algorithm/PairingAlgorithm.h
        src/algorithm/minCostPerfectMatching/matchingAlg/dilson_pereira.cpp
        src/algorithm/minCostPerfectMatching/Graph.h
        src/algorithm/minCostPerfectMatching/MinCostPerfectMatchingV1.cpp
        src/algorithm/minCostPerfectMatching/matchingAlg/matching.h
        swisspair.h)

target_link_libraries(swisspair "${GMPWRAP_LIB}")
target_link_libraries(swisspair "${MINCOSTPMATCH_LIB}")
