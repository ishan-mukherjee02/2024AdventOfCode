#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <boost/algorithm/string.hpp>

using namespace std;

// Function to parse rules from the file
unordered_map<int, vector<int>> parseRules(ifstream& file) {
    unordered_map<int, vector<int>> rules;
    string line;

    while (getline(file, line)) {
        if (line.empty()) break;

        size_t delimiterPos = line.find('|');
        if (delimiterPos == string::npos) continue; // Skip malformed lines

        int leftNumber = stoi(line.substr(0, delimiterPos));
        int rightNumber = stoi(line.substr(delimiterPos + 1));

        rules[leftNumber].push_back(rightNumber);
    }

    return rules;
}

// Function to perform DFS for topological sorting
void dfs(int node, const unordered_map<int, vector<int>>& graph, unordered_set<int>& visited, stack<int>& result) {
    if (visited.count(node)) return;
    visited.insert(node);

    if (graph.count(node)) {
        for (int neighbor : graph.at(node)) {
            dfs(neighbor, graph, visited, result);
        }
    }

    result.push(node);
}

// Function to build a subgraph restricted to the given numbers
unordered_map<int, vector<int>> buildSubgraph(const vector<int>& nums, const unordered_map<int, vector<int>>& graph) {
    unordered_map<int, vector<int>> subgraph;
    unordered_set<int> numSet(nums.begin(), nums.end());

    for (int num : nums) {
        if (graph.count(num)) {
            for (int neighbor : graph.at(num)) {
                if (numSet.count(neighbor)) {
                    subgraph[num].push_back(neighbor);
                }
            }
        }
    }

    return subgraph;
}

// Function to sort numbers according to precedence rules using topological sorting
vector<int> sortAccordingToRules(const vector<int>& nums, const unordered_map<int, vector<int>>& precedenceRules) {
    auto subgraph = buildSubgraph(nums, precedenceRules);

    unordered_set<int> visited;
    stack<int> resultStack;

    for (int num : nums) {
        if (!visited.count(num)) {
            dfs(num, subgraph, visited, resultStack);
        }
    }

    vector<int> sortedResult;
    while (!resultStack.empty()) {
        sortedResult.push_back(resultStack.top());
        resultStack.pop();
    }

    return sortedResult;
}

// Function to process lines based on rules for "part one"
int partOne(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << filename << endl;
        return -1;
    }

    auto rules = parseRules(file);
    string line;
    int sum = 0;

    while (getline(file, line)) {
        vector<string> words;
        boost::split(words, line, boost::is_any_of(","), boost::token_compress_on);

        bool correct = true;
        unordered_set<int> prev;

        for (const string& word : words) {
            int num = stoi(word);

            for (int dependent : rules[num]) {
                if (prev.count(dependent)) {
                    correct = false;
                    break;
                }
            }

            if (!correct) break;
            prev.insert(num);
        }

        if (correct) {
            sum += stoi(words[words.size() / 2]);
        }
    }

    return sum;
}

// Function to process lines based on rules for "part two"
int partTwo(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << filename << endl;
        return -1;
    }

    auto rules = parseRules(file);
    string line;
    int sum = 0;

    while (getline(file, line)) {
        vector<string> words;
        boost::split(words, line, boost::is_any_of(","), boost::token_compress_on);

        vector<int> nums;
        for (const string& word : words) {
            nums.push_back(stoi(word));
        }

        // Validate and sort if necessary
        bool correct = true;
        unordered_set<int> prev;

        for (int num : nums) {
            for (int dependent : rules[num]) {
                if (prev.count(dependent)) {
                    correct = false;
                    break;
                }
            }

            if (!correct) break;
            prev.insert(num);
        }

        if (!correct) {
            nums = sortAccordingToRules(nums, rules);
          sum += nums[nums.size() / 2];
        }

    }

    return sum;
}

int main() {
    cout << "Part One: " << partOne("input.txt") << endl;
    cout << "Part Two: " << partTwo("input.txt") << endl;

    return 0;
}