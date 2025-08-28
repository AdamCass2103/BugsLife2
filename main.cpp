#include <iostream>
#include <thread>
#include <chrono>
#include "Board.h"

int main() {
    Board board;
    int choice = -1;

    do {
        std::cout << "\n1: Load bugs.txt\n2: Display all bugs\n3: Find bug\n4: Tap board\n5: Display history\n6: Display cells\n7: Run simulation\n8: Save history to file\n0: Exit\nChoice: ";
        std::cin >> choice;
        switch (choice) {
            case 1:
                board.load("bugs.txt");
                break;
            case 2:
                board.displayAll();
                break;
            case 3: {
                int id; std::cout << "ID: "; std::cin >> id;
                Bug* b = board.findById(id);
                if (b) std::cout << b->getId() << " " << b->getType() << " Alive=" << (b->isAlive() ? "Yes" : "No") << "\n";
                else std::cout << "Not found\n";
                break;
            }
            case 4:
                board.tap();
                break;
            case 5:
                board.displayHistory();
                break;
            case 6:
                board.displayCells();
                break;
            case 7:
                while (board.aliveCount() > 1) {
                    board.tap();
                    board.displayAll();
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                }
                std::cout << "Simulation complete\n";
                break;
            case 8:
                board.saveHistory("bugs_history.txt");
                std::cout << "Saved history to bugs_history.txt\n";
                break;
            case 0:
                std::cout << "Exiting\n";
                break;
            default:
                std::cout << "Invalid choice\n";
                break;
        }
    } while (choice != 0);

    return 0;
}
