#include "Player.h"

std::string Player::to_string() const {
    return "#" + std::to_string(rank) + " " + id + " (" + std::to_string(points) + ")";
}

bool Player::operator==(const Player &other) const {
    return id == other.id;
}
