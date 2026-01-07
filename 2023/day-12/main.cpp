#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
enum class Spring {
    Operational,
    Damaged,
    Unknown
};

struct Record {
    std::vector<Spring> springs;
    std::vector<int> numbers;
    static std::map<std::tuple<std::vector<Spring>, std::vector<int>>, long> cache;
    long countVariations(std::vector<Spring>, std::vector<int>);
    void unfold(int n);
    std::string str();
};
struct Records { std::vector<Record> list; };
Records getRecords(std::ifstream &);

void partOne(Records);
void partTwo(Records);
int main() {
    std::ifstream input("../input.txt");
    if (!input.is_open()) return -1;

    Records records = getRecords(input);
    
    partOne(records);
    partTwo(records);

    input.close();
    return 0;
}

void partOne(Records records) {
    long sum = 0;
    for (Record record : records.list)
        sum += record.countVariations(record.springs, record.numbers);
    
    std::cout << "Answer part 1: " << sum << std::endl;
}
void partTwo(Records records) {
    long sum = 0;
    int i = 0;
    for (Record record : records.list) {
        record.unfold(4);
        sum += record.countVariations(record.springs, record.numbers);
    }

    std::cout << "Answer part 2: " << sum << std::endl;
}

std::string Record::str() {
    std::string string = "Record[springs: [";
    for (int i = 0; i < springs.size(); i++) {
        string += std::to_string(static_cast<int>(springs[i]));
        if (i != springs.size()-1) string += ',';
    }
    string += "], numbers: [";
    for (int i = 0; i < numbers.size(); i++) {
        string += std::to_string(numbers[i]);
        if (i != numbers.size()-1) string += ',';
    }
    return string + "], variations: " + std::to_string(countVariations(springs, numbers)) + "]";
}

void Record::unfold(int n) {
    std::vector<Spring> sps = springs;
    std::vector<int> nums = numbers;
    
    for (int i = 0; i < n; i++) {
        springs.push_back(Spring::Unknown);
        springs.insert(springs.end(), sps.begin(), sps.end());
        numbers.insert(numbers.end(), nums.begin(), nums.end());
    }
}

std::map<std::tuple<std::vector<Spring>, std::vector<int>>, long> Record::cache;
long Record::countVariations(std::vector<Spring> sprngs, std::vector<int> nums) {
    if (sprngs.empty()) return nums.empty() ? 1 : 0;
    if (nums.empty()) return std::find(sprngs.begin(), sprngs.end(), Spring::Damaged) != sprngs.end() ? 0 : 1;

    std::tuple<std::vector<Spring>, std::vector<int>> key = std::make_tuple(sprngs, nums);
    if (Record::cache.find(key) != Record::cache.end())
        return Record::cache[key];

    long count = 0;
    if (sprngs[0] == Spring::Unknown || sprngs[0] == Spring::Operational)
        count += countVariations(std::vector<Spring>(sprngs.begin()+1, sprngs.end()), nums);

    if (sprngs[0] == Spring::Unknown || sprngs[0] == Spring::Damaged) {
        std::vector<Spring> tempVec(sprngs.begin(), sprngs.begin()+nums[0]);
        if (nums[0] <= sprngs.size() && std::find(tempVec.begin(), tempVec.end(), Spring::Operational) == tempVec.end() &&
            (nums[0] == sprngs.size() || sprngs[nums[0]] != Spring::Damaged)) {
            if (sprngs.begin()+nums[0]+1 > sprngs.end()) count += countVariations({}, std::vector<int>(nums.begin()+1, nums.end()));
            else count += countVariations(std::vector<Spring>(sprngs.begin()+nums[0]+1, sprngs.end()), std::vector<int>(nums.begin()+1, nums.end()));
        }
    }
    Record::cache[key] = count;
    return count;
}

Records getRecords(std::ifstream &input) {
    Records records;

    std::string line;
    while(std::getline(input, line)) {
        Record record;
        std::string split;
        std::stringstream ss(line);
        for(int i = 0; std::getline(ss, split, ' '); i++) {
            if (i == 0) {
                std::vector<Spring> springs;
                for (char c : split) {
                    if (c == '?') springs.push_back(Spring::Unknown);
                    if (c == '#') springs.push_back(Spring::Damaged);
                    if (c == '.') springs.push_back(Spring::Operational);
                }
                record.springs = springs;
            }
            if (i == 1) {
                std::string string;
                std::stringstream ss2(split);
                std::vector<int> numbers;
                while(std::getline(ss2, string, ',')) {
                    numbers.push_back(std::stoi(string));
                }
                record.numbers = numbers;
            }
        }
        records.list.push_back(record);
    }
    return records;
}