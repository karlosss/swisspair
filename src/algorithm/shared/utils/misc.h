//
// Created by karlosss on 10/7/24.
//

#ifndef MISC_H
#define MISC_H

#include <unordered_map>
#include <vector>

#include "../../../models/Player.h"

static std::unordered_map<std::string, Player> create_player_id_to_player_map(const std::vector<Player> & players) {
    std::unordered_map<std::string, Player> player_id_to_player_map;
    for (const auto & player : players) {
        player_id_to_player_map.insert(make_pair(player.id, player));
    }
    return player_id_to_player_map;
}

#endif //MISC_H
