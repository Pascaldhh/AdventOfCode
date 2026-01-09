#include <format>
#include <iostream>
#include <fstream>

struct BatteryBank {
    std::string value;

    long long get_largest_joltage(int length = 2) const;

    static int get_all(std::ifstream &input, BatteryBank **banks);
};

void part_one(int size, BatteryBank* banks);
void part_two(int size, BatteryBank* banks);

int main() {
    std::ifstream input("../input.txt");
    if (input.fail()) return -1;

    BatteryBank* banks = nullptr;
    int size = BatteryBank::get_all(input, &banks);

    part_one(size, banks);
    part_two(size, banks);

    input.close();

    delete [] banks;

    return 0;
}

void part_one(int size, BatteryBank* banks) {
    long long sum = 0;
    for (int i = 0; i < size; i++) {
        long long value = banks[i].get_largest_joltage();
        sum += value;
    }
    std::cout << "Answer part one: " << sum << std::endl;
}

void part_two(int size, BatteryBank* banks) {
    long long sum = 0;
    for (int i = 0; i < size; i++) {
        long long value = banks[i].get_largest_joltage(12);
        sum += value;
    }
    std::cout << "Answer part two: " << sum << std::endl;
}

long long BatteryBank::get_largest_joltage(int length) const {
    std::string result = "";

    for (int i = 0; i < value.size(); i++) {
        char current_cn = value[i];
        bool been_inserted = false;

        int offset = i - value.size() + length;
        for (int j = result.size()-1; j >= (offset < 0 ? 0 : offset); j--) {
            char current_r_cn = result[j];
            if (current_r_cn >= current_cn) continue;

            been_inserted = true;
            result.erase(j);
            result.insert(j, { current_cn });
        }

        if (!been_inserted && result.size() < length) result.append({ current_cn });
    }

    return std::stoll(result);
}

int BatteryBank::get_all(std::ifstream &input, BatteryBank **banks) {
    int size = 1;
    std::string line;
    for (;std::getline(input, line, '\n'); size++) {
        auto* newBanks = new BatteryBank[size];

        for (int i = 0; i < size-1; i++) {
            newBanks[i] = BatteryBank((*banks)[i].value);
        }

        newBanks[size-1] = BatteryBank(line);

        delete [] *banks;

        *banks = newBanks;
    }

    return --size;
}


