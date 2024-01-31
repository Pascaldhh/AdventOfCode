#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <regex>
#include <string>
#include <map>

struct Card {
    int id;
    std::vector<int> winningNumbers;
    std::vector<int> myNumbers;
    std::vector<int> myWinningNumbers() {
        std::vector<int> r;
        for (int winNum : winningNumbers) {
            for (int myNum : myNumbers) {
                if (winNum == myNum) r.push_back(myNum); 
            }
        }
        return r;
    }
    int points() {
        int result = 0;
        int count = (int)myWinningNumbers().size();
        for (int i = 0; i < count; i++) {
            result = result * 2;
            if (result == 0) result = 1;
        }
        return result;
    }
};

std::vector<Card> getCards(std::ifstream &);

void partOne(std::vector<Card> &);
void partTwo(std::vector<Card> &);


int main() {
    std::ifstream input("../input.txt");
    if (!input.is_open()) return -1;
    
    std::vector<Card> cards = getCards(input);
    
    partOne(cards);
    partTwo(cards);
    
    input.close();
    return 0;
}

void partOne(std::vector<Card> &cards) {
    int sum = 0;
    for(Card card : cards) sum += card.points();
    std::cout << "Answer part 1: " << sum << std::endl;
}

void insertOrUpdate(std::map<int, int> &m, Card &card, int val) {
    if (m.find(card.id) != m.end()) m[card.id] += val;
    else m.insert({card.id, val});
}

void partTwo(std::vector<Card> &cards) {
    std::map<int, int> cardAmount;
    for(int i = 0; i < cards.size(); i++) {
        insertOrUpdate(cardAmount, cards[i], 1);
        
        for (int k = 1; k <= cards[i].myWinningNumbers().size(); k++) 
            insertOrUpdate(cardAmount, cards[i + k], 1*cardAmount[cards[i].id]);
    }
    
    int sum = 0;
    for (auto const &kvp : cardAmount)sum += kvp.second;
    std::cout << "Answer part 2: " << sum << std::endl;
}

std::vector<Card> getCards(std::ifstream &input) {
    std::vector<Card> cards;
    std::string line;
    while(getline(input, line)) {
        std::smatch match;
        std::regex_search(line, match, std::regex(R"(Card\s+(\d+):)"));
        Card card;
        card.id = std::stoi(match.str(1));

        line = std::regex_replace(line, std::regex(R"(Card\s+\d+:)"), "");
        std::stringstream ss(line);
        std::string numbers;
        for(int i = 0; std::getline(ss, numbers, '|'); i++) {
            std::stringstream ssinner(numbers);
            std::string number;
            while(std::getline(ssinner, number, ' ')) {
                if (number.empty()) continue;
                if (i == 0) card.winningNumbers.push_back(std::stoi(number));
                if (i == 1) card.myNumbers.push_back(std::stoi(number));
            }
        }
        cards.push_back(card);
    }
    return cards;
}

