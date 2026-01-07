#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>


enum class Direction { North, East, South, West };
enum class Rock { Rounded, Cube, Empty };

using IterFunc = std::function<void(std::function<void(int)>)>;

struct Platform {
    static std::map<Rock, char> bind;
    static Platform readFile(std::ifstream &);

    const int pointsEachRow = 1;
    std::vector<std::vector<Rock>> grid;
    int move(Direction);
    int setToDirection(Direction direction, int rockCount, int currentX, int currentY);
    int setRock(int x, int y);
    int test(IterFunc iterFirst, IterFunc iterSecond);
    std::string to_string();
};

std::map<Rock, char> Platform::bind {
    { Rock::Rounded, 'O' },
    { Rock::Cube, '#' },
    { Rock::Empty, '.' }
};

void partOne(Platform);
void partTwo(const Platform&);

int main() {
    std::ifstream input("../input.txt");

    if(!input.is_open()) return -1;

    Platform platform = Platform::readFile(input);

    partOne(platform);
    partTwo(platform);

    input.close();
    return 0;
}

void partOne(Platform platform) {
    int totalPoints = platform.move(Direction::South);
    std::cout << totalPoints << std::endl;
    std::cout << platform.to_string() << std::endl;
}

void partTwo(const Platform& platform) {

}

int Platform::setRock(int x, int y) {
    this->grid[y][x] = Rock::Rounded;
    return (grid.size()-y) * pointsEachRow;
}

int Platform::setToDirection(Direction direction, int rockCount, int currentX, int currentY) {
    int totalPoints = 0;

    switch (direction) {
        case Direction::North:
            std::cout << currentY << std::endl;
            for(int y = currentY; y < currentY+1 + rockCount; y++) {
                std::cout << rockCount << ": " << y << std::endl;

                totalPoints += setRock(currentX, y);
            }
            break;
        case Direction::East:
            break;
        case Direction::South:
            std::cout << currentY << " - " << rockCount << std::endl;

            for(int y = currentY; y > currentY-1-rockCount; y--) {
                std::cout << rockCount << ": " << y << std::endl;
                totalPoints += setRock(currentX, y);
            }
            break;
        case Direction::West:
            break;
    }

    return totalPoints;
}

int Platform::move(Direction direction, IterFunc iterX, IterFunc iterY) {
    int totalPoints = 0;
    auto commonLogic = [&](int x, int y, int &rockCount) {
        switch (this->grid[y][x]) {
            case Rock::Rounded:
                this->grid[y][x] = Rock::Empty;
                rockCount++;
                break;

            case Rock::Cube:
                totalPoints += this->setToDirection(direction, rockCount, x, y);
                rockCount = 0;
                break;

            case Rock::Empty: break;
        }
    };

//    IterFunc iterX;
//    IterFunc iterY;
//
//    switch (direction) {
//        case Direction::North:
//
//            break;
//    }

    switch (direction) {
        case Direction::North:
        case Direction::South:
            iterX([&](int x) {
                int rockCount = 0;
                int latestY;
                iterY([&](int y) {
                    commonLogic(x, y, rockCount);
                    latestY = y;
                });
                totalPoints += this->setToDirection(direction, rockCount, x,  latestY);
            });
            break;

        case Direction::East:
        case Direction::West:
            iterY([&](int y) {
                int rockCount = 0;
                int latestX;
                iterX([&](int x) {
                    commonLogic(x, y, rockCount);
                    latestX = x;
                });
                totalPoints += this->setToDirection(direction, rockCount, latestX,  y);
            });
            break;
    }

    return totalPoints;
}

/// NORTH
//int Platform::move(Direction direction) {
//    if(direction != Direction::North) return -1;
//
//    int totalPoints = 0;
//
//    for (int x = 0; x < this->grid[0].size(); x++) {
//        int rockCount = 0;
//        for (int y = this->grid.size(); y-- > 0;) {
//            switch (this->grid[y][x]) {
//                case Rock::Rounded:
//                    this->grid[y][x] = Rock::Empty;
//                    rockCount++;
//                    break;
//
//                case Rock::Cube:
//                    totalPoints += this->setToDirection(Direction::North, rockCount, x, y+1);
//                    rockCount = 0;
//                    break;
//
//                case Rock::Empty: break;
//            }
//        }
//        totalPoints += this->setToDirection(Direction::North, rockCount, x, 0);
//    }
//
//    return totalPoints;
//}

/// SOUTH
//int Platform::move(Direction direction) {
//    if(direction != Direction::South) return -1;
//
//    int totalPoints = 0;
//
//    for (int x = 0; x < this->grid[0].size(); x++) {
//        int rockCount = 0;
//        for (int y = 0; y < this->grid.size(); y++) {
//            switch (this->grid[y][x]) {
//                case Rock::Rounded:
//                    this->grid[y][x] = Rock::Empty;
//                    rockCount++;
//                    break;
//
//                case Rock::Cube:
//                    totalPoints += this->setToDirection(direction, rockCount, x, y-1);
//                    rockCount = 0;
//                    break;
//
//                case Rock::Empty: break;
//            }
//        }
//        totalPoints += this->setToDirection(direction, rockCount, x, this->grid.size() - 1);
//    }
//
//    return totalPoints;
//}

/// EAST
//int Platform::move(Direction direction) {
//    if(direction != Direction::East) return -1;
//
//    int totalPoints = 0;
//
//    for (int y = 0; y < this->grid[0].size(); y++) {
//        int rockCount = 0;
//        for (int x = 0; x < this->grid.size(); x++) {
//            switch (this->grid[y][x]) {
//                case Rock::Rounded:
//                    this->grid[y][x] = Rock::Empty;
//                    rockCount++;
//                    break;
//
//                case Rock::Cube:
//                    totalPoints += this->setToDirection(direction, rockCount, x, y-1);
//                    rockCount = 0;
//                    break;
//
//                case Rock::Empty: break;
//            }
//        }
//        totalPoints += this->setToDirection(direction, rockCount, this->grid[0].size(), y);
//    }
//
//    return totalPoints;
//}

std::string Platform::to_string() {
    std::string s;

    for(const std::vector<Rock>& rocks : this->grid) {
        for(Rock rock : rocks)
            s += std::to_string(static_cast<int>(rock));
        
        s += "\n";
    }

    return s;
}

Platform Platform::readFile(std::ifstream &input) {
    Platform platform;

    std::string line;
    while(std::getline(input, line)) {
        std::vector<Rock> rocks;
        for(char lineChar : line) {
            auto result = std::find_if(Platform::bind.begin(), Platform::bind.end(), [lineChar](const std::pair<Rock, char>& pair) {
                return lineChar == pair.second;
            });
            rocks.push_back(result != Platform::bind.end() ? result->first : Rock::Empty);
        }
        platform.grid.push_back(rocks);
    }

    return platform;
}