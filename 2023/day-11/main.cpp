#include <iostream>
#include <fstream>
#include <vector>
#include <map>

const char galaxy = '#';
struct Point { 
    long x, y, value; 
    std::string str() const {return "Point[x: " + std::to_string(x) + ", y: " + std::to_string(y) + ", value: " + std::to_string(value) + "]";}; 
    bool operator ==(Point p) const {return x == p.x && y == p.y;};
};
struct GalaxyPair {
    Point first, second;
    long getSteps() const;
    std::string str() const {return "GalaxyPair[first: " + first.str() + ", second: " + second.str() + ", steps: " + std::to_string(getSteps()) + "]";}
};

struct Image {
    std::vector<std::vector<char>> space;
    std::map<int, long> expandedRows;
    std::map<int, long> expandedCols;
    std::vector<std::vector<char>> identifySpace() const;
    std::vector<Point> getGalaxies();
    std::vector<GalaxyPair> getGalaxyPairs();
    void expandSpace(long);
};

Image getImage(std::ifstream &);

void partOne(Image);
void partTwo(Image);
int main() {
    std::ifstream input("../input.txt");
    if (!input.is_open()) return -1;
    
    Image image = getImage(input);
    
    partOne(image);
    partTwo(image);
    
    input.close();
    return 0;
}

void partOne(Image image) {
    image.expandSpace(2);
    long sum = 0;
    for (GalaxyPair galaxyPair : image.getGalaxyPairs()) sum += galaxyPair.getSteps();
    std::cout << "Answer for part 1: " << sum << std::endl;
}

void partTwo(Image image) {
    image.expandSpace(1000000);
    long sum = 0;
    for (GalaxyPair galaxyPair : image.getGalaxyPairs()) sum += galaxyPair.getSteps();
    std::cout << "Answer for part 2: " << sum << std::endl;
}

Image getImage(std::ifstream &input) {
    Image image;
    std::string line;
    while(std::getline(input, line))
        image.space.emplace_back(line.begin(), line.end());
    return image;
}

void Image::expandSpace(long expandSize) {
    // Check for rows and insert new
    for (size_t y = space.size()-1; y > 0; y--) {
        for (size_t x = 0; x < space[y].size(); x++) {
            if (space[y][x] == galaxy) break;
            if(x == space[y].size()-1) expandedRows[y] = expandSize;
        }
    }
    // Check for cols and insert new
    for (size_t x = space.front().size()-1; x > 0; x--) {
        for (size_t y = 0; y < space.size(); y++) {
            if (space[y][x] == galaxy) break;
            if(y == space.size()-1) expandedCols[x] = expandSize;
        }
    }
}

std::vector<std::vector<char>> Image::identifySpace() const {
    std::vector<std::vector<char>> copySpace = space;
    int n = 1;
    for(std::vector<char> &v : copySpace) {
        for (char &c : v) {
            if (c == galaxy) {
                c = (char) ('0' + n);
                n++;
            }
        }
    }
    return copySpace;
}

std::vector<Point> Image::getGalaxies() {
    std::vector<Point> points;
    std::vector<std::vector<char>> identitySpace = identifySpace();
    
    long x = 0, y = 0;
    for (int i = 0; i < identitySpace.size(); i++) {
        for (int j = 0; j < identitySpace[i].size(); j++) {
            if(space[i][j] == galaxy) points.push_back(Point{x, y, identitySpace[i][j]-'0'});
            x = expandedCols[j] ? x + expandedCols[j] : x + 1;
        }
        x = 0, y = expandedRows[i] ? y + expandedRows[i] : y + 1;
    }
    return points;
}

std::vector<GalaxyPair> Image::getGalaxyPairs() {
    std::vector<GalaxyPair> pairs;
    std::vector<Point> galaxies = getGalaxies();

    for (int i = 0; i < galaxies.size()-1; i++) {
        for (int j = i+1; j < galaxies.size(); j++) {
            pairs.push_back(GalaxyPair{galaxies[i], galaxies[j]});
        }
    }
    return pairs;
}

long GalaxyPair::getSteps() const {
    return std::abs(first.x - second.x) + std::abs(first.y - second.y);
}