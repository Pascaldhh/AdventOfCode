#include <iostream>
#include <fstream>
#include <vector>
#include <map>

enum class Rock { Rounded, Cube, Empty };
enum class Direction { North, West, South, East }; // Correct order for cycle

struct Platform {
    std::vector<std::vector<Rock>> grid;
    void slideDirection(Direction);
    void slideCycle();
    void slideCycleAmount(int n);
    long totalLoad();
    std::string str();
    static std::map<Rock, char> RockCharTable;
};

std::map<Rock, char> Platform::RockCharTable {
    {Rock::Rounded, 'O'},
    {Rock::Cube, '#'},
    {Rock::Empty, '.'}
};
Platform getPlatform(std::ifstream &);

void partOne(Platform);
void partTwo(Platform);
int main() {
    std::ifstream input("../input.txt");
    if (!input.is_open()) return -1;
    
    Platform platform = getPlatform(input);
//    partOne(platform);
    partTwo(platform);
    
    input.close();
    return 0;
}

void partOne(Platform platform) {
    platform.slideDirection(Direction::North);
    std::cout << "Answer for part 1: " << platform.totalLoad() << std::endl;
}

void partTwo(Platform platform) {
    platform.slideCycleAmount(100000000);
//    std::cout << platform.str() << std::endl;
    std::cout << "Answer for part 2: " << platform.totalLoad() << std::endl;
}

void Platform::slideDirection(Direction direction) {
    switch (direction) {
        case Direction::North: {
            
            for (int x = 0; x < grid[0].size(); x++) {
                int roundedCount = 0, insertY = 0;
                for (int y = 0; y < grid.size(); y++) {
                    if (grid[y][x] == Rock::Rounded) {
                        roundedCount++;
                        grid[y][x] = Rock::Empty;
                    }
                    if (grid[y][x] == Rock::Cube) {
                        for (int i = 0; i < roundedCount; i++)
                            grid[insertY+i][x] = Rock::Rounded;

                        roundedCount = 0;
                        insertY = y+1;
                    }
                }
                if (roundedCount > 0) {
                    for (int i = 0; i < roundedCount; i++)
                        grid[insertY+i][x] = Rock::Rounded;
                }
            }
            break;
        }
        case Direction::East: {
            for (int y = 0; y < grid.size(); y++) {
                int roundedCount = 0, insertX = grid[y].size()-1;
                for (int x = grid[y].size()-1; x >= 0; x--) {
                    if (grid[y][x] == Rock::Rounded) {
                        roundedCount++;
                        grid[y][x] = Rock::Empty;
                    }
                    if (grid[y][x] == Rock::Cube) {
                        for (int i = 0; i < roundedCount; i++)
                            grid[y][insertX-i] = Rock::Rounded;

                        roundedCount = 0;
                        insertX = x-1;
                    }
                }
                if (roundedCount > 0) {
                    for (int i = 0; i < roundedCount; i++)
                        grid[y][insertX-i] = Rock::Rounded;
                }
            }

            break;
        }
        case Direction::South: {
            for (int x = 0; x < grid[0].size(); x++) {
                int roundedCount = 0, insertY = grid.size()-1;
                for (int y = grid.size()-1; y >= 0; y--) {
                    if (grid[y][x] == Rock::Rounded) {
                        roundedCount++;
                        grid[y][x] = Rock::Empty;
                    }
                    if (grid[y][x] == Rock::Cube) {
                        for (int i = 0; i < roundedCount; i++)
                            grid[insertY-i][x] = Rock::Rounded;

                        roundedCount = 0;
                        insertY = y-1;
                    }
                }
                if (roundedCount > 0) {
                    for (int i = 0; i < roundedCount; i++)
                        grid[insertY-i][x] = Rock::Rounded;
                }
            }
            break;
        }
        case Direction::West: {
            for (int y = 0; y < grid.size(); y++) {
                int roundedCount = 0, insertX = 0;
                for (int x = 0; x < grid[y].size(); x++) {
                    if (grid[y][x] == Rock::Rounded) {
                        roundedCount++;
                        grid[y][x] = Rock::Empty;
                    }
                    if (grid[y][x] == Rock::Cube) {
                        for (int i = 0; i < roundedCount; i++)
                            grid[y][insertX+i] = Rock::Rounded;

                        roundedCount = 0;
                        insertX = x+1;
                    }
                }
                if (roundedCount > 0) {
                    for (int i = 0; i < roundedCount; i++)
                        grid[y][insertX+i] = Rock::Rounded;
                }
            }

            break;
        }
    }
}

long Platform::totalLoad() {
    long load = 0;
    long size = grid.size();
    for (int y = 0; y < grid.size(); ++y) {
        for (int x = 0; x < grid[y].size(); ++x) {
            if (grid[y][x] == Rock::Rounded) load += size - y;
        }
    }
    return load;
}

void Platform::slideCycle() {
    for (int d = (int)Direction::North; d <= (int)Direction::East; d++)
        slideDirection((Direction)d);
}

void Platform::slideCycleAmount(int n) {
    std::vector<std::vector<std::vector<Rock>>> list;
    long removedNumbers = 0;
    std::vector<std::vector<Rock>>::iterator ittr;
    for (;;) {
        slideCycle();
        auto ittr = std::find(list.begin(), list.end(), grid);
        if (ittr != list.end())
            break;
        
        list.push_back(grid);
    }
    
    int first_cycle_grid_index =
    
    int posKey = (n - );
    grid = list[posKey];
    std::cout << "Size vec: " << list.size() << ", removed: " << removedNumbers << ", pos key: " << posKey <<  std::endl;
}

std::string Platform::str() {
    std::string result;
    for (std::vector<Rock> &v : grid) {
        for (Rock &rock : v) result += RockCharTable[rock];
        result += "\n";
    }
    return result + "\n";
}

Platform getPlatform(std::ifstream &input) {
    Platform platform;
    std::string line;
    while(std::getline(input, line)) {
        std::vector<Rock> rocks;
        for (char c : line) {
            if (c == 'O') rocks.push_back(Rock::Rounded);
            if (c == '#') rocks.push_back(Rock::Cube);
            if (c == '.') rocks.push_back(Rock::Empty);
        }
        platform.grid.push_back(rocks);
    }
    return platform;
}