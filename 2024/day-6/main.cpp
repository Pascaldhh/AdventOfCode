#include <iostream>
#include <vector>
#include <fstream>
#include <format>

enum class Dir {
    Top,
    Right,
    Bottom,
    Left
};

struct Point {
    int x;
    int y;

    std::string str();
};

struct Walker {
    Dir dir;
    Point position;

    std::string str();

    static Walker parse(std::ifstream &input);
};

struct Map {
    std::vector<std::vector<char>> value;

    std::string str();

    static Map parse(std::ifstream &input);
};

class PathFinder {
public:
    Map map;
    Walker walker;

    PathFinder(Map map, Walker walker) : map(std::move(map)), walker(walker) {}
    std::string str();

    static PathFinder parse(std::ifstream &input);
};

const char walkerIdentifier = '^';
const Dir walkerStartingDir = Dir::Top;

void partOne();
void partTwo();
int main() {
    std::ifstream input("../input.txt");
    if(!input.is_open()) return -1;

    PathFinder pathFinder = PathFinder::parse(input);
    
    partOne();
    partTwo();

    return 0;
}

void partOne() {

}

void partTwo() {

}

Map Map::parse(std::ifstream &input) {
    std::string line;
    while(std::getline(input, line)) {

    }
    return {};
}

Walker Walker::parse(std::ifstream &input) {
    Walker walker = { .dir = walkerStartingDir };

    std::string line;
    for(int row = 0; std::getline(input, line); row++) {
        for(int col = 0; col < line.size(); col++) {
            if (line[col] == walkerIdentifier) {
                walker.position = { .x = col, .y = row };
                return walker;
            }
        }
    }
    return walker;
}

PathFinder PathFinder::parse(std::ifstream &input) {
    return {Map::parse(input), Walker::parse(input) };
}

std::string PathFinder::str() {
    return std::format("PathFinder[Walker:{},Map:{}]", walker.str(), map.str());
}

std::string Walker::str() {
    return std::format("Walker[Dir:{},Pos:{}]", static_cast<int>(dir), position.str());
}

std::string Map::str() {
    std::string result = "Map[value:\n";

    for(const std::vector<char>& cVec : value) {
        for(char c : cVec) result += c;
        result += "\n";
    }

    result += "]";
    return result;
}

std::string Point::str() {
    return std::format("Point[x:{},y:{}]", x, y);
}