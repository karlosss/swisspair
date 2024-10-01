#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <set>

struct Player {
  std::string id;
  std::set<std::string> previous_opponents_ids;
  bool had_bye;
  int points;
  int rank;

  std::string to_string() const;

  bool operator==(const Player& other) const;
};

#endif //PLAYER_H
