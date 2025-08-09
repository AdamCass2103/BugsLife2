#include "Board.h"
#include "Crawler.h"
#include "Hopper.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <random>
#include <chrono>

Board::Board() = default;
Board::~Board() { for (auto* b : bugs) delete b; }

void Board::load(const std::string& filename) {
    std::ifstream in(filename);
    bugs.clear();
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        char type; ss >> type; ss.ignore(1, ';');
        int id, x, y, dir, size;
        ss >> id; ss.ignore(1, ';');
        ss >> x; ss.ignore(1, ';');
        ss >> y; ss.ignore(1, ';');
        ss >> dir; ss.ignore(1, ';');
        ss >> size;
        if (type == 'C') {
            bugs.push_back(new Crawler(id, {x,y}, static_cast<Direction>(dir), size));
        } else {
            ss.ignore(1, ';');
            int hop; ss >> hop;
            bugs.push_back(new Hopper(id, {x,y}, static_cast<Direction>(dir), size, hop));
        }
    }
}

void Board::displayAll() const {
    for (const auto* b : bugs) {
        std::cout << b->getId() << " " << b->getType()
                  << " (" << b->getPosition().x << "," << b->getPosition().y << ")"
                  << " size=" << b->getSize()
                  << " Alive=" << b->isAlive() << "\n";
    }
}

Bug* Board::findById(int id) const {
    for (auto* b : bugs) if (b->getId()==id) return b;
    return nullptr;
}

void Board::tap() {
    for (auto* b : bugs) if (b->isAlive()) b->move();
    fightAndEat();
}

void Board::fightAndEat() {
    std::map<std::pair<int,int>, std::vector<Bug*>> cells;
    for (auto* b : bugs)
        if (b->isAlive())
            cells[{b->getPosition().x, b->getPosition().y}].push_back(b);

    std::mt19937 gen(std::random_device{}());
    for (auto& [pos, vec] : cells) {
        if (vec.size() < 2) continue;
        int maxSize = 0;
        for (auto* b : vec) maxSize = std::max(maxSize, b->getSize());
        std::vector<Bug*> contenders;
        for (auto* b : vec) if (b->getSize() == maxSize) contenders.push_back(b);
        std::uniform_int_distribution<int> dist(0, contenders.size()-1);
        Bug* winner = contenders.size()==1 ? contenders[0] : contenders[dist(gen)];
        for (auto* b : vec)
            if (b != winner) {
                b->setAlive(false);
                winner->growBy(b->getSize());
            }
    }
}

void Board::displayHistory() const {
    for (const auto* b : bugs) {
        std::cout << b->getId() << " " << b->getType() << " Path: ";
        for (auto& p : b->getPath())
            std::cout << "(" << p.x << "," << p.y << ")";
        std::cout << (b->isAlive() ? " Alive" : " Dead") << "\n";
    }
}

void Board::displayCells() const {
    std::map<std::pair<int,int>, std::vector<const Bug*>> cells;
    for (const auto* b : bugs)
        if (b->isAlive())
            cells[{b->getPosition().x,b->getPosition().y}].push_back(b);

    for (int y=0; y<10; y++)
        for (int x=0; x<10; x++) {
            std::cout << "("<<x<<","<<y<<"):";
            auto it = cells.find({x,y});
            if (it == cells.end()) std::cout << " empty\n";
            else {
                for (auto* b:it->second)
                    std::cout << " " << b->getType() << b->getId();
                std::cout << "\n";
            }
        }
}

void Board::saveHistory(const std::string& filename) const {
    std::ofstream out(filename);
    for (const auto* b : bugs) {
        out << b->getId() << " " << b->getType() << " Path: ";
        for (auto& p : b->getPath())
            out << "(" << p.x << "," << p.y << ")";
        out << (b->isAlive() ? " Alive" : " Dead") << "\n";
    }
}

int Board::aliveCount() const {
    int count = 0;
    for (const auto* b : bugs) if (b->isAlive()) count++;
    return count;
}
