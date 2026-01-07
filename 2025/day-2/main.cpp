#include <format>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

struct Range {
    long from, to;

    std::vector<int> get_invalid_ids();
    std::string str();
    static std::vector<Range> get_all(std::ifstream &input);
};

bool is_even(const int &input);
void part_one(std::vector<Range> &ranges);
void part_two(std::vector<Range> &ranges);

int main() {
    std::ifstream input("../input.txt");

    if (input.fail()) return -1;

    std::vector<Range> ranges = Range::get_all(input);

    part_one(ranges);
    part_two(ranges);

    input.close();

    return 0;
}

void part_one(std::vector<Range> &ranges) {
    int sum = 0;
    for (Range &range : ranges) {
        for (int id : range.get_invalid_ids()) {
            sum += id;
        }
    }
    std::cout << "Answer part one: " << sum << std::endl;
}

void part_two(std::vector<Range> &ranges) {
    std::cout << "Answer part two: " << std::endl;
}

std::vector<int> Range::get_invalid_ids() {
    std::vector<int> result;

    std::string from_str = std::to_string(from);
    std::string to_str = std::to_string(to);

    if (!is_even(from_str.size()) &&
        from_str.length() == to_str.length()) return result;

    for (int num = from; num <= to; num++) {
        std::string num_str = std::to_string(num);
        int num_length = num_str.length();

        if (!is_even(num_length)) continue;

        std::string begin = std::string(num_str.begin(), num_str.begin() + num_length / 2);
        std::string end = std::string(num_str.begin() + num_length / 2, num_str.end());
        if (begin == end) result.push_back(num);
    }

    return result;
}

std::vector<Range> Range::get_all(std::ifstream &input) {
    std::vector<Range> result;

    std::string line;
    while (std::getline(input, line, ',')) {
        std::istringstream ss(line);
        Range range(0 ,0);

        ss >> range.from;
        ss.seekg(static_cast<int>(ss.tellg()) + 1);
        ss >> range.to;

        result.push_back(range);
    }

    return result;
}

std::string Range::str() {
    return std::format("Range[from={},to={}]", from, to);
}

bool is_even(const int &input) {
    return input % 2 == 0;
}


