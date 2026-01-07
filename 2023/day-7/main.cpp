#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <sstream>
#include <map>
#include <algorithm>

enum Type { FiveOfAKind, FourOfAKind, FullHouse, ThreeOfAKind, TwoPair, OnePair, HighCard, NONE };

std::map<Type, std::vector<int>> TypeMap {
    {FiveOfAKind, std::vector<int>{5}},
    {FourOfAKind, std::vector<int>{4,1}},
    {FullHouse, std::vector<int>{3, 2}},
    {ThreeOfAKind, std::vector<int>{3, 1, 1}},
    {TwoPair, std::vector<int>{2, 2, 1}},
    {OnePair, std::vector<int>{2, 1, 1, 1}},
    {HighCard, std::vector<int>{1, 1, 1, 1, 1}}
};

std::vector<char> PowerTableP1 {'A', 'K', 'Q', 'J', 'T', '9', '8', '7', '6', '5', '4', '3', '2'};
std::vector<char> PowerTableP2 {'A', 'K', 'Q', 'T', '9', '8', '7', '6', '5', '4', '3', '2', 'J'};
std::vector<char> CurrentTable = PowerTableP1;

struct Hand {
    std::string cards; 
    long bid; 
    Type type;
    bool checkMorePower(Hand h, std::vector<char> powerTable) const;
    bool operator<(const Hand &h) const {return type < h.type || !checkMorePower(h, CurrentTable);};
    bool operator>(const Hand &h) const {return type > h.type || type == h.type && checkMorePower(h, CurrentTable);};
};

std::vector<Hand> getHands(std::ifstream &,  bool);
template<typename C> void sortHandsByType(std::vector<Hand> &, C, std::vector<char>);
long calculateTotalWinnings(std::vector<Hand> hands);
void typeMapper(Hand &, bool);

void partOne(std::ifstream &);
void partTwo(std::ifstream &);
int main() {
    std::ifstream input("../input.txt");
    if (!input.is_open()) return -1;
    
    partOne(input);
    partTwo(input);
    
    input.close();
    return 0;
}

void partOne(std::ifstream &input) {
    std::vector<Hand> hands = getHands(input, false);
    sortHandsByType(hands, std::greater<>(), PowerTableP1);
    
    std::cout << "Answer part 1: " << calculateTotalWinnings(hands) << std::endl;
}

void partTwo(std::ifstream &input){
    std::vector<Hand> hands = getHands(input, true);
    sortHandsByType(hands, std::greater<>(), PowerTableP2);
    
    std::cout << "Answer part 2: " << calculateTotalWinnings(hands) << std::endl;
}

std::vector<Hand> getHands(std::ifstream &input, bool hasJokers) {
    input.clear();
    input.seekg(0, std::ios::beg);
    
    std::vector<Hand> hands;
    std::string line;
    while(getline(input, line)) {
        std::string split;
        std::stringstream ss(line);
        Hand hand = {};
        for(int i = 0; getline(ss, split, ' '); i++) {
            if (i == 0) hand.cards = split;
            if (i == 1) hand.bid = std::stol(split);
        }
        typeMapper(hand, hasJokers);
        hands.push_back(hand);
    }
    return hands;
}

std::vector<int> checkType(std::string sequence, std::pair<Type, std::vector<int>> kvp) {
    std::vector<int> findings;
    char c = sequence[0];
    int count = 1;
    bool breakky = false;
    for(int i = 1; i <= sequence.size(); i++, count++) {
        if (c == sequence[i] && i != sequence.size()) continue;

        auto pos = std::find(kvp.second.begin(), kvp.second.end(), count);
        if (pos == kvp.second.end()) breakky = true;
        if (!breakky) kvp.second.erase(pos);
        
        c = sequence[i];
        findings.push_back(count);
        count = 0;
    }
    return findings;
}

void typeMapper(Hand &hand, bool hasJokers) {
    std::string sequence = hand.cards;
    
    int jokers = 0;
    if (hasJokers) {
        for (int i = sequence.size(); i >= 0; i--) {
            if (sequence[i] != 'J') continue;
            sequence.erase(i, 1);
            jokers++;
        }
    }
    
    std::sort(sequence.begin(), sequence.end());
    for (auto kvp : TypeMap) {
        auto vec1 = checkType(sequence, kvp);
        std::sort(vec1.begin(), vec1.end(), std::greater<>());
        if (hasJokers && !vec1.empty()) vec1[0] += jokers;
        
        auto vec2 = kvp.second;
        std::sort(vec2.begin(), vec2.end(), std::greater<>());
        
        if (vec1 == vec2) {
            hand.type = kvp.first;
            return;
        }
    }
}

template<typename C>
void sortHandsByType(std::vector<Hand> &hands, C comp, std::vector<char> powerTable) {
    CurrentTable = std::move(powerTable);
    std::sort(hands.begin(), hands.end(), comp);
}

bool Hand::checkMorePower (const Hand h, std::vector<char> powerTable) const {
    std::string cards1 = cards;
    std::string cards2 = h.cards;
    
    for (int i = 0; i < cards1.size(); i++) {
        auto pos1 = std::find(powerTable.begin(), powerTable.end(), cards1[i]);
        auto pos2 = std::find(powerTable.begin(), powerTable.end(), cards2[i]);
        if (pos1 < pos2) return false;
        if (pos1 > pos2) return true;
    }
    return false;
}

long calculateTotalWinnings(std::vector<Hand> hands) {
    long winnings = 0;
    for (int i = 0; i < hands.size(); i++) winnings += hands[i].bid * (i + 1);
    return winnings;
}