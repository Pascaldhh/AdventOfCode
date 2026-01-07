#include <iostream>
#include <fstream>
#include <map>

enum class Direction {
    Top,
    TopRight,
    Right,
    BottomRight,
    Bottom,
    BottomLeft,
    Left,
    TopLeft
};

struct Coords {
    int x;
    int y;
    Coords(int x, int y) { this->x = x; this->y = y; }
    static Coords merge(Coords, Coords);
};

std::map<Direction, Coords> directionMapper {
    {Direction::Top, Coords(0, -1)},
    {Direction::TopRight, Coords(1, -1)},
    {Direction::Right, Coords(1, 0)},
    {Direction::BottomRight, Coords(1, 1)},
    {Direction::Bottom, Coords(0, 1)},
    {Direction::BottomLeft, Coords(-1, 1)},
    {Direction::Left, Coords(-1, 0)},
    {Direction::TopLeft, Coords(-1, -1)}
};

struct WordSearcher {
public:
    std::vector<std::string> field;

    int count(std::string);
    int countX(std::string);

    static WordSearcher parse(std::ifstream &);

private:
    int countWordsEveryDirectionOnCoords(std::string, Coords);
    bool isWord(std::string, Coords, Direction);
    bool isValidCoord(Coords);
};

void partOne(WordSearcher);
void partTwo(WordSearcher);
int main() {
    std::ifstream input("../input.txt");

    WordSearcher wordSearcher = WordSearcher::parse(input);

    partOne(wordSearcher);
    partTwo(wordSearcher);

    return 0;
}

void partOne(WordSearcher wordSearcher) {
    std::cout << "Answer part 1: " << wordSearcher.count("XMAS") << std::endl;
}

void partTwo(WordSearcher wordSearcher) {
    std::cout << "Answer part 2: " << wordSearcher.countX("MAS") << std::endl;
}

bool WordSearcher::isWord(std::string word, Coords current, Direction direction) {
    if(word.empty()) return true;
    if(isValidCoord(current) && word[0] == field[current.y][current.x]) {
        std::string newWord = word;
        newWord.erase(0, 1);
        Coords newCurrent = Coords::merge(current, directionMapper.at(direction));
        return isWord(newWord, newCurrent, direction);
    }

    return false;
}

int WordSearcher::countWordsEveryDirectionOnCoords(std::string word, Coords current) {
    int result = 0;
    for(std::pair<Direction, Coords> kvp : directionMapper) {
        if(isWord(word, current, kvp.first)) {
            result++;
        }
    }
    return result;
}

int WordSearcher::count(std::string word) {
    int result = 0;

    for(int y = 0; y < field.size(); y++) {
        for(int x = 0; x < field[y].length(); x++) {
            if(word[0] == field[y][x]) {
                result += countWordsEveryDirectionOnCoords(word, Coords(x, y));
            }
        }
    }
    return result;
}

int WordSearcher::countX(std::string word) {
    int result = 0;

    int middlePointWord = word.length() / 2;
    for(int y = middlePointWord; y < field.size()-middlePointWord; y++) {
        for(int x = middlePointWord; x < field[y].length()-middlePointWord; x++) {
            if(word[middlePointWord] == field[y][x]) {
                if((isWord(word, Coords(x+middlePointWord, y+middlePointWord), Direction::TopLeft) || isWord(word, Coords(x-middlePointWord, y-middlePointWord), Direction::BottomRight))
                && (isWord(word, Coords(x+middlePointWord, y-middlePointWord), Direction::BottomLeft) || isWord(word, Coords(x-middlePointWord, y+middlePointWord), Direction::TopRight))) {
                    result++;
                }
            }
        }
    }

    return result;
}

WordSearcher WordSearcher::parse(std::ifstream &input) {
    WordSearcher result{ .field = std::vector<std::string>() };

    std::string line;
    while(std::getline(input, line)) result.field.push_back(line);

    return result;
}

bool WordSearcher::isValidCoord(Coords coords) {
    return 0 <= coords.x && coords.x <= field[coords.y].length()-1
           && 0 <= coords.y && coords.y <= field.size()-1;
}

Coords Coords::merge(Coords coordsOne, Coords coordsTwo) {
    return {coordsOne.x + coordsTwo.x, coordsOne.y + coordsTwo.y};
}