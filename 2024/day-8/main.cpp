#include <iostream>
#include <fstream>
#include <set>

struct Point {
    int x, y;

    std::string str();

    bool operator<(const Point &) const;
    Point operator-(const Point &) const;
    Point operator+(const Point &) const;
};

struct Map {
    std::vector<std::vector<char>> value;

    bool isInside(Point);
    Point searchNext(Point, Point);
    std::set<Point> searchPath(Point, bool);
    std::set<Point> calcAntinodes(bool);
    std::string str();

    void print();

    static Map parse(std::ifstream &);
};

const char emptySpots = '.';

void partOne(Map &);
void partTwo(Map &);
int main() {
    std::ifstream input("../input.txt");
    if (!input.is_open()) return -1;

    Map map = Map::parse(input);
    input.close();

    partOne(map);
    partTwo(map);

    return 0;
}

void partOne(Map &map) {
    std::set<Point> antiNodes = map.calcAntinodes(false);
    std::cout << "Answer part 1: " << antiNodes.size() << std::endl;
}


void partTwo(Map &map) {
    std::set<Point> antiNodes = map.calcAntinodes(true);
    std::cout << "Answer part 2: " << antiNodes.size() << std::endl;
}

bool Map::isInside(Point point) {
    auto [x, y] = point;
    return 0 <= x && x <= value.front().size()-1
        && 0 <= y && y <= value.size()-1;
}

Point Map::searchNext(Point position, Point dist) {
    return position + dist;
}


std::set<Point> Map::searchPath(Point position, bool anyGridPosition) {
    std::set<Point> result;

    auto [cX, cY] = position;
    char searchValue = value[cY][cX];

    for (int y = 0; y < value.size(); y++) {
        for (int x = 0; x < value[y].size(); x++) {
            char currentChar = value[y][x];
            if ((cX == x && cY == y) || currentChar != searchValue) {
                continue;
            }

            Point foundPosition = { .x = x, .y = y };
            if (anyGridPosition) result.insert(foundPosition);

            Point distance = position - foundPosition;
            Point anitNode = searchNext(foundPosition, distance);

            while (isInside(anitNode)) {
                result.insert(anitNode);

                if (!anyGridPosition) break;
                anitNode = searchNext(anitNode, distance);
            }
        }
    }

    return result;
}

std::set<Point> Map::calcAntinodes(bool anyGridPosition) {
    std::set<Point> points;

    for (int y = 0; y < value.size(); y++) {
        for (int x = 0; x < value[y].size(); x++) {
            char currentChar = value[y][x];
            if (currentChar == emptySpots) continue;

            std::set<Point> newAntiNodes = searchPath(Point { .x = x, .y = y }, anyGridPosition);
            points.insert(newAntiNodes.begin(), newAntiNodes.end());
        }
    }

    return points;
}

std::string Point::str() {
    return std::format("Point[x:{},y:{}]", x, y);
}

std::string Map::str() {
    std::string result;

    for (const std::vector<char>& charVec : value) {
        for (char c : charVec) {
            result += std::format("{} ", c);
        }
        result.pop_back();
        result += "\n";
    }
    result.pop_back();

    return result;
}

void Map::print() {
    std::cout << str() << std::endl;
}

Map Map::parse(std::ifstream &input) {
    Map map;

    std::string line;
    while (std::getline(input, line)) {
        std::vector<char> chars;
        for (char c : line) {
            chars.push_back(c);
        }
        map.value.push_back(chars);
    }

    return map;
}

bool Point::operator<(const Point &p) const {
    return x < p.x || (!(p.x < x) && y < p.y);
}

Point Point::operator-(const Point &p) const {
    return Point{ .x = p.x - x, .y = p.y - y };
}

Point Point::operator+(const Point &p) const {
    return Point{ .x = p.x + x, .y = p.y + y };
}
