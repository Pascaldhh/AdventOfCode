#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <format>
#include <numeric>

struct PageUpdate;

struct PageOrderingRule {
    int before;
    int after;

    bool isAvailable(PageUpdate);
    std::string str();

    static PageOrderingRule parse(std::string);
};

struct PageUpdate {
    std::vector<int> pageNumbers;

    bool isValid(PageOrderingRule);
    bool isValid(std::vector<PageOrderingRule>);
    void sort(std::vector<PageOrderingRule>);
    void sort(PageOrderingRule);
    int getCenterPage();
    std::string str();

    static PageUpdate parse(std::string);
};

struct PrintQueue {
    std::vector<PageUpdate> updates;
    std::vector<PageOrderingRule> rules;

    std::vector<PageOrderingRule> getAvailableRules(PageUpdate);
    std::vector<PageUpdate> getSuccessfulUpdates(bool (*f)(PageUpdate, std::vector<PageOrderingRule>));
    std::vector<int> getCenterPages(std::vector<PageUpdate>);
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
    auto pred = [](PageUpdate update, std::vector<PageOrderingRule> rules) { return update.isValid(rules); };
    std::vector<PageUpdate> successUpdates = printQueue.getSuccessfulUpdates(pred);
    std::vector<int> centerNumbers = printQueue.getCenterPages(successUpdates);

    std::cout << "Answer part 1: " << std::reduce(centerNumbers.begin(), centerNumbers.end()) << std::endl;
}

void partTwo(PrintQueue printQueue) {
    auto pred = [](PageUpdate update, std::vector<PageOrderingRule> rules) { return !update.isValid(rules); };
    std::vector<PageUpdate> successUpdates = printQueue.getSuccessfulUpdates(pred);
    for(PageUpdate &pageUpdate : successUpdates) pageUpdate.sort(printQueue.getAvailableRules(pageUpdate));
    std::vector<int> centerNumbers = printQueue.getCenterPages(successUpdates);

    std::cout << "Answer part 2: " << std::reduce(centerNumbers.begin(), centerNumbers.end()) << std::endl;
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

bool PageOrderingRule::isAvailable(PageUpdate update) {
    for(int i = 0; i < 2; i++) {
        bool available = false;

        for(int pageNumber : update.pageNumbers) {
           if((i == 0 && pageNumber == before) || (i == 1 && pageNumber == after)) {
               available = true;
           }
        }

        if(!available) return false;
    }

    return true;
}

std::vector<PageOrderingRule> PrintQueue::getAvailableRules(PageUpdate update) {
    std::vector<PageOrderingRule> availableRules;
    for(PageOrderingRule rule : rules) {
        if(rule.isAvailable(update)) availableRules.push_back(rule);
    }
    return availableRules;
}

bool PageUpdate::isValid(PageOrderingRule rule) {
    bool foundBefore = false;

    for(int number : pageNumbers) {
        if(!foundBefore && rule.before == number) {
            foundBefore = true;
            continue;
        }

        if(foundBefore && rule.after == number) {
            return true;
        }
    }

    return false;
}

bool PageUpdate::isValid(std::vector<PageOrderingRule> rules) {
    for(PageOrderingRule rule : rules) {
        if(!isValid(rule)) return false;
    }
    return true;
}

std::vector<PageUpdate> PrintQueue::getSuccessfulUpdates(bool (*f)(PageUpdate, std::vector<PageOrderingRule>)) {
    std::vector<PageUpdate> solidUpdates;

    for(PageUpdate update : updates) {
        if(f(update, getAvailableRules(update))) {
            solidUpdates.push_back(update);
        }
    }

    return solidUpdates;
}

void PageUpdate::sort(std::vector<PageOrderingRule> rules) {
    for(auto rule : rules) {
        if(isValid(rule)) continue;


        sort(rule);
        sort(rules);
    }
}

void PageUpdate::sort(PageOrderingRule rule) {
    enum class SearchState {
        After,
        Before,
        Finished
    };

    SearchState current = SearchState::After;

    for(int &p : pageNumbers) {
        switch (current) {
            case SearchState::After:
                if(p != rule.after) continue;
                p = rule.before;
                current = SearchState::Before;
                break;
            case SearchState::Before:
                if(p != rule.before) continue;
                p = rule.after;
                current = SearchState::Finished;
                break;
            case SearchState::Finished:
                return;
        }
    }
}


int PageUpdate::getCenterPage() {
    unsigned long index = pageNumbers.size() / 2;
    return pageNumbers[index];
}

std::vector<int> PrintQueue::getCenterPages(std::vector<PageUpdate> updates) {
    std::vector<int> pageNums;

    for(PageUpdate update : updates) {
        pageNums.push_back(update.getCenterPage());
    }

    return pageNums;
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
