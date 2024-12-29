#include <algorithm>
#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <regex>
#include <format>

struct IRegexResult {
public:
    int position;
    std::string word;

    IRegexResult(int position, std::string word);
    virtual ~IRegexResult() = default;
};

struct RegexResult : public IRegexResult {
    RegexResult(int position, std::string word);
};

struct Mul : public IRegexResult {
    std::vector<int> numbers;
    Mul(int position, std::string word, std::vector<int> numbers);

    int multiply();
    std::string str();
};

struct RegexResults {
    std::vector<std::unique_ptr<IRegexResult>> value;
    RegexResults(std::vector<std::unique_ptr<IRegexResult>>& value);
    void sortOnPosition();
    int multiplyEnabledMuls();
    int multiplyMuls();
    static RegexResults parseAll(std::ifstream &);
};

void partOne(RegexResults &);
void partTwo(RegexResults &);
int main() {
    std::ifstream input("../input.txt");

    if(!input.is_open()) return -1;

    RegexResults regexResults = RegexResults::parseAll(input);

    partOne(regexResults);
    partTwo(regexResults);

    input.close();
    return 0;
}

void partOne(RegexResults& regexResults) {
    std::cout << "Answer part 1: " << regexResults.multiplyMuls() << std::endl;
}

void partTwo(RegexResults& regexResults) {
    std::cout << "Answer part 2: " << regexResults.multiplyEnabledMuls() << std::endl;
}

IRegexResult::IRegexResult(int position, std::string word) {
    this->position = position;
    this->word = std::move(word);
}

RegexResult::RegexResult(int position, std::string word): IRegexResult(position, std::move(word)) {}

Mul::Mul(int position, std::string word, std::vector<int> numbers) : IRegexResult(position, std::move(word)) {
    this->numbers = std::move(numbers);
}

RegexResults::RegexResults(std::vector<std::unique_ptr<IRegexResult>>& value) {
   this->value = std::move(value);
}

std::string Mul::str() {
    std::string result = std::format("Mul[position={},numbers=", position);
    std::string divider = ",";
    for(int n : numbers) result += std::format("{}{}", n, divider);
    result = result.substr(0, result.size()-divider.size());
    result += "]";
    return result;
}

RegexResults RegexResults::parseAll(std::ifstream &input) {
    std::vector<std::unique_ptr<IRegexResult>> result;

    std::string allLines;
    std::string line;
    while(std::getline(input, line)) {
        allLines += line;
    }

    std::regex exp(R"(mul\((\d+),(\d+)\))");

    std::sregex_iterator iter(allLines.begin(), allLines.end(), exp);
    std::sregex_iterator end;

    for (;iter != end; iter++) {
        int num1 = std::stoi(iter->str(1));
        int num2 = std::stoi(iter->str(2));
        result.push_back(std::make_unique<Mul>(Mul(static_cast<int>(iter->position()), iter->str(), std::vector<int>{num1, num2})));
    }
    exp = (R"(do\(\))");
    iter = std::sregex_iterator (allLines.begin(), allLines.end(), exp);
    for (;iter != end; iter++) {
        result.push_back(std::make_unique<RegexResult>(RegexResult(static_cast<int>(iter->position()), iter->str())));
    }

    exp = (R"(don't\(\))");
    iter = std::sregex_iterator (allLines.begin(), allLines.end(), exp);
    for (;iter != end; iter++) {
        result.push_back(std::make_unique<RegexResult>(RegexResult(static_cast<int>(iter->position()), iter->str())));
    }

    return {(result)};
}

void RegexResults::sortOnPosition() {
    std::sort(value.begin(), value.end(),
  [](const std::unique_ptr<IRegexResult> &a, const std::unique_ptr<IRegexResult> &b) {
        return a->position < b->position;
    });
}

int RegexResults::multiplyEnabledMuls() {
    sortOnPosition();

    std::vector<Mul> mulResults;
    bool enabled = true;
    for (const std::unique_ptr<IRegexResult> &regexResult: value) {
        if (regexResult->word == "do()") enabled = true;
        if (regexResult->word == "don't()") enabled = false;

        Mul *mul = dynamic_cast<Mul*>(regexResult.get());
        if (enabled && mul) {
            mulResults.push_back(*mul);
        }
    }

    int result = 0;
    for(Mul mul : mulResults) result += mul.multiply();

    return result;
}

int RegexResults::multiplyMuls() {
    int result = 0;

    for(const std::unique_ptr<IRegexResult>& regexResult : value) {
        if (Mul* mul = dynamic_cast<Mul*>(regexResult.get())) {
            result += mul->multiply();
        }
    }

    return result;
}

int Mul::multiply() {
    if(numbers.empty()) return 0;

    int result = numbers.front();
    for(int i = 1; i < numbers.size(); i++) {
        int current = numbers[i];

        result *= current;
    }
    return result;
}
