#include <iostream>
#include <fstream>

struct BatteryBank {
    std::string value;

    int get_largest_joltage();

    static int get_all(std::ifstream &input, BatteryBank **banks);
};

void part_one();
void part_two();

int main() {
    std::ifstream input("../input.txt");
    if (input.fail()) return -1;

    BatteryBank* banks = nullptr;
    int size = BatteryBank::get_all(input, &banks);

    part_one();
    part_two();

    input.close();

    delete [] banks;

    return 0;
}

void part_one() {

}

void part_two() {

}

int BatteryBank::get_largest_joltage() {
    for() {
        BatteryBank inner(this->value.substr(1));
    }


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


