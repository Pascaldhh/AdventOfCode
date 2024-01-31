#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <regex>

struct Race { long time, distance; };

Race getRace(std::vector<Race> races);
std::vector<Race> getRaces(std::ifstream &);
std::vector<long> waysToWin(std::vector<Race>);

void partOne(std::vector<Race>);
void partTwo(std::vector<Race>);
int main() {
    std::ifstream input("../input.txt");
    if (!input.is_open()) return -1;

    std::vector<Race> races = getRaces(input);
    
//    for(Race race : races) std::cout << race.distance << " " << race.time << std::endl;
    partOne(races);
    partTwo(races);
    
    input.close();
    return 0;
}

void partOne(std::vector<Race> races) {
    std::vector<long> calc = waysToWin(races);
    
    long f = calc[0];
    for(int i = 1; i < calc.size(); i++) f = f * calc[i];
    
    std::cout << "Answer part 1: " << f << std::endl;
}

void partTwo(std::vector<Race> races) {
    Race race = getRace(races);
    long result = waysToWin({race})[0];
    
    std::cout << "Answer part 2: " << result << std::endl;
}
Race getRace(std::vector<Race> races) {
    std::string time;
    std::string distance;
    
    for (Race race : races) {
        time += std::to_string(race.time);
        distance += std::to_string(race.distance);
    }
    
    return {std::stol(time), std::stol(distance)};
}
std::vector<Race> getRaces(std::ifstream &input) {
    std::vector<Race> races;
    std::vector<long> times;
    std::vector<long> distances;

    std::string line;
    for(int i = 0; getline(input, line); i++) {
        std::string replaced = regex_replace(line, std::regex(R"(\w+:\s+)"), "");
        std::string number;
        std::stringstream ss(replaced);
        while(getline(ss, number, ' ')) {
            if (number.empty()) continue;
            if(i == 0) times.push_back(std::stoi(number));
            if(i == 1) distances.push_back(std::stoi(number));
        }
    }

    races.reserve(times.size());
    for (int i = 0; i < times.size(); i++)
        races.push_back(Race{times[i], distances[i]});
    
    input.clear();
    input.seekg(0, std::ios::beg);
    
    return races;
}

std::vector<long> waysToWin(std::vector<Race> races) {
    std::vector<long> calc;
    for (Race race : races) {
        std::vector<long> winWays;
        for (int i = 0; i <= race.time; i++) {
            long distance = (i * (race.time - i));
            if (distance > race.distance) winWays.push_back(distance);
        }
        calc.push_back(winWays.size());
    }
    return calc;
}