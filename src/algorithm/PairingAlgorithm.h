#ifndef PAIRINGALGORITHM_H
#define PAIRINGALGORITHM_H

#include <vector>
#include "../models/Match.h"
#include "../models/Player.h"

std::vector<Match> create_matches(const std::vector<Player>& players, bool powerPairing);

#endif //PAIRINGALGORITHM_H
