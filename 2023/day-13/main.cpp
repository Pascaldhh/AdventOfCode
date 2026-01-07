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
    std::vector<std::tuple<Found, int, int, bool>> findStartPossibleMirrors(const std::function<bool(std::vector<Terrain>&, std::vector<Terrain>&)>& = [](std::vector<Terrain> &one, std::vector<Terrain> &two)->bool{return false;});
    std::pair<std::tuple<Found, int, int, bool>, std::tuple<int, int>> findStartMirror(const std::function<bool(std::vector<Terrain>&, std::vector<Terrain>&)>& = [](std::vector<Terrain> &one, std::vector<Terrain> &two)->bool{return false;}, bool = false);
    long summarize(const std::function<bool(std::vector<Terrain>&, std::vector<Terrain>&)>& = [](std::vector<Terrain> &one, std::vector<Terrain> &two)->bool{return false;}, bool = false);
};

std::vector<Pattern> getPatterns(std::ifstream &);
bool hasSmug(std::vector<Terrain> &, std::vector<Terrain> &);

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
    int answer = 0;
    for (Pattern pattern : patterns) answer += pattern.summarize(hasSmug, true);
    std::cout << "Answer for part 2: " << answer << std::endl;
}

std::vector<std::tuple<Found, int, int, bool>> Pattern::findStartPossibleMirrors(const std::function<bool(std::vector<Terrain>&, std::vector<Terrain>&)>& predicate) {
    std::vector<std::tuple<Found, int, int, bool>> result;
    
    // Horizontal check
    std::vector<Terrain> previousPattern = terrain[0];
    for(int y = 1; y < terrain.size(); y++) {
        
        bool smug = predicate(previousPattern, terrain[y]);
        if (previousPattern == terrain[y] || smug)
            result.emplace_back(Found::Horizontal, y-1, y, smug);
        previousPattern = terrain[y];
    }

    // Vertical check
    previousPattern = {};
    for(int x = 0; x < terrain[0].size(); x++) {
        std::vector<Terrain> tempV;
        for (int y = 0; y < terrain.size(); y++)
            tempV.push_back(terrain[y][x]);

        bool smug = predicate(previousPattern, tempV);
        if (previousPattern == tempV || smug)
            result.emplace_back(Found::Vertical, x-1, x, smug);
        previousPattern = tempV;
        tempV.clear();
    }
    
    return result;
}

std::pair<std::tuple<Found, int, int, bool>, std::tuple<int, int>> Pattern::findStartMirror(const std::function<bool(std::vector<Terrain>&, std::vector<Terrain>&)>& predicate, bool mug) {
    std::vector<std::tuple<Found, int, int, bool>> possibleMirrors = findStartPossibleMirrors(predicate);
    std::map<std::tuple<Found, int, int, bool>, std::tuple<int, int>> answers;
    
    for(std::tuple<Found, int, int, bool> tuple : possibleMirrors) {
        bool found = true;
        switch (std::get<0>(tuple)) {
            case Found::Vertical: {
                int left = std::get<1>(tuple)-1, right = std::get<2>(tuple)+1;
                int min = std::min(std::get<1>(tuple), (int)terrain[0].size() - right);
                for(int x = 0; x < min; x++) {
                    std::vector<Terrain> tempLeft;
                    for (int y = 0; y < terrain.size(); y++)
                        tempLeft.push_back(terrain[y][left - x]);

                    std::vector<Terrain> tempRight;
                    for (int y = 0; y < terrain.size(); y++)
                        tempRight.push_back(terrain[y][right + x]);
                    
                    bool smug = predicate(tempLeft, tempRight);
                    if (!std::get<3>(tuple) && smug) std::get<3>(tuple) = smug;
                    if (tempLeft != tempRight && !smug) {
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
                for (int y = 0; y < min; y++) {
                    bool smug = predicate(terrain[up - y], terrain[down + y]);
                    if (!std::get<3>(tuple) && smug) std::get<3>(tuple) = smug;
                    if (terrain[up - y] != terrain[down + y] && !smug) {
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
        return {std::make_tuple(Found::None, -1, -1, false), std::make_tuple(-1, -1)};

    if (mug) {
        std::erase_if(answers, [](auto kvp)->bool{return !std::get<3>(kvp.first);});
    }
    
    return *std::max_element(answers.begin(),answers.end(),
                                  [] (const std::pair<std::tuple<Found, int, int, bool>, std::tuple<int, int>> &a, const std::pair<std::tuple<Found, int, int, bool>, std::tuple<int, int>> &b)->bool{ 
        return (std::get<0>(a.second) - std::get<1>(a.second)) < (std::get<0>(b.second) - std::get<1>(b.second)); 
    } );
}

long Pattern::summarize(const std::function<bool(std::vector<Terrain>&, std::vector<Terrain>&)> &predicate, bool validate) {
    long summary = 0;
    std::pair<std::tuple<Found, int, int, bool>, std::tuple<int, int>> found = findStartMirror(predicate, validate);

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

bool hasSmug(std::vector<Terrain> &tV1, std::vector<Terrain> &tV2) {
    int count = 0, index = 0;
    for (int i = 0; i < tV1.size(); i++) {
        if (tV1[i] != tV2[i]) {
            count++;
            index = i;
        }
        if (count > 1) return false;
    }
    if (count < 1) return false;
    return true;
}