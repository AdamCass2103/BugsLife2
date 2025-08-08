#ifndef HOPPER_H
#define HOPPER_H

#include "Bug.h"
#include <string>

class Hopper : public Bug {
    int hopLength;
public:
    Hopper(int id, Position pos, Direction dir, int size, int hopLen);
    void move() override;
    std::string getType() const override { return "Hopper"; }
};

#endif
