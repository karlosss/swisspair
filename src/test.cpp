//
// Created by karlosss on 10/8/24.
//

#include <iostream>
#include <vector>

#include "algorithm/PairingAlgorithm.h"
#include "models/Player.h"

int main() {
    std::vector<Player> players;
    for(int i = 0; i < 2000; ++i) {
        Player p;
        p.points = 0;
        p.id = "P" + std::to_string(i+1);
        p.rank = i+1;
        players.push_back(p);
    }

    players[0].previous_opponents_ids.insert(players[1].id);
    players[1].previous_opponents_ids.insert(players[0].id);

    auto matches = create_matches(players, true);

    for(const auto & match : matches) {
        std::cout << match.to_string() << std::endl;
    }

    return 0;
}
