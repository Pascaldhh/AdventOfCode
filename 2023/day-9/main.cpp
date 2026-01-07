#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>

struct HistorySequence { 
    std::vector<long> numbers; 
    HistorySequence calcNextSequence();
    std::vector<HistorySequence> getAllSequences();
    bool allZero() {return std::all_of(numbers.begin(), numbers.end(), [](long num){return num == 0;}); };
    long getFutureReading();
    long getBackwardsReading();
};
struct History { std::vector<HistorySequence> sequences; };

History getHistory(std::ifstream &);


void partOne(History &);
void partTwo(History &);
int main() {
    std::ifstream input("../input.txt");
    if(!input.is_open()) return -1;

    History history = getHistory(input);

    partOne(history);
    partTwo(history);

    input.close();
    return 0;
}

void partOne(History &history) {
    int answer = 0;
    for (HistorySequence historySequence : history.sequences) 
         answer += historySequence.getFutureReading();
    std::cout << "Answer of part 1: " << answer << std::endl;
}

void partTwo(History &history) {
    int answer = 0;
    for (HistorySequence historySequence : history.sequences)
        answer += historySequence.getBackwardsReading();
    std::cout << "Answer of part 2: " << answer << std::endl;
}

History getHistory(std::ifstream &input) {
    History history;

    std::string line;
    while(std::getline(input, line)) {
        std::string number;
        std::stringstream ss(line);

        HistorySequence historySequence;
        while(std::getline(ss, number, ' '))
            historySequence.numbers.push_back(std::stol(number));
        
        history.sequences.push_back(historySequence);
    }
    return history;
}

HistorySequence HistorySequence::calcNextSequence() {
    HistorySequence sequence;
    for (int i = 0; i < numbers.size()-1; i++) 
        sequence.numbers.push_back(numbers[i+1] - numbers[i]);
    return sequence;
}

std::vector<HistorySequence> HistorySequence::getAllSequences() {
    std::vector<HistorySequence> sequences;
    
    HistorySequence current = *this;
    sequences.push_back(current);
    while(!current.allZero()) {
        current = current.calcNextSequence();
        sequences.push_back(current);
    }
    
    return sequences;
}
long HistorySequence::getFutureReading() {
    std::vector<HistorySequence> sequences = getAllSequences();
    
    sequences.back().numbers.push_back(0);
    for(int i = sequences.size()-2; i >= 0; i--)
        sequences[i].numbers.push_back(sequences[i+1].numbers.back() + sequences[i].numbers.back());
    
    
    return sequences.front().numbers.back();
}

long HistorySequence::getBackwardsReading() {
    std::vector<HistorySequence> sequences = getAllSequences();

    sequences.back().numbers.insert(sequences.back().numbers.begin(), 0);
    for(int i = sequences.size()-2; i >= 0; i--)
        sequences[i].numbers.insert(sequences[i].numbers.begin(), sequences[i].numbers.front() - sequences[i+1].numbers.front());
    
    return sequences.front().numbers.front();
}