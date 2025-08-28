#include "Board.h"
#include "Crawler.h"
#include "Hopper.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <random>

Board::Board() = default;

Board::~Board() {
    for (auto* b : bugs) delete b;
    bugs.clear();
}

void Board::load(const std::string& filename) {
    std::ifstream in(filename);
    if (!in) {
        std::cerr << "Could not open " << filename << "\n";
        return;
    }

    for (auto* b : bugs) delete b;
    bugs.clear();

    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        char type;
        ss >> type; // reads 'C' or 'H'
        ss.ignore(1, ';');

        int id, x, y, dirInt, size;
        ss >> id; ss.ignore(1, ';');
        ss >> x; ss.ignore(1, ';');
        ss >> y; ss.ignore(1, ';');
        ss >> dirInt; ss.ignore(1, ';');
        ss >> size;

        Direction dir = static_cast<Direction>(dirInt);

        if (type == 'C') {
            bugs.push_back(new Crawler(id, {x,y}, dir, size));
        } else if (type == 'H') {
            ss.ignore(1, ';'); // skip separator before hop length if present
            int hopLen = 1;
            if (ss >> hopLen) {
                bugs.push_back(new Hopper(id, {x,y}, dir, size, hopLen));
            } else {
                // default hop length if missing
                bugs.push_back(new Hopper(id, {x,y}, dir, size, 2));
            }
        }
    }

    std::cout << "Loaded " << bugs.size() << " bugs from " << filename << "\n";
}

void Board::displayAll() const {
    if (bugs.empty()) { std::cout << "No bugs loaded.\n"; return; }
    for (const auto* b : bugs) {
        std::cout << b->getId() << " " << b->getType()
                  << " (" << b->getPosition().x << "," << b->getPosition().y << ")"
                  << " size=" << b->getSize()
                  << " Alive=" << (b->isAlive() ? "Yes" : "No");
        if (b->getType() == "Hopper") {
            auto* h = dynamic_cast<const Hopper*>(b);
            if (h) std::cout << " hop=" << h->getHopLength();
        }
        std::cout << "\n";
    }
}

Bug* Board::findById(int id) const {
    for (auto* b : bugs) if (b->getId() == id) return b;
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
    for (auto& kv : cells) {
        auto& vec = kv.second;
        if (vec.size() < 2) continue;

        int maxSize = 0;
        for (auto* b : vec) maxSize = std::max(maxSize, b->getSize());

        std::vector<Bug*> contenders;
        for (auto* b : vec) if (b->getSize() == maxSize) contenders.push_back(b);

        Bug* winner = nullptr;
        if (contenders.size() == 1) winner = contenders.front();
        else {
            std::uniform_int_distribution<int> dist(0, (int)contenders.size() - 1);
            winner = contenders[dist(gen)];
        }

        for (auto* b : vec) {
            if (b == winner) continue;
            b->setAlive(false);
            winner->growBy(b->getSize());
        }
    }
}

void Board::displayHistory() const {
    for (const auto* b : bugs) {
        std::cout << b->getId() << " " << b->getType() << " Path: ";
        for (auto it = b->getPath().begin(); it != b->getPath().end(); ++it) {
            std::cout << "(" << it->x << "," << it->y << ")";
            if (std::next(it) != b->getPath().end()) std::cout << ",";
        }
        std::cout << (b->isAlive() ? " Alive" : " Dead") << "\n";
    }
}

void Board::displayCells() const {
    std::map<std::pair<int,int>, std::vector<const Bug*>> cells;
    for (const auto* b : bugs)
        if (b->isAlive())
            cells[{b->getPosition().x, b->getPosition().y}].push_back(b);

    for (int y = 0; y < 10; ++y) {
        for (int x = 0; x < 10; ++x) {
            auto key = std::make_pair(x,y);
            std::cout << "(" << x << "," << y << "): ";
            auto it = cells.find(key);
            if (it == cells.end()) std::cout << "empty\n";
            else {
                const auto& vec = it->second;
                for (size_t i = 0; i < vec.size(); ++i) {
                    std::cout << vec[i]->getType() << " " << vec[i]->getId();
                    if (i + 1 < vec.size()) std::cout << ", ";
                }
                std::cout << "\n";
            }
        }
    }
}

void Board::saveHistory(const std::string& filename) const {
    std::ofstream out(filename);
    for (const auto* b : bugs) {
        out << b->getId() << " " << b->getType() << " Path: ";
        for (auto it = b->getPath().begin(); it != b->getPath().end(); ++it) {
            out << "(" << it->x << "," << it->y << ")";
            if (std::next(it) != b->getPath().end()) out << ",";
        }
        out << (b->isAlive() ? " Alive" : " Dead") << "\n";
    }
}
int Board::aliveCount() const {
    int c = 0;
    for (const auto* b : bugs) if (b->isAlive()) ++c;
    return c;
}
