#include <iostream>
#include <fstream>
#include <vector>

const char galaxy = '#';
struct Point { 
    int x, y, value; 
    std::string str() {return "Point[x: " + std::to_string(x) + ", y: " + std::to_string(y) + ", value: " + std::to_string(value) + "]";}; 
    bool operator ==(Point p) const{return x == p.x && y == p.y;};
};
struct GalaxyPair {
    Point first, second;
    long getSteps();
    std::string str(){return "GalaxyPair[first: " + first.str() + ", second: " + second.str() + ", steps: " + std::to_string(getSteps()) + "]";}
};
struct Image {
    std::vector<std::vector<char>> space;
    std::vector<std::vector<char>> identifySpace();
    std::vector<Point> getGalaxies();
    std::vector<GalaxyPair> getGalaxyPairs();
    void expandSpace();
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
    image.expandSpace();
    long sum = 0;
    for (GalaxyPair galaxyPair : image.getGalaxyPairs()) sum += galaxyPair.getSteps();
    std::cout << "Answer for part 1: " << sum << std::endl;
}

void partTwo(Image image) {
    std::cout << "Answer for part 2: " << 1 << std::endl;
}

Image getImage(std::ifstream &input) {
    Image image;
    std::string line;
    while(std::getline(input, line))
        image.space.emplace_back(line.begin(), line.end());
    return image;
}

void Image::expandSpace() {
    // Check for rows and insert new
    for (size_t y = space.size()-1; y > 0; y--) {
        for (size_t x = 0; x < space[y].size(); x++) {
            if (space[y][x] == galaxy) break;
            if(x == space[y].size()-1) space.insert(space.begin() + y, std::vector<char>(space[y].size(), '.'));
        }
    }
    // Check for cols and insert new
    for (size_t x = space.front().size()-1; x > 0; x--) {
        for (size_t y = 0; y < space.size(); y++) {
            if (space[y][x] == galaxy) break;
            if(y == space.size()-1) {
                for (auto &v : space)
                    v.insert(v.begin() + x, '.');
            }
        }
    }
}

std::vector<std::vector<char>> Image::identifySpace() {
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

    for (int y = 0; y < identitySpace.size(); y++) {
        for (int x = 0; x < identitySpace[y].size(); x++) {
            if(space[y][x] == galaxy) points.push_back(Point{x, y, identitySpace[y][x]-'0'});
        }
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

long GalaxyPair::getSteps() {
    long steps = 0;
    Point current = first;
    
    while(current != second) {
        if(current.x < second.x) {
            current.x++;
            steps++;
        }
        if (current.x > second.x) {
            current.x--;
            steps++;
        }

        if (current.y < second.y) {
            current.y++;
            steps++;
        }
        if (current.y > second.y) {
            current.y--;
            steps++;
        }
    }
    
    return steps;
}