cmake_minimum_required(VERSION 3.29)
project(swisspair)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(gmpwrap)
add_subdirectory(Minimum-Cost-Perfect-Matching)

target_link_libraries(mincostpmatch PUBLIC gmpwrap)
target_include_directories(mincostpmatch PUBLIC Minimum-Cost-Perfect-Matching)

add_definitions(-DDFS_THRESHOLD=200)

add_library(models src/models/Player.cpp src/models/Player.h src/models/Match.h
                   src/models/Match.cpp)

add_library(graph src/algorithm/shared/Graph.h src/algorithm/shared/Graph.cpp)
target_include_directories(graph INTERFACE src/algorithm/shared)

add_library(
  swisspair SHARED
  src/algorithm/PairingAlgorithm.h
  src/algorithm/shared/utils/pods.h
  src/algorithm/shared/utils/misc.h
  src/algorithm/shared/utils/matches.h
  src/algorithm/minCostPerfectMatching/matchingAlg/dilson_pereira.cpp
  src/algorithm/minCostPerfectMatching/MinCostPerfectMatchingV1.cpp
  src/algorithm/minCostPerfectMatching/matchingAlg/matching.h
  swisspair.h
  src/algorithm/dfs/dfsV1.cpp
  src/test.cpp
  src/algorithm/hybrid/hybridV1.cpp)

target_include_directories(swisspair PUBLIC src)
target_link_libraries(swisspair models gmpwrap mincostpmatch)

add_executable(swisspair_test src/test.cpp)
target_link_libraries(swisspair_test swisspair)
