#include <vector>

#include "../../models/Match.h"
#include "../../models/Player.h"

#include "../PairingAlgorithm.h"

std::vector<Match> create_matches_mcpm(const std::vector<Player>& players, bool powerPairing);
std::vector<Match> create_matches_dfs(const std::vector<Player>& players, bool powerPairing);


std::vector<Match> create_matches(const std::vector<Player>& players, bool powerPairing) {
    if(players.size() >= DFS_THRESHOLD) {
        return create_matches_dfs(players, powerPairing);
    }
    return create_matches_mcpm(players, powerPairing);
}
