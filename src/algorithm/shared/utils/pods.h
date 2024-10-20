//
// Created by karlosss on 10/7/24.
//

#ifndef PODS_H
#define PODS_H

#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <map>

#include "../../../models/Player.h"

constexpr size_t POWER_PAIRING_PLAYER_COUNT = -1;
const std::string BYE_PLAYER_ID = "BYE";

struct Pod {
    int pod_rank;
    std::unordered_set<std::string> player_ids;
};

static std::unordered_map<std::string, Pod> create_player_id_to_pod_map(const std::vector<Pod> & pods) {
    std::unordered_map<std::string, Pod> player_id_to_pod_map;
    for (const auto & pod : pods) {
        for(const auto & player_id : pod.player_ids) {
            player_id_to_pod_map.insert(make_pair(player_id, pod));
        }
    }
    return player_id_to_pod_map;
}

static std::vector<Pod> create_pods(const std::vector<Player> & players, bool power_pairing) {
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

#endif
