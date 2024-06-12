#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>

enum class Direction { North, East, South, West };
enum class Rock { Rounded, Cube, Empty };

struct Platform {
    static std::map<Rock, char> bind;
    static Platform readFile(std::ifstream &);

    int pointsEachRow = 1;
    std::vector<std::vector<Rock>> grid;
    int move(Direction);
    int setColumn(int rockCount, int currentX, int currentY);
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
    int totalPoints = platform.move(Direction::North);
    std::cout << totalPoints << std::endl;
}

void partTwo(const Platform& platform) {

}

int Platform::setColumn(int rockCount, int currentX, int currentY) {
    int totalPoints = 0;
    for(int y = currentY; y < currentY + rockCount; y++) {
        this->grid[y][currentX] = Rock::Rounded;
        totalPoints += (grid.size() - y)*pointsEachRow;
    }
    return totalPoints;
}

int Platform::move(Direction direction) {
    if(direction != Direction::North) return -1;

    int totalPoints = 0;

    for (int x = 0; x < this->grid[0].size(); x++) {
        int rockCount = 0;
        for (int y = this->grid.size(); y-- > 0;) {
            switch (this->grid[y][x]) {
                case Rock::Rounded:
                    this->grid[y][x] = Rock::Empty;
                    rockCount++;
                    break;

                case Rock::Cube:
                    totalPoints += this->setColumn(rockCount, x, y+1);
                    rockCount = 0;
                    break;

                case Rock::Empty:
                    break;
            }
        }
        totalPoints += this->setColumn(rockCount, x, 0);
    }

    return totalPoints;
}

std::string Platform::to_string() {
    std::string s;

    for(const std::vector<Rock>& rocks : this->grid) {
        for(Rock rock : rocks) {
            s += std::to_string(static_cast<int>(rock));
        }
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