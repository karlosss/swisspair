#include "matching.h"
#include <unordered_map>

#include "../../../../gmpwrap/gmpwrap.h"
#include "../../../../Minimum-Cost-Perfect-Matching/library.h"

std::vector<std::pair<std::string, std::string>> compute_min_cost_perfect_matching(const UndirectedSimpleWeightedGraph<std::string, BigFloat> & graph) {
    Graph g(graph.num_vertices());
    std::vector<std::string> vertex_id_to_name;
    std::unordered_map<std::string, int> name_to_vertex_id;

    for(const auto & vertex: graph.get_vertices()) {
        vertex_id_to_name.push_back(vertex);
        name_to_vertex_id.insert(std::make_pair(vertex, vertex_id_to_name.size()-1));
    }

    std::vector<BigFloat> costs(graph.num_vertices()*graph.num_vertices());
    auto edges = graph.get_edges(true);

    for(const auto & edge: edges) {
        g.AddEdge(name_to_vertex_id[edge.u], name_to_vertex_id[edge.v]);
        costs[g.GetEdgeIndex(name_to_vertex_id[edge.u], name_to_vertex_id[edge.v])] = edge.w;
    }

    Matching matching_alg(g);
    auto matching = matching_alg.SolveMinimumCostPerfectMatching(costs);

    std::vector<std::pair<std::string, std::string>> result;

    for(const auto & edge_idx : matching.first) {
        auto e = g.GetEdge(edge_idx);
        auto u = vertex_id_to_name[e.first];
        auto v = vertex_id_to_name[e.second];
        result.push_back(std::make_pair(u, v));
    }

    return result;
}
