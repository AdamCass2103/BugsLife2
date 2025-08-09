#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include "Bug.h"

class Board {
    std::vector<Bug*> bugs;
    void fightAndEat();

public:
    Board();
    ~Board();

    void load(const std::string& filename);
    void displayAll() const;
    Bug* findById(int id) const;
    void tap();
    void displayHistory() const;
    void displayCells() const;
    void saveHistory(const std::string& filename) const;
    int aliveCount() const;
};

#endif
