#include <iostream>
#include <fstream>
#include <sstream>

struct Record {
    std::vector<char> springs;
    std::vector<int> numbers;
    std::vector<std::vector<char>> splitSprings();
    std::vector<int> amountSolveEachSplit();
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
    for (Record record : records.list) {
        for (int n : record.amountSolveEachSplit()) std::cout << n;
        std::cout << std::endl;
        std::cout << record.str() << std::endl;
    }
}
void partTwo(Records records) {

}

std::string Record::str() {
    std::string string = "Record[springs: [";
    for (int i = 0; i < springs.size(); i++) {
        string += springs[i];
        if (i != springs.size()-1) string += ',';
    }
    string += "], numbers: [";
    for (int i = 0; i < numbers.size(); i++) {
        string += std::to_string(numbers[i]);
        if (i != numbers.size()-1) string += ',';
    }
    string += "]]";
    return string;
}

std::vector<std::vector<char>> Record::splitSprings() {
    std::vector<std::vector<char>> allSprings;
    
    std::string string;
    for(int i = 0; i < springs.size(); i++) {
        char c = springs[i];
        if (c == '?' || c == '#') string += c;
        if (c == '.') {
            if (string.empty()) continue;
            
            allSprings.emplace_back(string.begin(), string.end());
            string = "";
        }
        if (i == springs.size()-1 && !string.empty()) 
            allSprings.emplace_back(string.begin(), string.end());
    }
    
    return allSprings;
}

int countSinglePositions(int n, std::vector<char> v) {
    int count = 0;
    for (int i = 0; i < v.size(); i++) if (i+n <= v.size()) count++;
    return count;
}

int countMultiplePositions(std::vector<int> nums, std::vector<char> vec) {
    int solve = 0;
    
    return solve;
}

//void countAndEraseSingleOccurrence(std::vector<int> &amount, std::vector<std::vector<char>> &sp, std::vector<int> &nums) {
//    for (int i = nums.size()-1; i >= 0; i--) {
//        int count = 0, index = 0;
//        for (int j = 0; nums[i] <= sp[j].size(); j++) {
//            count++;
//            index = j;
//        }
//        if (count != 1) continue;
//        std::cout << count << std::endl;
//        amount[i] += countSinglePositions(nums[i], sp[index]);
//        nums.erase(nums.begin() + i);
//        sp.erase(sp.begin() + index);
//    }
//}


std::vector<int> Record::amountSolveEachSplit() {
    std::vector<std::vector<char>> sp = this->splitSprings();
    std::vector<int> amount(this->numbers.size(), 0);
    std::vector<int> nums = this->numbers;

    //Same size nums and springs
    if (nums.size() == sp.size()) {
        for (int i = 0; sp.size() > i; i++)
            amount[i] = countSinglePositions(nums[i], sp[i]);
        return amount;
    }
    
    
    std::cout << "NUMS: ";
    for (int n : nums) std::cout << n << " ";
    std::cout << std::endl;
    
    for (std::vector<char> v : sp) {
        for (char c : v) std::cout << c;
        std::cout << std::endl;
    }
    
    return amount;
}

Records getRecords(std::ifstream &input) {
    Records records;
    
    std::string line;
    while(std::getline(input, line)) {
        Record record;
        std::string split;
        std::stringstream ss(line);
        for(int i = 0; std::getline(ss, split, ' '); i++) {
            if (i == 0) record.springs = std::vector<char>(split.begin(), split.end());
            if (i == 1) {
                std::string number;
                std::stringstream ss2(split);
                std::vector<int> numbers;
                while(std::getline(ss2, number, ',')) {
                    numbers.push_back(std::stoi(number));
                }
                record.numbers = numbers;
            }
        }
        records.list.push_back(record);
    }
    return records;
}