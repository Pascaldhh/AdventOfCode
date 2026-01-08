#include <format>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

struct Range {
    long long from, to;

    std::vector<long long> get_invalid_ids();
    std::vector<long long> get_invalid_ids_two();
    std::string str();
    static std::vector<Range> get_all(std::ifstream &input);
};

bool is_even(const long long &input);
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
    long long sum = 0;
    for (Range &range : ranges) {
        for (long long id : range.get_invalid_ids()) {
            sum += id;
        }
    }
    std::cout << "Answer part one: " << sum << std::endl;
}

void part_two(std::vector<Range> &ranges) {
    long long sum = 0;
    for (Range &range : ranges) {
        for (long long id : range.get_invalid_ids_two()) {
            sum += id;
        }
    }
    std::cout << "Answer part two: " << sum << std::endl;
}

std::vector<long long> Range::get_invalid_ids() {
    std::vector<long long> result;

    std::string from_str = std::to_string(from);
    std::string to_str = std::to_string(to);

    if (!is_even(from_str.size()) &&
        from_str.length() == to_str.length()) return result;

    for (long long num = from; num <= to; num++) {
        std::string num_str = std::to_string(num);
        int num_length = num_str.length();

        if (!is_even(num_length)) continue;

        std::string begin = std::string(num_str.begin(), num_str.begin() + num_length / 2);
        std::string end = std::string(num_str.begin() + num_length / 2, num_str.end());
        if (begin == end) result.push_back(num);
    }

    return result;
}

std::vector<long long> Range::get_invalid_ids_two() {
    std::vector<long long> result;

    std::string from_str = std::to_string(from);
    for (long long num = from; num <= to; num++) {
        std::string num_str = std::to_string(num);

        for (int i = 1; i <= num_str.length() / 2; i++) {
            std::string first_part_num = num_str.substr(0, i);

            bool done = false;

            for (int j = i; j < num_str.length(); j += i) {
                std::string second_sub = num_str.substr(j, i);
                if (first_part_num != second_sub) break;
                if (j+i >= num_str.length()) done = true;
            }

            if (done) {
                result.push_back(num);
                break;
            }
        }
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

bool is_even(const long long &input) {
    return input % 2 == 0;
}


