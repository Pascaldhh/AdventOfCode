#include <iostream>
#include <fstream>
#include <sstream>
#include <functional>
#include <map>
#include <algorithm>
#include <vector>

struct Line {
    std::vector<int> value;
    bool isSafe(int = 3, int = 0);
    static Line parse(std::string &);
    std::vector<int> getDist();

    std::string str(int = 3, int = 0);
};

struct Lines {
    std::vector<Line> value;
    int countIsSafe(int = 3, int = 0);
    static Lines parse(std::vector<std::string> &);
    static Lines parse(std::ifstream &);

    std::string str(int = 3, int = 0);
};

void partOne(Lines);
void partTwo(Lines);
int main() {
    std::ifstream input("../input.txt");
    if(!input.is_open()) return -1;

    Lines lines = Lines::parse(input);

    partOne(lines);
    partTwo(lines);

    input.close();
    return 0;
}

void partOne(Lines lines) {
    std::cout << "Answer part 1: " << lines.countIsSafe(3) << std::endl;
}

void partTwo(Lines lines) {
    std::cout << "Answer part 2: " << lines.countIsSafe(3, 1) << std::endl;
}

std::vector<int> Line::getDist() {
    std::vector<int> dists;

    int oldNumber = value.front();
    for(int i = 1; i < value.size(); i++) {
        dists.push_back(value[i] - oldNumber);
        oldNumber = value[i];
    }

    return dists;
}

bool Line::isSafe(int threshold, int tolerateFaulty) {
    std::vector<int> dists = getDist();

    bool positive = std::all_of(dists.begin(), dists.end(), [](int n){ return n > 0; });
    bool negative = std::all_of(dists.begin(), dists.end(), [](int n){ return n < 0; });

    bool inRange = std::all_of(dists.begin(), dists.end(), [threshold](int n) { return std::abs(n) <= threshold; });

    if((positive || negative) && inRange) return true;

    if(tolerateFaulty > 0) {
        for(int i = 0; i <= dists.size(); i++) {
            std::vector<int> newValue = std::vector<int>(value);
            newValue.erase(newValue.begin() + i);
            Line newLine = Line{ .value =  newValue};
            if(newLine.isSafe(threshold, tolerateFaulty-1)) {
                return true;
            }
        }
    }

    return false;
}

int Lines::countIsSafe(int threshold, int tolerateFaulty) {
    int n = 0;
    for(Line line : value) {
        n += line.isSafe(threshold, tolerateFaulty);
    }
    return n;
}

Line Line::parse(std::string &line) {
    Line result;

    std::string item;
    std::stringstream ss(line);
    while(std::getline(ss, item, ' ')) {
        result.value.push_back(std::stoi(item));
    }

    return result;
}

std::string Line::str(int threshold, int tolerateFaulty) {
    std::string result = "Line [value:'";
    std::string divider = ", ";
    for(int n : value) result += std::format("{}{}", n, divider);
    result = result.substr(0, result.size()-divider.size());
    result += std::format("', isSafe:{}]", isSafe(threshold, tolerateFaulty));
    return result;
}

std::string Lines::str(int threshold, int tolerateFaulty) {
    std::string result;
    for(Line line : value) result += std::format("{}\n",line.str(threshold, tolerateFaulty));

    return result;
}

Lines Lines::parse(std::vector<std::string> &lines) {
    Lines result;
    for(std::string line : lines)
        result.value.push_back(Line::parse(line));

    return result;
}

Lines Lines::parse(std::ifstream &lines) {
    std::vector<std::string> stringLines;

    std::string line;
    while(std::getline(lines, line)) stringLines.push_back(line);

    return Lines::parse(stringLines);
}