#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <set>

struct Player {
  std::string id;
  std::set<std::string> cannot_be_paired_against_ids;
  bool can_get_bye = true;
  int points;
  int rank;

  std::string to_string() const;

  bool operator==(const Player& other) const;
};

#endif //PLAYER_H
