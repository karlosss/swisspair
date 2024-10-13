cmake_minimum_required(VERSION 3.29)
project(swisspair)

set(CMAKE_CXX_STANDARD 20)

add_definitions(-DDFS_THRESHOLD=200)

add_library(swisspair SHARED
        src/models/Player.cpp
        src/models/Player.h
        src/models/Match.h
        src/models/Match.cpp
        src/algorithm/PairingAlgorithm.h
        src/algorithm/minCostPerfectMatching/matchingAlg/dilson_pereira.cpp
        src/algorithm/shared/Graph.h
        src/algorithm/minCostPerfectMatching/MinCostPerfectMatchingV1.cpp
        src/algorithm/minCostPerfectMatching/matchingAlg/matching.h
        swisspair.h
        src/algorithm/shared/utils/pods.h
        src/algorithm/shared/utils/misc.h
        src/algorithm/shared/utils/matches.h
        src/algorithm/dfs/dfsV1.cpp
        src/test.cpp
        src/algorithm/hybrid/hybridV1.cpp
)

#add_link_options(-lgmp)
#
#add_subdirectory(gmpwrap)
#add_subdirectory(Minimum-Cost-Perfect-Matching)
#
#add_executable(swisspair src/models/Player.cpp
#        src/models/Player.h
#        src/models/Match.h
#        src/models/Match.cpp
#        src/algorithm/PairingAlgorithm.h
#        src/algorithm/minCostPerfectMatching/matchingAlg/dilson_pereira.cpp
#        src/algorithm/shared/Graph.h
#        src/algorithm/minCostPerfectMatching/MinCostPerfectMatchingV1.cpp
#        src/algorithm/minCostPerfectMatching/matchingAlg/matching.h
#        swisspair.h
#        src/algorithm/shared/utils/pods.h
#        src/algorithm/shared/utils/misc.h
#        src/algorithm/shared/utils/matches.h
#        src/algorithm/dfs/dfsV1.cpp
#        src/test.cpp
#        src/algorithm/hybrid/hybridV1.cpp)
#
#target_link_libraries(swisspair gmpwrap)
#target_link_libraries(swisspair mincostpmatch)
