#include "../shared/Graph.h"
#include "../PairingAlgorithm.h"
#include "../shared/utils/misc.h"
#include "../shared/utils/pods.h"
#include "../shared/utils/matches.h"

#include <unordered_set>

void _dfs(const UndirectedSimpleWeightedGraph<std::string, int> & graph, const std::vector<Player> & players, int current_player_rank, std::unordered_set<std::string> & paired_ids, bool & done, std::vector<std::pair<std::string, std::string>> & out) {
    if(paired_ids.size() == players.size()) {
        done = true;
        return;
    }

    auto current_player_id = players[current_player_rank].id;
    if(paired_ids.contains(current_player_id)) return _dfs(graph, players, current_player_rank+1, paired_ids, done, out); // already paired

    auto neighbors = graph.get_neighbors(current_player_id, false);

    for(const auto & neighbor : neighbors) {
        if(paired_ids.contains(neighbor)) continue;
        out.push_back(std::make_pair(current_player_id, neighbor));
        paired_ids.insert(current_player_id);
        if(neighbor != BYE_PLAYER_ID) {
            paired_ids.insert(neighbor);
        }

        _dfs(graph, players, current_player_rank+1, paired_ids, done, out);
        if(done) return;

        out.pop_back();
        if(neighbor != BYE_PLAYER_ID) {
            paired_ids.erase(neighbor);
        }
        paired_ids.erase(current_player_id);
    }
}

std::vector<std::pair<std::string, std::string>> dfs(const UndirectedSimpleWeightedGraph<std::string, int> & graph, const std::vector<Player> & players) {
    std::vector<std::pair<std::string, std::string>> out;
    std::unordered_set<std::string> paired_ids;
    bool done = false;
    _dfs(graph, players, 0, paired_ids, done, out);
    return out;
}

std::vector<std::pair<std::string, std::string>> power_pairing_create_graph_and_do_dfs(const std::vector<Player> & players) {
    UndirectedSimpleWeightedGraph<std::string, int> graph;

    for(const auto & player : players) {
        graph.add_vertex(player.id);
    }

    for(int i = 0; i < players.size(); ++i) {
        for(int j = i+1; j < players.size(); ++j) {
            if(players[i].previous_opponents_ids.contains(players[j].id)) continue;
            graph.add_edge(players[i].id, players[j].id, 0);
        }
    }

    if(players.size() & 1) {
        graph.add_vertex(BYE_PLAYER_ID);
        for(const auto & player : players) {
            if(!player.had_bye) graph.add_edge(player.id, BYE_PLAYER_ID, 0);
        }
    }

    return dfs(graph, players);
}


std::vector<std::pair<std::string, std::string>> create_graph_and_do_dfs(const std::unordered_map<std::string, Player> & player_id_to_player, const std::vector<Pod> & pods, const std::unordered_map<std::string, Pod> & player_id_to_pod, const std::vector<Player> & players) {
    std::vector<std::pair<std::string, std::string>> x;
    return x;
}


std::vector<Match> create_matches_dfs(const std::vector<Player>& players, bool powerPairing) {
    auto player_id_to_player = create_player_id_to_player_map(players);
    std::vector<std::pair<std::string, std::string>> matching;

    if(!powerPairing) {
        auto pods = create_pods(players, false);
        auto player_id_to_pod = create_player_id_to_pod_map(pods);
        matching = create_graph_and_do_dfs(player_id_to_player, pods, player_id_to_pod, players);
    }
    else {
        matching = power_pairing_create_graph_and_do_dfs(players);
    }

    auto matches = player_ids_pairs_to_matches(matching, player_id_to_player);

    return matches;
}

std::vector<Match> create_matches(const std::vector<Player>& players, bool powerPairing) {
    return create_matches_dfs(players, powerPairing);
}
