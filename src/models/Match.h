#ifndef MATCH_H
#define MATCH_H
#include "Player.h"

struct Match {
  Player p1;
  Player p2;
  bool is_bye;

  Match(const Player & p1, const Player & p2);
  Match(const Player & p1);
  Match(const Match & other);

  bool operator<(const Match & other) const;


  std::string to_string() const;
};

#endif //MATCH_H
