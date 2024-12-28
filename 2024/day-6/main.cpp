#include <iostream>
#include <utility>
#include <vector>
#include <fstream>
#include <format>
#include <map>
#include <set>

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

    Point operator+(const Point &) const;
    bool operator<(const Point &) const;
};

struct Guard {
    Dir dir;
    Point position;

    std::string str();

    static Guard parse(std::ifstream &input);

    bool operator<(const Guard &) const;
};

struct Map {
    std::vector<std::vector<char>> value;

    std::string str();

    static Map parse(std::ifstream &input);
};

class PathFinder {
    const std::map<Dir, Point> coordsMap = {
        { Dir::Top, Point{ .x = 0, .y = -1 } },
        { Dir::Right, Point{ .x = 1, .y = 0 } },
        { Dir::Bottom, Point{ .x = 0, .y = 1 } },
        { Dir::Left, Point{ .x = -1, .y = 0 } }
    };

    Map map;
    Guard guard;
    int stepsTaken;
    std::set<Guard> history = {};

public:
    PathFinder(const Map m, const Guard w) : map(m), guard(w) {}
    bool guardIsOutside() const;
    bool guardIsBlocked();
    bool guardIsLooped() const;
    Map getMap();
    void setMap(Map);
    std::set<Point> getUniquePositions();
    std::set<Guard> getHistory();
    std::string str();

    void print(bool) const;
    void step();
    void stepUntilOutside();

    static PathFinder parse(std::ifstream &input);
};

class PathSearcher {
    PathFinder startPathFinder;

public:
    explicit PathSearcher(PathFinder start_path_finder)
        : startPathFinder(std::move(start_path_finder)) {}

    PathFinder createFinder();
    int countGuardLoops();

    static PathSearcher parse(std::ifstream &input);
};

const char guardIdentifier = '^';
const char blockedIdentifier = '#';
const Dir guardStartingDir = Dir::Top;

void partOne(PathSearcher &);
void partTwo(PathSearcher &);
int main() {
    std::ifstream input("../input.txt");
    if(!input.is_open()) return -1;

    PathSearcher pathSearcher = PathSearcher::parse(input);
    input.close();

    partOne(pathSearcher);
    partTwo(pathSearcher);

    return 0;
}

void partOne(PathSearcher &pathSearcher) {
    PathFinder pathFinder = pathSearcher.createFinder();
    pathFinder.stepUntilOutside();
    std::cout << "Answer part 1: " << pathFinder.getUniquePositions().size() << std::endl;
}

void partTwo(PathSearcher &pathSearcher) {
    const int result = pathSearcher.countGuardLoops();
    std::cout << "Answer part 2: " << result << std::endl;
}

Map Map::parse(std::ifstream &input) {
    Map map;

    std::string line;
    while(std::getline(input, line)) {
        std::vector<char> value;
        for (char c : line) {
            if (c == guardIdentifier) {
                value.push_back('.');
                continue;
            }
            value.push_back(c);
        }
        map.value.push_back(value);
    }

    input.clear();
    input.seekg(0);

    return map;
}

Guard Guard::parse(std::ifstream &input) {
    Guard guard = { .dir = guardStartingDir };

    std::string line;
    for(int row = 0; std::getline(input, line); row++) {
        for(int col = 0; col < line.size(); col++) {
            if (line[col] == guardIdentifier) {
                guard.position = { .x = col, .y = row };
                return guard;
            }
        }
    }

    input.clear();
    input.seekg(0);

    return guard;
}

PathFinder PathFinder::parse(std::ifstream &input) {
    return { Map::parse(input), Guard::parse(input) };
}

PathFinder PathSearcher::createFinder() {
    return startPathFinder;
}

int PathSearcher::countGuardLoops() {
    int result = 0;

    const Map map = startPathFinder.getMap();
    for (int y = 0; y < map.value.size(); y++) {
        for (int x = 0; x < map.value[y].size(); x++) {
            if (map.value[y][x] == blockedIdentifier) continue;

            PathFinder pathFinder = createFinder();
            Map newMap = map;
            newMap.value[y][x] = blockedIdentifier;
            pathFinder.setMap(newMap);

            pathFinder.stepUntilOutside();
            if (pathFinder.guardIsLooped()) result++;
        }
    }

    return result;
}

PathSearcher PathSearcher::parse(std::ifstream &input) {
    return PathSearcher(PathFinder::parse(input) );
}

void PathFinder::print(bool withHistory) const {
    std::vector<std::vector<char>> m = map.value;

    if (!guardIsOutside()) {
        auto [x, y] = guard.position;
        m[y][x] = guardIdentifier;
    }

    if (withHistory) {
        for (auto [dir, position] : history) m[position.y][position.x] = 'X';
    }

    std::cout << "Steps: " << stepsTaken << std::endl;
    for (const std::vector<char>& row : m) {
        for (const char c : row) std::cout << c << " ";
        std::cout << std::endl;
    }
}

void PathFinder::step() {
    if (!guardIsOutside()) {
        history.insert(guard);
    }

    const Point oldPosition = guard.position;
    guard.position = coordsMap.at(guard.dir) + guard.position;

    if (!guardIsBlocked()) {
        stepsTaken++;
        return;
    }

    guard.position = oldPosition;
    guard.dir = static_cast<Dir>((static_cast<int>(guard.dir) + 1) % coordsMap.size());
}

void PathFinder::stepUntilOutside() {
    while (!guardIsOutside() && !guardIsLooped()) {
        step();
    }
}

bool PathFinder::guardIsOutside() const {
    auto [x, y] = guard.position;
    return 0 > x || x > map.value.front().size()-1
        || 0 > y || y > map.value.size()-1;
}

bool PathFinder::guardIsBlocked() {
    if (guardIsOutside()) return false;
    auto [x, y] = guard.position;
    return map.value[y][x] == blockedIdentifier;
}

bool PathFinder::guardIsLooped() const {
    return history.contains(guard);
}

Map PathFinder::getMap() {
    return map;
}

void PathFinder::setMap(Map map) {
    this->map = std::move(map);
}

std::set<Point> PathFinder::getUniquePositions() {
    std::set<Point> result;
    for (Guard guard : history) result.insert(guard.position);
    return result;
}

std::set<Guard> PathFinder::getHistory() {
    return history;
}

std::string PathFinder::str() {
    return std::format("PathFinder[{},{}]", guard.str(), map.str());
}

std::string Guard::str() {
    return std::format("guard[Dir:{},Pos:{}]", static_cast<int>(dir), position.str());
}

std::string Map::str() {
    std::string result = "Map[value:";

    for(const std::vector<char>& cVec : value) {
        result += "\n";
        for(const char c : cVec) result += c;
    }

    result += "]";
    return result;
}

std::string Point::str() {
    return std::format("Point[x:{},y:{}]", x, y);
}

Point Point::operator+(const Point &p) const {
    Point newP{};
    newP.x = x + p.x;
    newP.y = y + p.y;
    return newP;
}

bool Point::operator<(const Point &pt) const {
    return x < pt.x || (!(pt.x < x) && y < pt.y);
}

bool Guard::operator<(const Guard &guard) const {
    return position < guard.position || (!(guard.position < position) && dir < guard.dir);
}
