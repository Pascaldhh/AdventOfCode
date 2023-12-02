#include <iostream>
#include <string>
#include <fstream>
#include <map>

int getDigit(const std::string&, bool);
int glueDigits(int, int);
void stringToDigits(std::string &s);
std::map<std::string, int> replaceTable {
    {"one", 1},
    {"two", 2},
    {"three", 3},
    {"four", 4},
    {"five", 5},
    {"six", 6},
    {"seven", 7},
    {"eight", 8},
    {"nine", 9},
};

void partOne(std::ifstream &);
void partTwo(std::ifstream &);

int main() {
    std::ifstream input("../input.txt");
    if(!input.is_open()) return -1;

    partOne(input);
    input.clear();
    input.seekg(0, std::ios::beg);
    partTwo(input);

    input.close();
    return 0;
}

void partOne(std::ifstream &input) {
    std::string line;
    int sum = 0;
    while(getline(input, line))
        sum += glueDigits(getDigit(line, true),getDigit(line, false));

    std::cout << "Answer part 1: " << sum << std::endl;
}

void partTwo(std::ifstream &input) {
    std::string line;
    int sum = 0;

    while(getline(input, line)){
        stringToDigits(line);
        sum += glueDigits(getDigit(line, true),getDigit(line, false));
    }

    std::cout << "Answer part 2: " << sum << std::endl;
}

int getDigit(const std::string& line, bool first) {
    for(size_t i = first ? 0 : line.length();
        first ? i < line.length(): i >= 0;
        first ? i++ : i--)
        if(isdigit(line[i])) return line[i] - '0';
    return 0;
}

int glueDigits(int d, int d2) {
    return std::stoi(std::to_string(d) + std::to_string(d2));
}

void stringToDigits(std::string &s) {
    std::string dstring;
    for (int i = 0; i < s.length(); i++) {
        if(isdigit(s[i])) dstring += std::to_string(s[i] - '0');
        else for (auto const &kvp : replaceTable) {
            if(s.substr(i, kvp.first.length()).find(kvp.first) != std::string::npos)
                dstring += std::to_string(kvp.second);
        }
    }
    s = dstring;
}