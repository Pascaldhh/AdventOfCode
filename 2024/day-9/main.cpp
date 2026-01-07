#include <algorithm>
#include <format>
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>
#include <set>

enum class Order {
    First,
    Last
};

struct Space {
    int amount;

    Space(const int amount) : amount(amount) {}
    bool isEmpty() const;

    virtual std::string str();
    virtual ~Space() = default;
};

struct EmptySpace : public Space {
    EmptySpace(const int amount) : Space(amount) {}
    std::string str() override;
};

struct TakenSpace : public Space {
    int id;

    TakenSpace(const int amount, const int id) : Space(amount), id(id) {}
    std::string str() override;
};

struct Disk {
    std::vector<std::unique_ptr<Space>> format;

    template <class T = Space>
    T* get(Order);
    int getIndex(Space*);
    std::tuple<TakenSpace, EmptySpace> createTakenSpace(EmptySpace*, TakenSpace*);
    long long calculateChecksum();
    std::string str();

    void remove(Space*);
    void reFormat();
    void reFormatWholeFiles();

    static Disk parse(std::ifstream &);
    static bool isTakenSpace(int);
    static std::vector<std::unique_ptr<Space>> generateEmptyOrTakenSpaceWithId(const std::string &);
    static std::unique_ptr<Space> createEmptyOrTakenSpaceOnIndex(int, int, int);
};

void partOne(Disk &);
void partTwo(Disk &);
int main() {
    std::ifstream input("../input.txt");
    if (!input.is_open()) return -1;

    Disk disk = Disk::parse(input);
    Disk disk2 = Disk::parse(input);
    input.close();

    partOne(disk);
    partTwo(disk2);

    return 0;
}

void partOne(Disk &disk) {
    disk.reFormat();
    std::cout << "Answer part 1: " << disk.calculateChecksum() << std::endl;
}

void partTwo(Disk &disk) {
    disk.reFormatWholeFiles();

    std::cout << "Answer part 2: " << disk.calculateChecksum() << std::endl;
}

void Disk::reFormat() {
    auto* emptySpace = get<EmptySpace>(Order::First);
    auto* takenSpace = get<TakenSpace>(Order::Last);

    while (getIndex(emptySpace) < getIndex(takenSpace)) {
        while (!emptySpace->isEmpty()) {
            const int indexOfPlacement = getIndex(emptySpace);
            auto newSpaces = createTakenSpace(emptySpace, takenSpace);
            format.insert(format.begin() + indexOfPlacement, std::make_unique<TakenSpace>(std::get<0>(newSpaces)));
            if (takenSpace->isEmpty()) {
                remove(takenSpace);
                takenSpace = get<TakenSpace>(Order::Last);
            }
        }
        remove(emptySpace);
        emptySpace = get<EmptySpace>(Order::First);
    }
}

void Disk::reFormatWholeFiles() {
    std::set<int> alreadyPlaced;
    for (int i = format.size()-1; i >= 0; i--) {
        auto* takenSpace = dynamic_cast<TakenSpace*>(format[i].get());
        if (!takenSpace) continue;

        if (alreadyPlaced.contains(takenSpace->id)) continue;;
        alreadyPlaced.insert(takenSpace->id);

        for (int j = 0; j < format.size(); j++) {
            if (takenSpace->isEmpty() || i < j) break;
            auto *emptySpace = dynamic_cast<EmptySpace*>(format[j].get());
            if (!emptySpace || emptySpace->amount < takenSpace->amount) continue;
            int indexOfPlacement = getIndex(emptySpace);

            auto newSpaces = createTakenSpace(emptySpace, takenSpace);
            format.insert(format.begin() + indexOfPlacement, std::make_unique<TakenSpace>(std::get<0>(newSpaces)));
            format.insert(format.begin() + getIndex(takenSpace), std::make_unique<EmptySpace>(std::get<1>(newSpaces)));

            if (emptySpace->isEmpty()) remove(emptySpace);
        }
        if (takenSpace->isEmpty()) remove(takenSpace);
    }
}

template<class T>
T* Disk::get(const Order order) {
    T* spacePtr = nullptr;

    for (auto &space : format) {
        if (auto *type = dynamic_cast<T*>(space.get())) {
            if (order == Order::First) {
                return type;
            }
            spacePtr = type;
        }
    }

    return spacePtr;
}

int Disk::getIndex(Space *space) {
    const auto it = std::find_if(format.begin(), format.end(), [space](const std::unique_ptr<Space> &item) {
        return item.get() == space;
    });

    return it - format.begin();
}

std::tuple<TakenSpace, EmptySpace> Disk::createTakenSpace(EmptySpace *emptySpace, TakenSpace *takenSpace) {
    std::tuple<TakenSpace, EmptySpace> tuple = std::make_tuple(TakenSpace(0, takenSpace->id), EmptySpace(0));
    TakenSpace &newTakenSpace = std::get<0>(tuple);
    EmptySpace &newEmptySpace = std::get<1>(tuple);

    const int amountOfLoops = std::min(emptySpace->amount, takenSpace->amount );
    for (int i = 0; i < amountOfLoops; i++) {
        newTakenSpace.amount++;
        newEmptySpace.amount++;
        emptySpace->amount--;
        takenSpace->amount--;
    }
    return tuple;
}

long long Disk::calculateChecksum() {
    long long checksum = 0, position = 0;
    for (std::unique_ptr<Space> &space : format) {
        auto* emptySpace = dynamic_cast<EmptySpace*>(space.get());
        if (emptySpace) {
            for (int i = 0; i < emptySpace->amount; i++) {
                position++;
            }
        }

        auto* takenSpace = dynamic_cast<TakenSpace*>(space.get());
        if (!takenSpace) continue;

        for (int i = 0; i < space->amount; i++) {
            checksum += position * takenSpace->id;
            position++;
        }
    }

    return checksum;
}

bool Space::isEmpty() const {
    return amount <= 0;
}

void Disk::remove(Space *space) {
    int index = getIndex(space);

    if (0 > index || index > format.size()-1) {
        return;
    }

    format.erase(format.begin() + index);
}

std::string Disk::str() {
    std::string result = "Disk[format: [\n";
    for (const std::unique_ptr<Space> &space : format) {
        result += '\t' + space->str() + '\n';
    }
    result += "]";
    return result;
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

    std::string line, wholeFileAsLine;
    while (std::getline(input, line)) {
        wholeFileAsLine += line;
    }

    disk.format = generateEmptyOrTakenSpaceWithId(wholeFileAsLine);

    input.clear();
    input.seekg(std::ios::beg);

    return disk;
}

std::vector<std::unique_ptr<Space>> Disk::generateEmptyOrTakenSpaceWithId(const std::string &line) {
    std::vector<std::unique_ptr<Space>> result;

    long id = 0;
    for (int i = 0; i < line.size(); i++) {
        const int amount = line[i] - '0';
        result.push_back(createEmptyOrTakenSpaceOnIndex(i, amount, id));

        if (!isTakenSpace(i)) {
            continue;
        }

        id++;
    }

    return result;
}

std::unique_ptr<Space> Disk::createEmptyOrTakenSpaceOnIndex(const int index, const int amount, const int id) {
    if (isTakenSpace(index)) {
        return std::make_unique<TakenSpace>(TakenSpace(amount, id));
    }

    return std::make_unique<EmptySpace>(EmptySpace(amount));
}

bool Disk::isTakenSpace(const int index) {
    return index % 2 == 0;
}
