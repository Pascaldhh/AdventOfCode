#include <format>
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

struct Space {
    int amount;

    virtual std::string str();

    Space(int amount) : amount(amount) {}
    virtual ~Space() = default;
};

struct EmptySpace : public Space {
    EmptySpace(int amount) : Space(amount) {}
    std::string str() override;
};

struct TakenSpace : public Space {
    int id;

    TakenSpace(int amount, int id) : Space(amount), id(id) {}
    std::string str() override;
};

struct Disk {
    std::vector<std::unique_ptr<Space>> format;

    std::string str();

    std::tuple<EmptySpace*, int> getFirstEmptySpace();
    std::tuple<TakenSpace*, int> getLastTakenSpace();

    void insertSpace(int, const std::unique_ptr<Space> &);
    void compact();

    static Disk parse(std::ifstream &);
};

void partOne(Disk &);
void partTwo(Disk &);
int main() {
    std::ifstream input("../input.txt");
    if (!input.is_open()) return -1;

    Disk disk = Disk::parse(input);
    input.close();

    partOne(disk);
    partTwo(disk);

    return 0;
}

void partOne(Disk &disk) {
    disk.compact();
    std::cout << disk.str() << std::endl;
}

void partTwo(Disk &disk) {

}



void Disk::compact() {
    std::tuple<EmptySpace*, int> emptyTuple = getFirstEmptySpace();
    std::tuple<TakenSpace*, int> takenTuple = getLastTakenSpace();

    while (std::get<1>(emptyTuple) > std::get<1>(takenTuple)) {
        EmptySpace* emptySpace = std::get<0>(emptyTuple);
        TakenSpace* takenSpace = std::get<0>(takenTuple);
        while (emptySpace->amount > 0) {
            if (0 > takenSpace->amount) {
                takenTuple = getLastTakenSpace();
            }

            takenSpace->amount--;
            emptySpace->amount--;

        }

        emptyTuple = getFirstEmptySpace();
    }
}

void Disk::insertSpace(int index, const std::unique_ptr<Space> &space) {
    format.insert(format.begin() + index, space);
}

std::string Disk::str() {
    std::string result = "Disk[format: [\n";
    for (const std::unique_ptr<Space> &space : format) {
        result += '\t' + space->str() + '\n';
    }
    result += "]";
    return result;
}

std::tuple<EmptySpace*, int> Disk::getFirstEmptySpace() {
    for (int i = 0; i < format.size(); i++) {
        if (EmptySpace* emptySpace = dynamic_cast<EmptySpace*>(format[i].get())) {
            return std::make_tuple(emptySpace, i);
        }
    }

    return std::make_tuple(nullptr, -1);
}

std::tuple<TakenSpace*, int> Disk::getLastTakenSpace() {
    for (int i = format.size()-1; i >= 0; i--) {
        if (TakenSpace* takenSpace = dynamic_cast<TakenSpace*>(format[i].get())) {
            return std::make_tuple(takenSpace, i);
        }
    }

    return std::make_tuple(nullptr, -1);
}


std::string Space::str() {
    return std::format("Space[amount:{}]", amount);
}

std::string EmptySpace::str() {
    return std::format("EmptySpace[amount:{}]", amount);
}

std::string TakenSpace::str() {
    return std::format("TakenSpace[amount:{},id:{}]", amount, id);
}

Disk Disk::parse(std::ifstream &input) {
    Disk disk;

    std::string line;
    while (std::getline(input, line)) {
        int id = 0;
        for (int i = 0; i < line.size(); i++) {
            const char c = line[i];
            if (i % 2 == 0) {
                disk.format.push_back(std::make_unique<TakenSpace>(TakenSpace(c - '0', id)));
                id++;
                continue;
            }

            disk.format.push_back(std::make_unique<EmptySpace>(EmptySpace(c - '0')));
        }
    }

    return disk;
}
