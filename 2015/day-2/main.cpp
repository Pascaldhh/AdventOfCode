#include <iostream>
#include <fstream>
#include <regex>
#include <algorithm>

struct Box {
    int length, width, height;
};

Box createBox(const std::string&);
int calculateWrappingPaper(Box);
int calculateRibbon(Box);
int getSmallestSide(Box);


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
    int totalWrappingPaper = 0;
    
    std::string line;
    while(getline(input, line)) {
        totalWrappingPaper += calculateWrappingPaper(createBox(line));
    }
    std::cout << "Answer part 1: " << totalWrappingPaper << std::endl;
}

void partTwo(std::ifstream &input) {
    int totalRibbon = 0;
    
    std::string line;
    while(getline(input, line)) {
        totalRibbon += calculateRibbon(createBox(line));
        std::cout << calculateRibbon(createBox(line)) << std::endl;
    }
    
    std::cout << "Answer part 2: " << totalRibbon << std::endl;
    
}

Box createBox(const std::string &line) {
    std::smatch match;
    std::regex_search(line, match, std::regex(R"((\d+)x(\d+)x(\d+))"));

    return Box {
std::stoi(match.str(1)),
std::stoi(match.str(2)),
std::stoi(match.str(3))
    };
}

int getSmallestSide(Box box) {
    return std::min({box.length*box.width, box.width*box.height, box.height*box.length});
}

int calculateWrappingPaper(Box box) {
    return (2*box.length*box.width + 2*box.width*box.height + 2*box.height*box.length) + getSmallestSide(box);
}

int calculateRibbon(Box box) {
    int arr[]{box.length, box.width, box.height};
    std::sort(arr, arr + sizeof(arr) / sizeof(int));
    return (arr[0]*2 + arr[1]*2) + (box.length * box.width * box.height);
}