#include <iostream>
#include "src/algorithm/PairingAlgorithm.h"
#include "src/models/Player.h"

int main() {
    // std::vector<Player> players {
    //     Player {"P1", {}, false, 9, 1},
    //     Player {"P2", {}, false, 8, 2},
    //     Player {"P3", {}, false, 8, 3},
    //     Player {"P4", {}, false, 7, 4},
    // };

    std::vector<Player> players;

    for(int i = 0; i < 20; ++i) {
        players.push_back(Player {"P" + std::to_string(i), {}, false, 0, i+1});
    }

    // std::vector<Player> players {
    //     Player {"P1", {}, false, 6, 1},
    //     Player {"P2", {}, false, 6, 2},
    //     Player {"P3", {"P4"}, false, 3, 3},
    //     Player {"P4", {"P3"}, false, 3, 4},
    //     Player {"P5", {}, false, 0, 5},
    //     Player {"P6", {}, false, 0, 6},
    // };

    auto matches = create_matches(players, true);

    for(const auto & match : matches) {
        std::cout << match.to_string() << std::endl;
    }
    return 0;
}
