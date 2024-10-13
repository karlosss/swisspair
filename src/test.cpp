    #include <iostream>
    #include <vector>
#include <fstream>

    #include "algorithm/PairingAlgorithm.h"
    #include "models/Player.h"

    int main() {
           std::vector<Player> players;

            std::ifstream f("/tmp/out.txt");

            int n;
        f >> n;

        for(int i = 0; i < n; i++) {
            Player player;
            int m;
            f >> player.id >> player.points >> player.rank >> player.can_get_bye;
            f >> m;
            for(int j = 0; j < m; j++) {
                std::string x;
                f >> x;
                player.cannot_be_paired_against_ids.insert(x);
            }
            players.push_back(player);
        }

              auto matches = create_matches(players, false);

               for(const auto & match : matches) {
                   std::cout << match.to_string() << std::endl;
               }

               return 0;
               }
