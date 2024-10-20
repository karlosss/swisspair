#include "../shared/Graph.h"
#include "../PairingAlgorithm.h"
#include "../shared/utils/misc.h"
#include "../shared/utils/pods.h"
#include "../shared/utils/matches.h"

#include <unordered_set>

void _dfs(const std::vector<Player> & players, int current_player_rank, std::unordered_set<std::string> & paired_ids, bool & done, std::vector<std::pair<std::string, std::string>> & out) {
    if(paired_ids.size() == players.size()) {
        done = true;
        return;
    }

    auto current_player_id = players[current_player_rank].id;
    if(paired_ids.contains(current_player_id)) return _dfs(players, current_player_rank+1, paired_ids, done, out); // already paired

    for(int i = current_player_rank+1; i < players.size(); i++) {
        if(paired_ids.contains(players[i].id)) continue;
        if(players[current_player_rank].cannot_be_paired_against_ids.contains(players[i].id)) continue;
        if(!players[current_player_rank].can_get_bye && players[i].id == BYE_PLAYER_ID) continue;

        paired_ids.insert(players[i].id);
        paired_ids.insert(current_player_id);
        out.push_back(std::make_pair(current_player_id, players[i].id));

        _dfs(players, current_player_rank+1, paired_ids, done, out);
        if(done) return;

        out.pop_back();
        paired_ids.erase(current_player_id);
        paired_ids.erase(players[i].id);
    }
}

std::vector<std::pair<std::string, std::string>> dfs(const std::vector<Player> & players) {
    std::vector<std::pair<std::string, std::string>> out;
    std::unordered_set<std::string> paired_ids;
    bool done = false;

    _dfs(players, 0, paired_ids, done, out);

    return out;
}

std::vector<Player> create_pseudo_ranked_players_from_pods(const std::unordered_map<std::string, Player> & player_id_to_player, const std::vector<Pod> & pods) {
    std::vector<Player> out;
    for(const auto & pod: pods) {
        std::vector<std::string> ids_in_pod;
        for(const auto & player_id: pod.player_ids) {
            ids_in_pod.push_back(player_id);
        }

        auto rd = std::random_device{};
        auto rng = std::default_random_engine{ rd() };
        std::shuffle(std::begin(ids_in_pod), std::end(ids_in_pod), rng);

        for(const auto & player_id: ids_in_pod) {
            if(player_id != BYE_PLAYER_ID) {
                out.push_back(player_id_to_player.at(player_id));
            }
            else {
                Player bye;
                bye.id = BYE_PLAYER_ID;
                out.push_back(bye);
            }
        }
    }

    return out;
}


std::vector<Match> create_matches_dfs(const std::vector<Player>& players, bool powerPairing) {
    if(players.empty()) return std::vector<Match>{};

    auto player_id_to_player = create_player_id_to_player_map(players);
    std::vector<std::pair<std::string, std::string>> matching;

    if(!powerPairing) {
        auto pods = create_pods(players, false);
        auto players_for_dfs = create_pseudo_ranked_players_from_pods(player_id_to_player, pods);

        matching = dfs(players_for_dfs);
    }
    else {
        if(players.size() & 1) {
            Player bye;
            bye.id = BYE_PLAYER_ID;
            auto players_for_dfs = players;
            players_for_dfs.push_back(bye);
            matching = dfs(players_for_dfs);
        }
        else {
            matching = dfs(players);
        }
    }

    auto matches = player_ids_pairs_to_matches(matching, player_id_to_player);

    return matches;
}

// std::vector<Match> create_matches(const std::vector<Player>& players, bool powerPairing) {
//     return create_matches_dfs(players, powerPairing);
// }
