//
// Created by karlosss on 10/7/24.
//

#ifndef SORTING_H
#define SORTING_H

#include <vector>
#include <algorithm>

#include "../../../models/Match.h"

static void sort_matches(std::vector<Match> & matches) {
    std::sort(matches.begin(), matches.end(), [](const Match & i, const Match & j) {return i < j;});
}

static std::vector<Match> player_ids_pairs_to_matches(const std::vector<std::pair<std::string, std::string>> & id_pairs, const std::unordered_map<std::string, Player> & player_id_to_player) {
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

    sort_matches(matches);
    return matches;
}

#endif //SORTING_H
