#ifndef POSITION_H
#define POSITION_H

struct Position {
    int x;
    int y;

    Position(int xPos = 0, int yPos = 0) : x(xPos), y(yPos) {}
};

#endif
