#include <iostream>
#include <fstream>

void partOne(std::ifstream &);
void partTwo(std::ifstream &);

int main() {
    std::ifstream input ("../input.txt");
    if (!input.is_open()) return -1;

    partOne(input);

    input.clear();
    input.seekg(0, std::ios::beg);
    
    partTwo(input);
    
    return 0;
}

void partOne(std::ifstream &input) {
    int floor = 0;
    std::string line;
    while(getline(input, line)) {
        for (char c : line) floor += c == '(' ? 1 : (c == ')' ? -1 : 0);
    }
    std::cout << "Answer part 1: " << floor << std::endl;
}

void partTwo(std::ifstream &input) {
    int floor = 0, answer;
    std::string line;
    while(getline(input, line)) {
        for (int i = 0; i < line.length(); i++) {
            floor += line[i] == '(' ? 1 : (line[i] == ')' ? -1 : 0);
            if (floor == -1) {
                answer = i+1;
                break;
            }
        }
    }
    
    std::cout << "Answer part 2: " << answer << std::endl;
}