//
// Created by karlosss on 10/1/24.
//

#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <unordered_map>
#include <random>
#include <algorithm>
#include <sstream>


template <typename TVertex, typename TWeight>
class UndirectedSimpleWeightedGraph {
public:
  struct Edge {
    TVertex u;
    TVertex v;
    TWeight w;

    std::string to_string() const {
      std::stringstream ss;
      ss << u << " " << v << " " << w;
      return ss.str();
    }
  };

  void add_vertex(TVertex name) {
      vertex_id_to_name.push_back(name);
      vertex_name_to_id.insert(std::make_pair(name, vertex_id_to_name.size()-1));

      adjacency_map.emplace_back();
      adjacency_list.emplace_back();
    }

  void add_edge(TVertex u, TVertex v, TWeight w) {
    auto uid = vertex_name_to_id[u];
    auto vid = vertex_name_to_id[v];
    if(uid == vid) return;

    if(vid < uid) {
      auto tmp = uid;
      uid = vid;
      vid = tmp;
    }

    if(adjacency_map[uid].contains(vid)) return;

    adjacency_map[uid].insert(std::make_pair(vid, w));
    adjacency_list[uid].emplace_back(vid);
    ++edge_cnt;
  }

  std::vector<Edge> get_edges(bool randomized) const {

    std::vector<Edge> out;
    for(int i = 0; i < adjacency_list.size(); ++i) {
      for(int j = 0; j < adjacency_list[i].size(); ++j) {
        out.push_back(Edge(vertex_id_to_name[i], vertex_id_to_name[adjacency_list[i][j]], adjacency_map.at(i).at(adjacency_list[i][j])));
      }
    }

    if(randomized) {
      auto rd = std::random_device{};
      auto rng = std::default_random_engine{ rd() };
      std::shuffle(std::begin(out), std::end(out), rng);
    }

    return out;
  }

  std::vector<TVertex> get_vertices() const {
    return vertex_id_to_name;
  }

  int num_vertices() const {
    return vertex_id_to_name.size();
  }

  int num_edges() const {
    return edge_cnt;
  }

  std::vector<TVertex> get_neighbors(TVertex vertex, bool randomized) const {
    auto vid = vertex_name_to_id.at(vertex);
    std::vector<TVertex> out;
    for(int i = 0; i < adjacency_list[vid].size(); ++i) {
      out.emplace_back(vertex_id_to_name[adjacency_list[vid][i]]);
    }

    if(randomized) {
      auto rd = std::random_device{};
      auto rng = std::default_random_engine{ rd() };
      std::shuffle(std::begin(out), std::end(out), rng);
    }

    return out;
  }

  std::string to_string() const {
    std::stringstream ss;
    ss << num_vertices() << "\n";
    ss << num_edges() << "\n";
    for(const auto & edge : get_edges()) {
      ss << edge.u << " " << edge.v << " " << edge.w << "\n";
    }
    return ss.str();
  }

private:
    int edge_cnt = 0;
    std::vector<TVertex> vertex_id_to_name;
    std::unordered_map<TVertex, int> vertex_name_to_id;

    std::vector<std::unordered_map<int, TWeight>> adjacency_map;
    std::vector<std::vector<int>> adjacency_list;
};


#endif //GRAPH_H
