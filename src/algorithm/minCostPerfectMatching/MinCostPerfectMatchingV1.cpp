#include "Graph.h"
#include "../PairingAlgorithm.h"
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <algorithm>
#include <random>

#include "../../../../gmpwrap/gmpwrap.h"
#include "matchingAlg/matching.h"

constexpr size_t POWER_PAIRING_PLAYER_COUNT = 8;
const std::string BYE_PLAYER_ID = "BYE";

struct Pod {
    int pod_rank;
    std::unordered_set<std::string> player_ids;
};

std::unordered_map<std::string, Player> create_player_id_to_player_map(const std::vector<Player> & players) {
    std::unordered_map<std::string, Player> player_id_to_player_map;
    for (const auto & player : players) {
        player_id_to_player_map.insert(make_pair(player.id, player));
    }
    return player_id_to_player_map;
}

std::unordered_map<std::string, Pod> create_player_id_to_pod_map(const std::vector<Pod> & pods) {
    std::unordered_map<std::string, Pod> player_id_to_pod_map;
    for (const auto & pod : pods) {
        for(const auto & player_id : pod.player_ids) {
            player_id_to_pod_map.insert(make_pair(player_id, pod));
        }
    }
    return player_id_to_pod_map;
}

std::vector<Pod> create_pods(const std::vector<Player> & players, bool power_pairing) {
    std::vector<Pod> pods;
    if(power_pairing) {
        for(int i = 0; i < std::min(POWER_PAIRING_PLAYER_COUNT, players.size()); ++i) {
            pods.emplace_back();
        }
    }

    std::map<int, Pod> points_to_pod;

    for(const auto & player : players) {
        if(power_pairing && player.rank <= POWER_PAIRING_PLAYER_COUNT) {
            pods[player.rank-1].player_ids.insert(player.id);
        }
        else {
            if(!points_to_pod.contains(player.points)) {
                points_to_pod.insert(std::make_pair(player.points, Pod()));
            }
            points_to_pod[player.points].player_ids.insert(player.id);
        }
    }

    std::vector<Pod> point_pods;

    for(const auto & entry : points_to_pod) {
        point_pods.push_back(entry.second);
    }

    for(int i = point_pods.size()-1;i >= 0; --i) {
        pods.push_back(point_pods[i]);
    }

    if(players.size() & 1) {
        Pod bye_pod;
        bye_pod.player_ids.insert(BYE_PLAYER_ID);
        pods.push_back(bye_pod);
    }

    for(int i = 0; i < pods.size(); ++i) {
        pods[i].pod_rank = i+1;
    }

    return pods;
}

void determine_and_set_precision(const std::vector<Player> & players, const std::vector<Pod> & pods) {
    auto v = players.size();
    int r = 0;
    while(v >>= 1) {
        ++r;
    }

    set_precision(r*static_cast<int>(pods.size())+32);
}

std::vector<std::vector<BigFloat>> compute_penalty_matrix(const std::vector<Pod> & pods, const std::vector<Player> & players) {
    determine_and_set_precision(players, pods);

    std::vector<std::vector<BigFloat>> penalty_matrix;
    penalty_matrix.resize(pods.size());

    for(int i = 0; i < pods.size(); ++i) {
        penalty_matrix[i].resize(pods.size());
        penalty_matrix[i][i] = 0;
    }

    if(pods.size() == 1) return penalty_matrix;

    penalty_matrix[pods.size()-2][pods.size()-1] = 1;

    if(pods.size() == 2) return penalty_matrix;

    int prevI = pods.size()-2;
    int prevJ = pods.size()-1;

    for(int i = pods.size()-3; i >= 0; --i) {
        for(int j = i+1; j < pods.size(); ++j) {
            BigFloat val = 0;
            if(i == prevI) {
                val += penalty_matrix[prevI][prevJ] * std::max(pods[prevI].player_ids.size(), pods[prevJ].player_ids.size());
            }
            for(int k = i+1; k < pods.size()-1; ++k) {
                val += penalty_matrix[k][pods.size()-1] * std::max(pods[k].player_ids.size(), pods.back().player_ids.size());
            }
            penalty_matrix[i][j] = val + 1;
            prevI = i;
            prevJ = j;
        }
    }

    return penalty_matrix;
}

std::vector<std::pair<std::string, std::string>> create_graph_and_compute_matching(const std::unordered_map<std::string, Player> & player_id_to_player, const std::vector<Pod> & pods, const std::unordered_map<std::string, Pod> & player_id_to_pod, const std::vector<Player> & players) {
    UndirectedSimpleWeightedGraph<std::string, BigFloat> graph;
    std::vector<std::string> player_ids;

    for(const auto & entry : player_id_to_pod) {
        graph.add_vertex(entry.first);
        player_ids.push_back(entry.first);
    }

    auto penalty_matrix = compute_penalty_matrix(pods, players);

    for(int i = 0; i < player_ids.size(); ++i) {
        for(int j = i+1; j < player_ids.size(); ++j) {

            const auto & p1id = player_ids[i];
            const auto & p2id = player_ids[j];

            if(p1id != BYE_PLAYER_ID && p2id != BYE_PLAYER_ID) {
                const auto & player = player_id_to_player.at(p1id);
                // prevent rematch
                if(player.previous_opponents_ids.contains(p2id)) continue;
            }

            if(p1id != BYE_PLAYER_ID && p2id == BYE_PLAYER_ID) {
                const auto & player = player_id_to_player.at(p1id);
                if(player.had_bye) continue;
            }

            if(p1id == BYE_PLAYER_ID && p2id != BYE_PLAYER_ID) {
                const auto & player = player_id_to_player.at(p2id);
                if(player.had_bye) continue;
            }

            const auto & pod1 = player_id_to_pod.at(p1id);
            const auto & pod2 = player_id_to_pod.at(p2id);

            auto penalty = penalty_matrix[std::min(pod1.pod_rank-1, pod2.pod_rank-1)][std::max(pod1.pod_rank-1, pod2.pod_rank-1)];

            graph.add_edge(p1id, p2id, penalty);
        }
    }

    return compute_min_cost_perfect_matching(graph);
}

std::vector<Match> sort_matches(const std::vector<Match> & matches) {
    std::vector<int> match_ids(matches.size());
    for(int i = 0; i < matches.size(); ++i) {
        match_ids[i] = i;
    }
    std::sort(match_ids.begin(), match_ids.end(), [matches](const int i, const int j) {return matches[i] < matches[j];});

    std::vector<Match> sorted_matches;
    for(int i = 0; i < match_ids.size(); ++i) {
        sorted_matches.push_back(matches[match_ids[i]]);
    }

    return sorted_matches;
}

std::vector<Match> player_ids_pairs_to_matches(const std::vector<std::pair<std::string, std::string>> & id_pairs, const std::unordered_map<std::string, Player> & player_id_to_player) {
    std::vector<Match> matches;
    for(const auto & pair : id_pairs) {
        if(pair.first == BYE_PLAYER_ID) {
            matches.push_back(Match(player_id_to_player.at(pair.second)));
        }
        else if(pair.second == BYE_PLAYER_ID) {
            matches.push_back(Match(player_id_to_player.at(pair.first)));
        }
        else {
            matches.push_back(Match(player_id_to_player.at(pair.first), player_id_to_player.at(pair.second)));
        }
    }

    return sort_matches(matches);
}

std::vector<Match> create_matches(const std::vector<Player>& players, bool powerPairing) {
    auto player_id_to_player = create_player_id_to_player_map(players);
    auto pods = create_pods(players, powerPairing);
    auto player_id_to_pod = create_player_id_to_pod_map(pods);
    auto matching = create_graph_and_compute_matching(player_id_to_player, pods, player_id_to_pod, players);
    auto matches = player_ids_pairs_to_matches(matching, player_id_to_player);

    return matches;
}
