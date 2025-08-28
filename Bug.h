#ifndef BUG_H
#define BUG_H

#include <list>
#include <string>
#include "Position.h"
#include "Direction.h"

class Bug {
protected:
    int id;
    Position position;
    Direction direction;
    int size;
    bool alive;
    std::list<Position> path;

    bool isWayBlocked() const;
    static Direction randomDirection();
    static Position nextPosition(const Position& pos, Direction dir, int step);
    static bool inBounds(const Position& pos);

public:
    Bug(int id, Position pos, Direction dir, int size);
    virtual ~Bug();

    int getId() const;
    Position getPosition() const;
    int getSize() const;
    bool isAlive() const;
    void setAlive(bool status);
    void growBy(int amount);
    const std::list<Position>& getPath() const;

    virtual void move() = 0;
    virtual std::string getType() const = 0;
};

#endif // BUG_H
