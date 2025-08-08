#include "Hopper.h"

Hopper::Hopper(int id, Position pos, Direction dir, int size, int hopLen)
    : Bug(id, pos, dir, size), hopLength(hopLen) {}

void Hopper::move() {
    if (!alive) return;
    int attempts = 0;
    while (isWayBlocked() && attempts < 10) {
        direction = randomDirection();
        attempts++;
    }
    Position nx = nextPosition(position, direction, hopLength);
    if (inBounds(nx)) {
        position = nx;
    } else {
        // move until edge
        for (int i = 1; i <= hopLength; i++) {
            Position step = nextPosition(position, direction, i);
            if (!inBounds(step)) {
                position = nextPosition(position, direction, i - 1);
                break;
            }
        }
    }
    path.push_back(position);
}
