#include <iostream>
#include <fstream>
#include <vector>

struct House { 
    int x, y;
    bool operator == (const House &other) const { return x == other.x && y == other.y; }
};

void checkDirection(House *, char );

void partOne(std::ifstream &);
void partTwo(std::ifstream &);


int main() {
    std::ifstream input("../input.txt");
    if (!input.is_open()) return -1;
    
    partOne(input);
    
    input.clear();
    input.seekg(0, std::ios::beg);
    
    partTwo(input);
    
    input.close();
    return 0;
}

void partOne(std::ifstream &input) {
    House currentHouse = {0,0};
    std::vector<House> allHouses;
    
    std::string line;
    while(getline(input, line)) {
        allHouses.push_back(currentHouse);
        for (char c : line) {
            checkDirection(&currentHouse, c);
            if (std::find(allHouses.begin(), allHouses.end(), currentHouse) == allHouses.end()) 
                allHouses.push_back(currentHouse);
        }
    }
    
    std::cout << "Answer part 1: " << allHouses.size() << std::endl;
}

void partTwo(std::ifstream &input) {
    House currentSantaHouse = {0, 0};
    House currentRoboSantaHouse = {0, 0};
    House *activeWalker = &currentSantaHouse;
    
    std::vector<House> allHouses;
    
    std::string line;
    while(getline(input, line)) {
        allHouses.push_back(*activeWalker);
        for (char c : line) {
            checkDirection(activeWalker, c);
            if (std::find(allHouses.begin(), allHouses.end(), *activeWalker) == allHouses.end())
                allHouses.push_back(*activeWalker);
            activeWalker = &(activeWalker == &currentSantaHouse ? currentRoboSantaHouse : currentSantaHouse);
        }
    }
    std::cout << "Answer part 2: " << allHouses.size() << std::endl;
}

void checkDirection(House *house, char dir) {
    if (dir == '^') house->y--;
    if (dir == 'v') house->y++;
    if (dir == '>') house->x++;
    if (dir == '<') house->x--;
}