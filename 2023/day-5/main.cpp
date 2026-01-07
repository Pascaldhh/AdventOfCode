#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <regex>
#include <sstream>
#include <map>
#include <algorithm>


enum Type { seed, soil, fertilizer, water, light, temperature, humidity, location, NONE };
const std::map<std::string, Type> typeTable {
    {"seed", Type::seed}, {"soil", Type::soil},
    {"fertilizer", Type::fertilizer}, {"water", Type::water},
    {"light", Type::light}, {"temperature", Type::temperature},
    {"humidity", Type::humidity}, {"location", Type::location},
};
struct Value { long num; Type type; bool operator < (const Value &other) const { return num < other.num; } };
struct ValueRange { long startNumber, endNumber; Type type; };
struct ConvertRule { 
    Type from, to; long destinationStart, sourceStart, rangeLength;
    long difference() const;
    bool check(Value&) const;
    std::string to_string() const;
};
struct Converter {
    std::vector<ConvertRule> rules;
    void convert(Value&, Type) const;
    std::vector<ValueRange> convert(ValueRange, Type);
    std::vector<ValueRange> convert(std::vector<ValueRange>, Type);
};

Type getType(std::string);
std::vector<Value> getSeeds(std::ifstream &);
std::vector<ConvertRule> getConvertRules(std::ifstream &);
std::vector<ValueRange> getSeedPairs(std::vector<Value>);

void partOne(std::vector<Value>, Converter &);
void partTwo(std::vector<Value>, Converter &);

int main() {
    std::ifstream input("../input.txt");
    if (!input.is_open()) return -1;

    std::vector<Value> seeds = getSeeds(input);
    Converter converter = {getConvertRules(input)};
    
    partOne(seeds, converter);
    partTwo(seeds, converter);
    
    input.close();
    return 0;
}

void partOne(std::vector<Value> seeds, Converter &converter) {
    for(Value &v: seeds)
        converter.convert(v, Type::location);
    
    std::forward_iterator auto min = std::ranges::min_element(seeds.begin(), seeds.end(), [](Value v1, Value v2){ return v1.num < v2.num; });
    std::cout << "Answer part 1: " << min->num << std::endl;
}

void partTwo(std::vector<Value> seeds, Converter &converter) {
    std::vector<ValueRange> seedPairs = getSeedPairs(seeds);
    
    auto calc = converter.convert(seedPairs, Type::location);
    
    auto answer = std::min_element(calc.begin(), calc.end(), [](ValueRange v1, ValueRange v2){ return v1.startNumber < v2.startNumber;});
    std::cout << "Answer part 2: " << answer->startNumber << std::endl;
}

Type getType(std::string s) {
    for(auto const &kvp : typeTable)
        if (s == kvp.first) return kvp.second;
    return Type::NONE;
}

std::vector<Value> getSeeds(std::ifstream &input) {
    input.clear();
    input.seekg(0, std::ios::beg);
    std::vector<Value> numbers;
    std::string line;
    if (getline(input, line))
        line = std::regex_replace(line, std::regex(R"(seeds: )"), "");

    std::stringstream ss(line);
    while(getline(ss, line, ' '))
        numbers.push_back(Value{std::stol(line), Type::seed});
    return numbers;
}

std::vector<ValueRange> getSeedPairs(std::vector<Value> seeds) {
    std::vector<ValueRange> pairs;
    for (int i = 0; i < seeds.size(); i += 2)
        pairs.push_back(ValueRange{ seeds[i].num, seeds[i].num + seeds[i+1].num-1, seeds[i].type });
    return pairs;
}

std::vector<ConvertRule> getConvertRules(std::ifstream &input) {
    std::vector<ConvertRule> converters;
    std::string line;
    while(getline(input, line)) {
        std::smatch match;
        if (std::regex_search(line, match, std::regex(R"((\w+)-to-(\w+) map:)"))) {
            std::string convertLine;
            while(getline(input, convertLine)) {
                if (convertLine.empty()) break;
                ConvertRule converter = {
                        getType(match.str(1)),
                        getType(match.str(2))
                };
                
                std::stringstream ss(convertLine);
                std::string num;
                for(int i = 0; getline(ss, num, ' '); i++) {
                    if (i == 0) converter.destinationStart = std::stol(num);
                    if (i == 1) converter.sourceStart = std::stol(num);
                    if (i == 2) converter.rangeLength = std::stol(num);
                }
                
                converters.push_back(converter);
            }
        }
    }
    return converters;
}

long ConvertRule::difference() const {
    return destinationStart - sourceStart;
}

bool ConvertRule::check(Value &value) const {
    if (sourceStart <= value.num && value.num <= sourceStart+rangeLength-1) {
        value.num = value.num + difference();
        return true;
    }
    return false;
}

std::string ConvertRule::to_string() const {
    return "Converter[From=" + std::to_string(from) + 
    ", To=" + std::to_string(to) + 
    ", DestinationStart=" + std::to_string(destinationStart) + 
    ", SourceStart=" + std::to_string(sourceStart) + 
    ", RangeLength=" + std::to_string(rangeLength) + 
    ", Difference()=" + std::to_string(difference()) +
    "]";
}

void Converter::convert(Value &v, Type t) const {
    while(v.type != t) {
        for(ConvertRule rule : rules) {
            if (rule.from == v.type && rule.check(v)) break;
        }
        v.type = (Type)(v.type+1);
    }
}

std::vector<ValueRange> glueRanges(std::vector<ValueRange> currentRanges, std::vector<ValueRange> changedRanges) {
    std::vector<ValueRange> allRanges;
    allRanges.insert( allRanges.end(), currentRanges.begin(), currentRanges.end() );
    allRanges.insert( allRanges.end(), changedRanges.begin(), changedRanges.end() );
    std::sort(allRanges.begin(), allRanges.end(), [](ValueRange v1, ValueRange v2){ return v1.startNumber < v2.startNumber; });

    std::vector<ValueRange> results;
    ValueRange r = allRanges[0];
    for (int i = 1; i < allRanges.size(); i++) {
        if (allRanges[i].startNumber-1 != r.endNumber) {
            results.push_back(r);
            r.startNumber = allRanges[i].startNumber;
        }
        r.endNumber = allRanges[i].endNumber;
    }
    results.push_back(r);

    return results;
}

std::vector<ValueRange> splitRanges(ValueRange range, std::vector<ValueRange> ranges) {
    std::vector<ValueRange> newRanges;
    std::sort(ranges.begin(), ranges.end(), [](ValueRange v1, ValueRange v2){ return v1.startNumber < v2.startNumber; });

    if (range.startNumber <= ranges[0].startNumber-1) newRanges.push_back(ValueRange{range.startNumber, ranges[0].startNumber-1, range.type});
    for (int i = 0; i < ranges.size(); i += 1) {
        if (i+1 > ranges.size()-1 || ranges[i].endNumber+1 == ranges[i+1].startNumber) continue;
        newRanges.push_back(ValueRange{ranges[i].endNumber+1, ranges[i+1].startNumber-1, range.type});
    }
    if (ranges[ranges.size()-1].endNumber != range.endNumber) newRanges.push_back(ValueRange{ranges[ranges.size()-1].endNumber+1, range.endNumber, range.type});
    return newRanges;
}

std::vector<ValueRange> Converter::convert(ValueRange range, Type t) {
    std::vector<ValueRange> ranges;
    std::vector<ValueRange> rangesCalculated;
    
    for(ConvertRule rule : rules) {
        if (rule.from != range.type) continue;
        
        bool ruleStartBetween = range.startNumber <= rule.sourceStart && rule.sourceStart <= range.endNumber;
        bool ruleEndBetween = range.startNumber <= rule.sourceStart+rule.rangeLength-1 && rule.sourceStart+rule.rangeLength-1 <= range.endNumber;
        bool rangeStartBetween = rule.sourceStart <= range.startNumber && range.startNumber <= rule.sourceStart+rule.rangeLength-1;
        bool rangeEndBetween = rule.sourceStart <= range.endNumber && range.endNumber <= rule.sourceStart+rule.rangeLength-1;

        ValueRange nVr = {};
        nVr.type = t;
        if (ruleStartBetween && ruleEndBetween) {
            nVr = {rule.sourceStart, rule.sourceStart + rule.rangeLength - 1};
        } else if (rangeStartBetween && rangeEndBetween) {
            nVr = {range.startNumber, range.endNumber};
        } else if (ruleStartBetween || rangeEndBetween) {
            nVr = {rule.sourceStart, range.endNumber};
        } else if (ruleEndBetween || rangeStartBetween) {
            nVr = {range.startNumber, rule.sourceStart + rule.rangeLength - 1};
        }
        
        if (ruleStartBetween || ruleEndBetween || rangeStartBetween || rangeEndBetween) {
            ranges.push_back(nVr);
            nVr.startNumber = nVr.startNumber + rule.difference();
            nVr.endNumber = nVr.endNumber + rule.difference();
            rangesCalculated.push_back(nVr);
        }
    }
    
    if (ranges.empty()) {
        range.type = (Type)(t+1);
        return {range};
    }
    
    ranges = glueRanges(splitRanges(range, ranges), rangesCalculated);
    for (auto &r : ranges)
        r.type = (Type) (t + 1);
    
    return ranges;
}

std::vector<ValueRange> Converter::convert(std::vector<ValueRange> ranges, Type t) {
    std::vector<ValueRange> results;
    
    for (ValueRange range : ranges) {
        std::vector<ValueRange> r = convert(range, range.type);
        results.insert( results.end(), r.begin(), r.end());
    }
    
    if (!std::all_of(results.begin(), results.end(), [t](ValueRange r){return t == r.type;}))
        return convert(results, t);
    
    return results;
}