#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>

enum class Direction { North, East, South, West, None };

struct Point {
    int x, y;
    char value;
    Direction direction;
    Point operator+(Point p) const { return Point{x + p.x, y+p.y};}
    bool operator==(Point p) const { return x == p.x && y == p.y;}
    std::string str() const {return "Point[x: " + std::to_string(x) + ", y: " + std::to_string(y) + ", value: " + value + ", direction: " + std::to_string((int)direction) + "]"; }
};
struct Map { 
    std::vector<std::vector<char>> grid; 
    std::vector<Point> getLoop();
    long getCountEnclosedTiles();
    long getStepsToEnd();
    char convertStartValue(Point, std::vector<Point>);
};

std::map<char, std::vector<Direction>> PipeTable {
    {'S', std::vector<Direction>{Direction::North, Direction::East, Direction::South, Direction::West}},
    {'|', std::vector<Direction>{Direction::North, Direction::South}},
    {'-', std::vector<Direction>{Direction::East, Direction::West}},
    {'L', std::vector<Direction>{Direction::North, Direction::East}},
    {'J', std::vector<Direction>{Direction::North, Direction::West}},
    {'7', std::vector<Direction>{Direction::South, Direction::West}},
    {'F', std::vector<Direction>{Direction::South, Direction::East}},
    {'.', std::vector<Direction>{Direction::None}}
};

std::map<Direction, Direction> ReflectionTable {
    {Direction::North, Direction::South},
    {Direction::East, Direction::West},
    {Direction::South, Direction::North},
    {Direction::West, Direction::East},
};

std::map<Direction, Point> DirectionCalibration {
    {Direction::North, Point{0, -1}},
    {Direction::South, Point{0, 1}},
    {Direction::East, Point{1, 0}},
    {Direction::West, Point{-1, 0}},
};

Map getMap(std::ifstream &);

void partOne(Map);
void partTwo(Map);
int main() {
    std::ifstream input("../input.txt");
    if (!input.is_open()) return -1;
    
    Map map = getMap(input);

    partOne(map);
    partTwo(map);
    
    input.close();
    return 0;
}

void partOne(Map map) {
    std::cout << "Answer for part 1: " << map.getStepsToEnd() << std::endl;
}

void partTwo(Map map) {
    std::cout << "Answer for part 2: " << map.getCountEnclosedTiles() << std::endl;
}

Map getMap(std::ifstream &input) {
    Map map;
    std::string line;
    while(std::getline(input, line))
        map.grid.emplace_back(line.begin(), line.end());
    return map;
}

Point findStart(Map map, char c) {
    for (int y = 0; y < map.grid.size(); y++) {
        for (int x = 0; x < map.grid[y].size(); x++) {
            if (map.grid[y][x] == c) return Point{x, y, map.grid[y][x]};
        }
    }
    return Point{-1, -1};
}

std::vector<Point> findPipes(Map map, Point point) {
    std::vector<Point> sidesOfLoop;
    for (Direction direction : PipeTable[point.value]) {
        Point currentPoint = point + DirectionCalibration[direction];
        if (0 > currentPoint.x || currentPoint.x > map.grid[0].size() || 0 > currentPoint.y || currentPoint.y > map.grid.size()) continue;
        currentPoint.value = map.grid[currentPoint.y][currentPoint.x];

        
        std::vector<Direction> currentDirections = PipeTable[currentPoint.value];
        std::erase(currentDirections, ReflectionTable[direction]);
        currentPoint.direction = currentDirections.front();
        
        if (std::any_of(PipeTable[currentPoint.value].begin(), PipeTable[currentPoint.value].end(),
                        [direction](Direction d){ return d == ReflectionTable[direction];})) {
            sidesOfLoop.push_back(currentPoint);
        }
    }
    return sidesOfLoop;
}

Point getNextPipe(Map map, Point point) {
    Point nextPoint = point + DirectionCalibration[point.direction];
    nextPoint.value = map.grid[nextPoint.y][nextPoint.x];
    std::vector<Direction> currentDirections = PipeTable[nextPoint.value];
    std::erase(currentDirections, ReflectionTable[point.direction]);
    nextPoint.direction = currentDirections.front();
    return nextPoint;
}

char Map::convertStartValue(Point start, std::vector<Point> foundedPoints) {
    char c;
    std::vector<Direction> directions;
    for (Point foundP : foundedPoints) {
        if (foundP.x > start.x && foundP.y == start.y)  directions.push_back(Direction::East);
        if (foundP.x < start.x && foundP.y == start.y)  directions.push_back(Direction::West);
        if (foundP.x == start.x && foundP.y > start.y)  directions.push_back(Direction::South);
        if (foundP.x == start.x && foundP.y < start.y)  directions.push_back(Direction::North);
    }
    
    std::sort(directions.begin(), directions.end());
    auto ittr = std::find_if(PipeTable.begin(), PipeTable.end(), [directions](auto kvp){
        std::sort(kvp.second.begin(), kvp.second.end());
        return kvp.second == directions;
    });
    return ittr->first;
}
std::vector<Point> Map::getLoop() {
    std::vector<Point> loop;
    Point start = findStart(*this, 'S');

    std::vector<Point> sidesOfLoop = findPipes(*this, start);
    start.value = convertStartValue(start, sidesOfLoop);
    loop.push_back(start);
    
    while(std::adjacent_find( sidesOfLoop.begin(), sidesOfLoop.end(), std::not_equal_to<>()) != sidesOfLoop.end()) {
        for (Point &point : sidesOfLoop) {
            loop.push_back(point);
            point = getNextPipe(*this, point);
        }
    }
    loop.push_back(sidesOfLoop.front());
    
    return loop;
}

long Map::getStepsToEnd() {
    std::vector<Point> loop = getLoop();
    int steps = 0;
    for (int i = 0; i < loop.size(); i+=2) steps++;
    return steps;
}

long Map::getCountEnclosedTiles() {
    std::map<char, std::vector<Direction>> pipeTable = PipeTable;
    pipeTable.erase('.');
    pipeTable.erase('7');
    pipeTable.erase('-');
    pipeTable.erase('F');
    
    std::vector<Point> loop = getLoop();
    int countOfTiles = 0;
    
    for (int y = 0; y < grid.size(); y++) {
        bool isInside = false;
        for (int x = 0; x < grid[y].size(); x++) {
            if (isInside && !std::any_of(loop.begin(), loop.end(), [x, y](Point p){return p.x == x && p.y == y;})) {
                countOfTiles++;
            }
            if (!std::any_of(loop.begin(), loop.end(), [x, y](Point p){return p.x == x && p.y == y && (p.value != '7' && p.value != '-' && p.value != 'F');})) continue;
            for (auto &kvp : pipeTable) {
                if (kvp.first != grid[y][x]) continue;
                isInside = !isInside;
                break;
            }
        }
    }
    return countOfTiles;
}