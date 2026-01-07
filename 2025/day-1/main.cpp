#include <iostream>
#include <fstream>
#include <map>
#include <utility>
#include <vector>
#include <sstream>
#include <functional>

enum class Direction { Left, Right };
std::map<char, Direction> charDirTable {
    { 'L', Direction::Left },
    { 'R', Direction::Right }
};

struct Rotation {
    Direction direction;
    int distance;
};

class Rotator {
    int max_dial = 100;

    int dial;
    std::vector<Rotation> rotations;
public:
    Rotator(const int dial, std::vector<Rotation> rotations): dial(dial), rotations(std::move(rotations)) {}

    [[nodiscard]] int getDial() const;
    [[nodiscard]] int decreaseToValid(Rotation& rotation) const;

    void rotate(const std::function<void(int, int, bool)>&);
    void rotateAll(const std::function<void(int, int, bool)>&);
};

std::vector<Rotation> getRotations(std::ifstream&);

void partOne(const std::vector<Rotation> &);
void partTwo(const std::vector<Rotation> &);

int main() {
    std::ifstream input("../input.txt");

    if (input.fail()) return -1;

    const std::vector<Rotation> rotations (getRotations(input));

    partOne(rotations);
    partTwo(rotations);

    input.close();

    return 0;
}

void partOne(const std::vector<Rotation> &rotations) {
    Rotator rotator(50, rotations);

    int answer = 0;
    rotator.rotateAll([&answer](const int dial, const int _, const bool _b) {
       if (dial == 0) answer++;
    });

    std::cout << "Answer part one: " << answer << std::endl;
}

void partTwo(const std::vector<Rotation> &rotations) {
    Rotator rotator(50, rotations);

    int answer = 0;
    rotator.rotateAll([&answer](const int dial, const int amount_decrease, const bool over_bounds) {
        answer += amount_decrease + over_bounds;
        if (dial == 0) answer += 1 - over_bounds;

    });

    std::cout << "Answer part two: " << answer << std::endl;
}

std::vector<Rotation> getRotations(std::ifstream &input) {
    std::vector<Rotation> result;

    std::string line;

    char direction;
    int distance;
    while (std::getline(input, line, '\n')) {
        std::stringstream stream(line);
        stream >> direction >> distance;

        Rotation rotation(charDirTable[direction], distance);
        result.push_back(rotation);
    }

    return result;
}

int Rotator::getDial() const {
    return dial;
}

int Rotator::decreaseToValid(Rotation& rotation) const {
    int amounts_decreased = 0;
    while (rotation.distance > max_dial) {
        rotation.distance -= max_dial;
        amounts_decreased++;
    }

    return amounts_decreased;
}

void Rotator::rotate(const std::function<void(int, int, bool)>& callback) {
    if (rotations.empty()) return;

    Rotation* rotation(&rotations.front());
    int amount_decreased = decreaseToValid(*rotation);
    bool over_bounds = false;

    bool on_zero = dial == 0;
    switch (rotation->direction) {
        case Direction::Left:
            dial -= rotation->distance;
            if (0 > dial) {
                dial += max_dial;
                if (!on_zero) over_bounds = true;
            }
            break;
        case Direction::Right:
            dial += rotation->distance;
            if (dial >= max_dial) {
                dial -= max_dial;
                over_bounds = true;
            }
            break;
    }

    callback(dial, amount_decreased, over_bounds);

    rotations.erase(rotations.begin());
}

void Rotator::rotateAll(const std::function<void(int, int, bool)> &callback) {
    while (!rotations.empty()) {
        rotate(callback);
    }
}
