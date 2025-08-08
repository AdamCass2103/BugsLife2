#include "Crawler.h"

Crawler::Crawler(int id, Position pos, Direction dir, int size)
    : Bug(id, pos, dir, size) {}

void Crawler::move() {
    if (!alive) return;
    int attempts = 0;
    while (isWayBlocked() && attempts < 10) {
        direction = randomDirection();
        attempts++;
    }
    Position nx = nextPosition(position, direction, 1);
    if (!inBounds(nx)) return;
    position = nx;
    path.push_back(position);
}
