#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <map>
#include <sstream>

int GetId(const std::string&);
std::map<std::string, int> GetCubes(const std::string&);
bool CubesValid(std::map<std::string, int>);

const std::map<std::string, int> maxCubes {
    {"red", 12},
    {"green", 13},
    {"blue", 14}
};


void partOne(std::ifstream &);
void partTwo(std::ifstream &);

int main() {
    std::ifstream input ("../input.txt");
    if(!input.is_open()) return 0;

    partOne(input);
    input.clear();
    input.seekg(0, std::ios::beg);
    partTwo(input);

    input.close();
    return 0;
}

void partOne(std::ifstream &input) {
    std::string line;
    int idSum = 0;
    while(std::getline(input, line)){
        if(CubesValid(GetCubes(line))) idSum += GetId(line);
    }

    std::cout << "Answer part 1: " << idSum << std::endl;
}

void partTwo(std::ifstream &input) {
    std::string line;
    int sumPow = 0;
    while(std::getline(input, line)){
        std::map<std::string, int> l = GetCubes(line);
        sumPow += l["red"] * l["green"] * l["blue"];
    }

    std::cout << "Answer part 2: " << sumPow << std::endl;
}


int GetId(const std::string& s) {
    std::smatch match;
    std::regex_search(s, match, std::regex(R"(Game (\d+):)"));
    return std::stoi(match.str(1));
}

std::map<std::string, int> GetCubes(const std::string& s) {
    std::map<std::string, int> cubes {
        {"red", 0},
        {"green", 0},
        {"blue", 0}
    };

    std::string cubesstring = std::regex_replace(s, std::regex(R"(Game (\d+):)"), "");
    std::stringstream outerstream(cubesstring);

    std::string set;
    while(std::getline(outerstream, set, ';')) {
        std::stringstream innerstream(set);
        std::string cube;
        while (std::getline(innerstream, cube, ',')) {

            std::smatch match;
            std::regex_search(cube, match, std::regex(R"((\d+) (red|green|blue))"));

            if(cubes[match.str(2)] < std::stoi(match.str(1))) cubes[match.str(2)] = std::stoi(match.str(1));
        }
    }
    return cubes;
}

bool CubesValid(std::map<std::string, int> cubes) {
    for(auto const& kvp : maxCubes)
        if(cubes[kvp.first] > kvp.second) return false;

    return true;
}