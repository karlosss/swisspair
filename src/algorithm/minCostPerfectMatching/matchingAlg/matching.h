//
// Created by karlosss on 10/1/24.
//

#ifndef MATCHING_H
#define MATCHING_H

#include <vector>
#include "../../shared/Graph.h"
#include "../../../../gmpwrap/gmpwrap.h"

std::vector<std::pair<std::string, std::string>> compute_min_cost_perfect_matching(const UndirectedSimpleWeightedGraph<std::string, BigFloat> & graph);

#endif //MATCHING_H
