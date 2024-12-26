#include <iostream>
#include <fstream>
#include <sstream>

struct PageOrderingRule {
    int before;
    int after;

    std::string str();

    static PageOrderingRule parse(std::string);
};
struct PageUpdate {
    std::vector<int> pageNumbers;
    bool isValid(std::vector<PageOrderingRule>);

    std::string str();

    static PageUpdate parse(std::string);
};
struct PrintQueue {
    std::vector<PageUpdate> updates;
    std::vector<PageOrderingRule> rules;

    std::string str();

    static PrintQueue parse(std::ifstream &);
};

void partOne(PrintQueue);
void partTwo(PrintQueue);
int main() {
    std::ifstream input("../input.txt");
    if(!input.is_open()) return -1;

    PrintQueue printQueue = PrintQueue::parse(input);

    partOne(printQueue);
    partTwo(printQueue);

    return 0;
}

void partOne(PrintQueue printQueue) {
    std::cout << printQueue.str() << std::endl;
    std::cout << "Answer part 1: " << std::endl;
}

void partTwo(PrintQueue printQueue) {
    std::cout << "Answer part 2: " << printQueue.updates.size() << std::endl;
}

PageOrderingRule PageOrderingRule::parse(std::string line) {
    PageOrderingRule result = PageOrderingRule();

    std::string num;
    std::stringstream ss(line);
    for (int i = 0; std::getline(ss, num, '|'); i++) {
        if(i == 0) result.before = std::stoi(num);
        if(i == 1) result.after = std::stoi(num);
    }

    return result;
}

std::string PageOrderingRule::str() {
    return std::format("PageOrderingRule[before:{}, after:{}]", before, after);
}

std::string PageUpdate::str() {
    std::string result = "PageUpdate[numbers:";
    for(int n : pageNumbers) result += std::format("{},", n);
    result.pop_back();
    result += "]";
    return result;
}

std::string PrintQueue::str() {
    std::string result = "PrintQueue[rules:[\n";
    for(PageOrderingRule rule : rules) result += "\t" + rule.str() + "\n";
    result += "], updates:[\n";
    for(PageUpdate update : updates) result += "\t" + update.str() + "\n";
    result += "]]";
    return result;
}

PageUpdate PageUpdate::parse(std::string line) {
    PageUpdate result = PageUpdate();

    std::string num;
    std::stringstream ss(line);
    while (std::getline(ss, num, ',')) {
        result.pageNumbers.push_back(std::stoi(num));
    }

    return result;
}

bool PageUpdate::isValid(std::vector<PageOrderingRule> rules) {

    return false;
}

PrintQueue PrintQueue::parse(std::ifstream &input) {
    PrintQueue printQueue = PrintQueue();

    std::string line;
    bool isPageOrderingRules = true;
    while(std::getline(input, line)) {
        if(line.empty()) {
            isPageOrderingRules = false;
            continue;
        }

        if(isPageOrderingRules) {
            printQueue.rules.push_back(PageOrderingRule::parse(line));
            continue;
        }

        printQueue.updates.push_back(PageUpdate::parse(line));
    }

    return printQueue;
}
