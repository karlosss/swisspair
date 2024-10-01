#include "Match.h"

Match::Match(const Player &p1, const Player &p2): p1(p1), p2(p2), is_bye(false) {}

Match::Match(const Player &p1) : p1(p1), p2(p1), is_bye(true) {}

Match::Match(const Match &other): p1 (other.p1), p2 (other.p2), is_bye (other.is_bye) {}

bool Match::operator<(const Match &other) const {
    if(is_bye) return false;
    if(other.is_bye) return true;
    if(p1.rank+p2.rank < other.p1.rank+other.p2.rank) return true;
    if(p1.rank+p2.rank > other.p1.rank+other.p2.rank) return false;
    return p1.rank < other.p1.rank;
}

std::string Match::to_string() const {
    if(!is_bye) return p1.to_string() + " - " + p2.to_string();
    return p1.to_string() + " has BYE";
}
