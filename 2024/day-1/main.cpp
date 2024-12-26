#include <iostream>
#include <fstream>
#include <vector>
#include <regex>
#include <algorithm>

std::vector<long> countNumbers(std::vector<std::vector<long>>);
std::vector<long> calcDistances(std::vector<std::vector<long>>);
void sortLists(std::vector<std::vector<long>>&);
std::vector<std::vector<long>> getLists(std::ifstream &);
void partOne(std::vector<std::vector<long>>);
void partTwo(std::vector<std::vector<long>>);

int main() {
    std::ifstream input("../input.txt");
    if (!input.is_open()) return -1;

    std::vector<std::vector<long>> lists = getLists(input);

    sortLists(lists);

    partOne(lists);
    partTwo(lists);

    return 0;
}

void partOne(std::vector<std::vector<long>> lists) {
    std::vector<long> dists = calcDistances(lists);

    std::cout << "Answer part 1: " << std::reduce(dists.begin(),dists.end()) << std::endl;
}

void partTwo(std::vector<std::vector<long>> lists) {
    std::vector<long> counts = countNumbers(lists);
    std::cout << "Answer part 2: " << std::reduce(counts.begin(),counts.end()) << std::endl;
}

std::vector<long> countNumbers(std::vector<std::vector<long>> lists) {
    std::vector<long> result;

    std::vector<long> left = lists.front();
    std::vector<long> right = lists.back();

    for(long num : left) {
        long count = std::count(right.begin(), right.end(), num);

        result.push_back(num * count);
    }

    return result;
}

std::vector<long> calcDistances(std::vector<std::vector<long>> lists) {
    std::vector<long> result;

    std::vector<long> left = lists.front();
    std::vector<long> right = lists.back();

    for(long i = 0; i < std::min(left.size(), right.size()); i++) {
        result.push_back(std::abs(left[i] - right[i]));
    }

    return result;
}

void sortLists(std::vector<std::vector<long>> &lists) {
    for(std::vector<long> &list : lists) {
        std::sort(list.begin(), list.end());
    }
}

std::vector<std::vector<long>> getLists(std::ifstream &input) {
    std::vector<std::vector<long>> results;

    std::vector<long> leftList;
    std::vector<long> rightList;

    std::string line;
    while(std::getline(input, line)) {
        std::smatch match;
        std::regex_search(line, match, std::regex(R"((\d+)\s+(\d+))"));
        leftList.push_back(std::stoi(match.str(1)));
        rightList.push_back(std::stoi(match.str(2)));
    }

    results.push_back(leftList);
    results.push_back(rightList);

    return results;
}