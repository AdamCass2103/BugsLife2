#include "Bug.h"
#include <random>

static std::mt19937& rng() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    return gen;
}

Bug::Bug(int id, Position pos, Direction dir, int size)
    : id(id), position(pos), direction(dir), size(size), alive(true) {
    path.push_back(position);
}
Bug::~Bug() = default;
int Bug::getId() const { return id; }
Position Bug::getPosition() const { return position; }
int Bug::getSize() const { return size; }
bool Bug::isAlive() const { return alive; }
void Bug::setAlive(bool status) { alive = status; }
void Bug::growBy(int amount) { size += amount; }
const std::list<Position>& Bug::getPath() const { return path; }

bool Bug::isWayBlocked() const {
    Position next = nextPosition(position, direction, 1);
    return !inBounds(next);
}

Direction Bug::randomDirection() {
    std::uniform_int_distribution<int> dist(1, 4);
    return static_cast<Direction>(dist(rng()));
}

Position Bug::nextPosition(const Position& pos, Direction dir, int step) {
    Position res = pos;
    if (dir == Direction::North) res.y -= step;
    else if (dir == Direction::East) res.x += step;
    else if (dir == Direction::South) res.y += step;
    else if (dir == Direction::West) res.x -= step;
    return res;
}

bool Bug::inBounds(const Position& pos) {
    return pos.x >= 0 && pos.x < 10 && pos.y >= 0 && pos.y < 10;
}
