#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include <algorithm>
#include <iterator>

enum class Object { None, RollOfPaper };
enum class Move { Up, UpRight, Right, DownRight, Down, DownLeft, Left, UpLeft };

struct Axes {
    int x, y;

    Axes operator+(const Axes &other);
    Axes operator-(const Axes &other);
};

const std::map<Move, Axes> move_axes_map {
    { Move::Up, { 0, -1 } },
    { Move::UpRight, { 1, -1 } },
    { Move::Right, { 1, 0 } },
    { Move::DownRight, { 1, 1 } },
    { Move::Down, { 0, 1 } },
    { Move::DownLeft, {-1, 1 } },
    { Move::Left, {-1, 0 } },
    { Move::UpLeft, { -1, -1 } }
};

const std::map<char, Object> char_object_map {
    { '.', Object::None },
    { '@', Object::RollOfPaper }
};

std::map<Object, char> object_char_map;

struct Diagram {
    std::vector<std::vector<Object>> value;

    std::vector<Axes> calculate_accessible();
    std::vector<Axes> calculate_accessible_repeat();

    bool is_accessible(Axes axes, int max_paper = 3);
    bool in_bounds(Axes axes);

    void replace_multiple_on(const std::vector<Axes>& axeses, const Object &object);
    void replace_on(const Axes &axes, const Object &object);
    void print();

    static Diagram get(std::ifstream &input);
};

void part_one(Diagram &diagram);
void part_two(Diagram &diagram);

int main() {
    std::transform(char_object_map.begin(), char_object_map.end(), std::inserter(object_char_map, object_char_map.end()), [](std::pair<char, Object> value) {
        return std::pair { value.second, value.first };
    });

    std::ifstream input("../input.txt");
    if (input.fail()) return -1;

    Diagram diagram = Diagram::get(input);

    part_one(diagram);
    part_two(diagram);

    input.close();

    return 0;
}

void part_one(Diagram &diagram) {
    std::cout << "Answer part one: " << diagram.calculate_accessible().size() << std::endl;
}

void part_two(Diagram &diagram) {
    std::cout << "Answer part two: " << diagram.calculate_accessible_repeat().size() << std::endl;
}

std::vector<Axes> Diagram::calculate_accessible() {
    std::vector<Axes> axeses;
    for (int y = 0; y < value.size(); y++) {
        for (int x = 0; x < value[y].size(); x++) {
            const Object &obj = value[y][x];

            if (obj == Object::RollOfPaper && is_accessible({ x, y })) axeses.push_back({ x, y });
        }
    }

    return axeses;
}

std::vector<Axes> Diagram::calculate_accessible_repeat() {
    std::vector<Axes> result;

    std::vector<Axes> current_calculated = calculate_accessible();
    while (!current_calculated.empty()) {
        replace_multiple_on(current_calculated, Object::None);
        result.insert(result.end(), current_calculated.begin(), current_calculated.end());
        current_calculated = calculate_accessible();
    }

    return result;
}

bool Diagram::is_accessible(Axes axes, int max_paper) {
    int count = 0;
    for (const auto &m : move_axes_map) {
        Axes point = axes + m.second;

        if (!in_bounds(point)) continue;

        if (value[point.y][point.x] == Object::RollOfPaper) count++;
    }

    return count <= max_paper;
}

bool Diagram::in_bounds(Axes axes) {
    return axes.x >= 0 && axes.y >= 0 &&
        axes.x < value[axes.y].size() && axes.y < value.size();
}

void Diagram::replace_multiple_on(const std::vector<Axes>& axeses, const Object &object) {
    for (const Axes &axes : axeses) {
        replace_on(axes, object);
    }
}

void Diagram::replace_on(const Axes& axes, const Object &object) {
    if (!in_bounds(axes)) return;

    value[axes.y][axes.x] = object;
}

Axes Axes::operator+(const Axes &other) {
    return {
        x + other.x,
        y + other.y
    };
}

Axes Axes::operator-(const Axes &other) {
    return {
        x - other.x,
        y - other.y
    };
}

void Diagram::print() {
    for (const std::vector<Object>& objects : value) {
        for (const Object &obj : objects) {
            std::cout << object_char_map.at(obj);
        }
        std::cout << std::endl;
    }
}

Diagram Diagram::get(std::ifstream &input) {
    Diagram result;

    std::string line;
    while (std::getline(input, line)) {
        std::vector<Object> objects;
        for (char c : line) objects.push_back(char_object_map.at(c));
        result.value.push_back(objects);
    }

    return result;
}