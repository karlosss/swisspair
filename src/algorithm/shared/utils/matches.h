//
// Created by karlosss on 10/7/24.
//

#ifndef SORTING_H
#define SORTING_H

#include <vector>
#include <algorithm>

#include "../../../models/Match.h"

static std::vector<Match> sort_matches(const std::vector<Match> & matches) {
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

    return sort_matches(matches);
}

#endif //SORTING_H
