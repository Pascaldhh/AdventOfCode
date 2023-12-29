#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <map>
#include <algorithm>

enum class Terrain { Rock, Ash };
enum class Found { Vertical, Horizontal, None };

struct Pattern {
    std::vector<std::vector<Terrain>> terrain;
    std::vector<std::tuple<Found, int, int>> findStartPossibleMirrors();
    std::pair<std::tuple<Found, int, int>, std::tuple<int, int>> findStartMirror();
    long summarize();
};

std::vector<Pattern> getPatterns(std::ifstream &);

void partOne(std::vector<Pattern>);
void partTwo(std::vector<Pattern>);
int main() {
    std::ifstream input("../input.txt");
    if (!input.is_open()) return -1;
    
    std::vector<Pattern> patterns = getPatterns(input);
    partOne(patterns);
    partTwo(patterns);
    
    input.close();
    return 0;
}

void partOne(std::vector<Pattern> patterns) {
    int answer = 0;
    for (Pattern pattern : patterns) answer += pattern.summarize();
    std::cout << "Answer for part 1: " << answer << std::endl;
}

void partTwo(std::vector<Pattern> patterns) {

}

std::vector<std::tuple<Found, int, int>> Pattern::findStartPossibleMirrors() {
    std::vector<std::tuple<Found, int, int>> result;
    
    // Horizontal check
    std::vector<Terrain> previousPattern = terrain[0];
    for(int y = 1; y < terrain.size(); y++) {
        if (previousPattern == terrain[y]) 
            result.emplace_back(Found::Horizontal, y-1, y);
        previousPattern = terrain[y];
    }

    // Vertical check
    previousPattern = {};
    for(int x = 0; x < terrain[0].size(); x++) {
        std::vector<Terrain> tempV;
        for (int y = 0; y < terrain.size(); y++)
            tempV.push_back(terrain[y][x]);
        
        if (previousPattern == tempV)
            result.emplace_back(Found::Vertical, x-1, x);
        previousPattern = tempV;
        tempV.clear();
    }
    
    return result;
}

std::pair<std::tuple<Found, int, int>, std::tuple<int, int>> Pattern::findStartMirror() {
    std::vector<std::tuple<Found, int, int>> possibleMirrors = findStartPossibleMirrors();
    std::map<std::tuple<Found, int, int>, std::tuple<int, int>> answers;
    
    for(std::tuple<Found, int, int> tuple : possibleMirrors) {
        switch (std::get<0>(tuple)) {
            case Found::Vertical: {
                int left = std::get<1>(tuple)-1, right = std::get<2>(tuple)+1;
                int min = std::min(std::get<1>(tuple), (int)terrain[0].size() - right);
                bool found = true;
                for(int x = 0; x < min; x++) {
                    std::vector<Terrain> tempLeft;
                    for (int y = 0; y < terrain.size(); y++)
                        tempLeft.push_back(terrain[y][left - x]);

                    std::vector<Terrain> tempRight;
                    for (int y = 0; y < terrain.size(); y++)
                        tempRight.push_back(terrain[y][right + x]);
                    
                    if (tempLeft != tempRight) {
                        found = false;
                        break;
                    }
                }
                if (found) answers[tuple] = std::make_tuple(left+2, terrain[0].size() - (right-1));
                break;
            }

            case Found::Horizontal: {
                int up = std::get<1>(tuple)-1, down = std::get<2>(tuple)+1;
                int min = std::min(std::get<1>(tuple), (int)terrain.size() - down);
                bool found = true;
                for (int y = 0; y < min; y++) {
                    if (terrain[up - y] != terrain[down + y]) {
                        found = false;
                        break;
                    }
                }
                if (found) answers[tuple] = std::make_tuple(up+2, terrain.size() - (down-1));
                break;
            }
            case Found::None:
                break;
        }
    }

    if (answers.empty())
        return {std::make_tuple(Found::None, -1, -1), std::make_tuple(-1, -1)};


    return *std::max_element(answers.begin(),answers.end(),
                                  [] (const auto &a, const auto &b)->bool{ 
        return (std::get<0>(a.second) - std::get<1>(a.second)) < (std::get<0>(b.second) - std::get<1>(b.second)); 
    } );
}

long Pattern::summarize() {
    long summary = 0;
    std::pair<std::tuple<Found, int, int>, std::tuple<int, int>> found = findStartMirror();
//    std::cout << static_cast<int>(std::get<0>(found.first)) << ": " << std::get<1>(found.first) << ", " << std::get<2>(found.first) << std::endl;
//    std::cout << "Left: " << std::get<0>(found.second) << ", Right: " << std::get<1>(found.second) << std::endl;

    if (get<0>(found.first) == Found::Vertical) summary += get<0>(found.second);
    if (get<0>(found.first) == Found::Horizontal) summary += (get<0>(found.second) * 100);
    return summary;
}

std::vector<Pattern> getPatterns(std::ifstream &input) {
    std::vector<Pattern> result;
    
    std::string line;
    std::vector<std::vector<Terrain>> terrain;
    while(std::getline(input, line)) {
        if (line.empty() && !terrain.empty()) {
            result.push_back(Pattern{terrain});
            terrain.clear();
        }
        if (line.empty()) continue;
        std::vector<Terrain> row;
        for (char c : line) {
            if (c == '.') row.push_back(Terrain::Ash);
            if (c == '#') row.push_back(Terrain::Rock);
        }
        terrain.push_back(row);
    }
    if (!terrain.empty()) result.push_back(Pattern{terrain});
    
    return result;
}