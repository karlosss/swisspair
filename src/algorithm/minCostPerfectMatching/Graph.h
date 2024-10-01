//
// Created by karlosss on 10/1/24.
//

#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <unordered_map>
#include <random>
#include <algorithm>
#include <iostream>
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
      edges.emplace_back();
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

    edges[uid].insert(std::make_pair(vid, w));
    ++edge_cnt;
  }

  std::vector<Edge> get_edges() const {

    std::vector<Edge> out;
    for(int i = 0; i < edges.size(); ++i) {
      for(const auto & entry : edges[i]) {
        out.push_back(Edge(vertex_id_to_name[i], vertex_id_to_name[entry.first], entry.second));
      }
    }

    auto rd = std::random_device{};
    auto rng = std::default_random_engine{ rd() };
    std::shuffle(std::begin(out), std::end(out), rng);

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

    std::vector<std::unordered_map<int, TWeight>> edges;
};


#endif //GRAPH_H
