#include <iostream>
#include <fstream>
#include <regex>
#include <vector>
#include <functional>
#include <numeric>

const std::string symboltable ("*#+-/$@%&=");

struct Point {int x, y;};
struct InputPoint {std::string input; Point point;};

bool IsSymbol(char);
bool IsAdjacent(const InputPoint &, const InputPoint &);
std::vector<InputPoint> FindLocations(std::ifstream &, std::function<bool(char)>);
std::vector<int> GetAdjacentNumbers(std::vector<InputPoint> &, std::vector<InputPoint> &);
std::vector<int> GetGearNumbers(std::vector<InputPoint> &, std::vector<InputPoint> &);

void partOne(std::ifstream &);
void partTwo(std::ifstream &);

int main() {
    std::ifstream input ("../input.txt");
    if(!input.is_open()) return -1;

    partOne(input);
    partTwo(input);
    
    return 0;
}

void partOne(std::ifstream &input) {
    std::vector<InputPoint> numbers = FindLocations(input, isdigit);
    std::vector<InputPoint> symbols = FindLocations(input, IsSymbol);
    std::vector<int> correctNums = GetAdjacentNumbers(numbers, symbols);
    
    int sum = std::accumulate(correctNums.begin(),correctNums.end(),0);
    
    std::cout << "Answer part 1: " << sum << std::endl;
}

void partTwo(std::ifstream &input) {
    std::vector<InputPoint> numbers = FindLocations(input, isdigit);
    std::vector<InputPoint> stars = FindLocations(input, [](char c){return c == '*';});
    std::vector<int> correctNums = GetGearNumbers(numbers, stars);

    int sum = std::accumulate(correctNums.begin(),correctNums.end(),0);
    
    std::cout << "Answer part 2: " << sum << std::endl;
}

bool IsSymbol(char c) {
    for (char symbol: symboltable)
        if (symbol == c) return true;

    return false;
}

std::vector<InputPoint> FindLocations(std::ifstream &input, std::function<bool(char)> pred) {
    std::vector<InputPoint> locations;
    
    std::string line;
    for(int i = 0; getline(input, line); i++) {
        std::string s;
        for (int j = 0; j < line.length(); j++) {
            if (pred(line[j])) s += line[j];
            else if (!s.empty()) {
                locations.push_back(InputPoint{s, Point{j-(int)s.length(), i}});
                s = "";
            }
            if (line.length()-1 == j && !s.empty()) locations.push_back(InputPoint{s, Point{j-(int)s.length(), i}});
        }
    }
    input.clear();
    input.seekg(0, std::ios::beg);
    return locations;
}

bool IsAdjacent(const InputPoint &p1, const InputPoint &p2) {
    bool sideAdjacent = p1.point.y == p2.point.y &&
        (p1.point.x-1 == p2.point.x || p1.point.x + p1.input.length() == p2.point.x);
    bool topBottomAdjacent = (p1.point.y-1 == p2.point.y || p1.point.y+1 == p2.point.y) &&
         (p2.point.x >= p1.point.x-1 && p2.point.x <= p1.point.x + p1.input.length());
    return sideAdjacent || topBottomAdjacent;
}

std::vector<int> GetAdjacentNumbers(std::vector<InputPoint> &nums, std::vector<InputPoint> &sym) {
    std::vector<int> numbers;
    for (const InputPoint& numPoint : nums) {
        for(const InputPoint &symbolPoint : sym) {
            if (IsAdjacent(numPoint, symbolPoint))
                numbers.push_back(std::stoi(numPoint.input));
        }
    }
    return numbers;
}

std::vector<int> GetGearNumbers(std::vector<InputPoint> &nums, std::vector<InputPoint> &stars) {
    std::vector<int> numbers;
    for (const InputPoint &star : stars) {
        std::vector<int> starNumbers;
        for(const InputPoint &num : nums) {
            if (IsAdjacent(num, star)) starNumbers.push_back(std::stoi(num.input));
        }
        if (starNumbers.size() == 2)
            numbers.push_back(starNumbers[0] * starNumbers[1]);
    }
    return numbers;
}