#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <regex>
#include <algorithm>

struct Node { std::string node; };
struct FromToNode { Node currentNode; std::tuple<Node, Node> toNodes; };
struct Map {
    std::string instructions; 
    std::vector<FromToNode> fromToNodes; 
};

Map getMap(std::ifstream &);

long calculateSteps(Map, const Node &, const Node &);
long long int calculateStepsBruteForce(Map, std::vector<Node>, std::vector<Node>);
long long int calculateStepsSmart(Map, std::vector<Node>, std::vector<Node>);
std::vector<Node> findNodesOnEnding(Map, char);

void partOne(Map);
void partTwo(Map);
int main() {
    std::ifstream input("./input.txt");
    if(!input.is_open()) return -1;
    
    Map map = getMap(input);

    partOne(map);
    partTwo(map);

    input.close();
    return 0;
}

void partOne(Map map) {
    std::cout << "Answer part 1: " << calculateSteps(std::move(map), Node{"AAA"}, Node{"ZZZ"}) << std::endl;
}

void partTwo(Map map) {
    std::cout << "Answer part 2: " << calculateStepsSmart(map, findNodesOnEnding(map, 'A'), findNodesOnEnding(map, 'Z')) << std::endl;
}

Map getMap(std::ifstream &input) {
    Map map;
    
    std::string line;
    for(int i = 0; getline(input, line); i++) {
        if(i == 0) map.instructions = line;
        if (line.empty() || i == 0) continue;
        
        FromToNode fromToNode;
        std::smatch match;
        std::regex_search(line, match, std::regex(R"((\w+) = \((\w+), (\w+)\))"));
        fromToNode.currentNode = Node{match.str(1)};
        fromToNode.toNodes = std::make_tuple(Node{match.str(2)}, Node{match.str(3)});
        map.fromToNodes.push_back(fromToNode);
    }
    
    return map;
}

long calculateSteps(Map map, const Node &startNode, const Node &endNode) {
    long steps = 0;
    Node currentNode = startNode;
    while(currentNode.node != endNode.node) {
        for (int i = 0; i < map.instructions.size(); i++) {
            auto result = std::find_if(map.fromToNodes.begin(), map.fromToNodes.end(), [currentNode](const FromToNode &fromToNode) {return fromToNode.currentNode.node == currentNode.node; });
            switch (map.instructions[i]) {
                case 'L': currentNode = std::get<0>(result->toNodes); break;
                case 'R': currentNode = std::get<1>(result->toNodes); break;
            }
            steps++;
        }
    }
    return steps;
}

bool allStepsOnEndNode(const std::vector<Node> &startNodes, const std::vector<Node> &endNodes) {
    int amountEndReached = 0;
    for (const Node &currentNode : startNodes) {
        if (std::any_of(endNodes.begin(), endNodes.end(), [currentNode](const Node &n) {return currentNode.node == n.node; }))
            amountEndReached++;
    }
    return amountEndReached == startNodes.size();
}

long long int calculateStepsBruteForce(Map map, std::vector<Node> startNodes, std::vector<Node> endNodes) {
    long steps = 0;
    
    while(!allStepsOnEndNode(startNodes, endNodes)) {
        for (char instruction : map.instructions) {
            for (Node &currentNode: startNodes) {
                auto result = std::find_if(map.fromToNodes.begin(), map.fromToNodes.end(), [currentNode](const FromToNode &fromToNode) {return fromToNode.currentNode.node == currentNode.node; });
                switch (instruction) {
                    case 'L': currentNode = std::get<0>(result->toNodes); break;
                    case 'R': currentNode = std::get<1>(result->toNodes); break;
                }
            }
            steps++;
        }
    }
    return steps;
}

long long lcmOfVector(std::vector<long long> vec) {
    long long ans = vec[0];

    for (int i = 1; i < vec.size(); i++)
        ans = (((vec[i] * ans)) /
               (std::__gcd(vec[i], ans)));

    return ans;
}

long long int calculateStepsSmart(Map map, std::vector<Node> startNodes, std::vector<Node> endNodes) {
    std::map<std::string, long> stepsForEachEndNode;

    std::vector<Node> currentNodes = startNodes;
    for (int i = 0; i < currentNodes.size(); i++) {
        Node currentNode = currentNodes[i];
        while(!allStepsOnEndNode(std::vector<Node>{currentNode}, endNodes)) {
            for (char instruction : map.instructions) {
                auto result = std::find_if(map.fromToNodes.begin(), map.fromToNodes.end(), [currentNode](const FromToNode &fromToNode) {return fromToNode.currentNode.node == currentNode.node; });
                switch (instruction) {
                    case 'L': currentNode = std::get<0>(result->toNodes); break;
                    case 'R': currentNode = std::get<1>(result->toNodes); break;
                }
                stepsForEachEndNode[startNodes[i].node]++;
            }
        }
    }

    std::vector<long long int> values;
    for (const auto &kvp : stepsForEachEndNode)
        values.push_back(kvp.second);


    return lcmOfVector(values);
}

std::vector<Node> findNodesOnEnding(Map map, char c) {
    std::vector<Node> findings;
    for (FromToNode fromToNode : map.fromToNodes) {
        if (fromToNode.currentNode.node.back() == c) 
            findings.push_back(fromToNode.currentNode);
    }
    return findings;
}
