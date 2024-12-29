#include <iostream>
#include <fstream>
#include <sstream>
#include <format>
#include <numeric>
#include <functional>

struct Equation {
    long answer;
    std::vector<long> numbers;
    std::vector<std::function<long(long, long)>> operators;

    bool isValid();
    std::string str() const;

    static Equation parse(const std::string&);
};

struct Equations {
    std::vector<Equation> value;

    std::vector<Equation> getValidEquations();
    long countValidEquationsAnswers();
    std::string str() const;

    void setOperators(const std::vector<std::function<long(long, long)>>&);

    static Equations parse(std::ifstream &);
};

void partOne(Equations &);
void partTwo(Equations &);
int main() {
    std::ifstream input("../input.txt");
    if (!input.is_open()) return -1;

    Equations equations = Equations::parse(input);
    input.close();

    partOne(equations);
    partTwo(equations);

    return 0;
}

void partOne(Equations &equations) {
    equations.setOperators({ std::plus(), std::multiplies() });
    long result = equations.countValidEquationsAnswers();
    std::cout << "Answer part 1: " << result << std::endl;
}

void partTwo(Equations &equations) {
    std::function concatenation = [](long x, long y) {
        return std::stol(std::to_string(x) + std::to_string(y));
    };

    equations.setOperators({ std::plus(), std::multiplies(), concatenation });

    long result = equations.countValidEquationsAnswers();
    std::cout << "Answer part 2: " << result << std::endl;
}

bool Equation::isValid() {
    if (0 > numbers.size()) {
        return false;
    }
    if (numbers.size() == 1) {
        return numbers.front() == answer;
    }
    for (const std::function<long(long, long)>& op : operators) {
        long answr = op(numbers[0], numbers[1]);

        Equation nEq = Equation{ .answer = answer, .numbers = numbers, .operators = operators };
        nEq.numbers.erase(nEq.numbers.begin(), nEq.numbers.begin()+1);
        nEq.numbers[0] = answr;
        if (nEq.isValid()) return true;
    }
}

std::vector<Equation> Equations::getValidEquations() {
    std::vector<Equation> equations;

    for (Equation equation : value) {
        if (equation.isValid()) {
            equations.push_back(equation);
        }
    }

    return equations;
}

long Equations::countValidEquationsAnswers() {
    std::vector<long> answersOfValid;
    for (Equation equation : getValidEquations()) {
        answersOfValid.push_back(equation.answer);
    }

    return std::reduce(answersOfValid.begin(), answersOfValid.end());
}

void Equations::setOperators(const std::vector<std::function<long(long, long)>> &operators) {
    for (Equation &equation : value) {
        equation.operators = operators;
    }
}

std::string Equation::str() const {
    std::string result = std::format("Equation[answer:{},numbers:[", answer);
    for (const long n : numbers) {
        result += std::format("{},", n);
    }
    result.pop_back();
    result += "]";
    return result;
}

std::string Equations::str() const {
    std::string result = "Equations[\n";

    for (const Equation& equation : value) {
        result += "\t" + equation.str() + '\n';
    }
    result += "]";
    return result;
}

Equation Equation::parse(const std::string& line) {
    Equation equation;

    std::string answer;
    std::stringstream ssLine(line);
    for (int i = 0; std::getline(ssLine, answer, ':'); i++) {
        if (i == 0) {
            equation.answer = std::stol(answer);
            continue;
        }

        std::stringstream ss(answer);
        std::string num;
        while (std::getline(ss, num, ' ')) {
            if (num.empty()) continue;
            equation.numbers.push_back(std::stol(num));
        }
    }

    return equation;
}

Equations Equations::parse(std::ifstream &input) {
    Equations equations;

    std::string line;
    while (std::getline(input, line)) {
        equations.value.push_back(Equation::parse(line));
    }

    return equations;
}
