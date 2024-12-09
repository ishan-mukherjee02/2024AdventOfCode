#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <algorithm>
#include <regex>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

using namespace std;

int partOne(const string& filename) {
  unordered_map<int, vector<int>> rules;
  ifstream file(filename);

  string line;
  while( getline(file, line) ) {
    if(line == "") {
      break;
    }

    size_t delimiterPos = line.find('|'); // Find position of '|'

    string left = line.substr(0, delimiterPos); // Extract left part
    string right = line.substr(delimiterPos + 1); // Extract right part

    int leftNumber = stoi(left); // Convert to integer
    int rightNumber = stoi(right); // Convert to integer

    if(rules.find(leftNumber) != rules.end()) {
      vector<int> temp = rules[leftNumber];
      temp.push_back(rightNumber);
      rules[leftNumber] = temp;
    } else {
      vector<int> temp;
      temp.push_back(rightNumber);
      rules[leftNumber] = temp;
    }
  }
  
  int sum = 0;
  while( getline(file, line) ) {
    bool correct = true;
    unordered_set<int> prev;

    vector<string> words;
    boost::split(words, line, boost::is_any_of(","), boost::token_compress_on);
  
    for (auto& number : words)
    {

      int num = stoi(number);

      for (auto& x: rules[num]) {
        if (prev.find(x) != prev.end()) {
          // if find gives you an index for one of the nums that should come
          // after current, it means that it has shown up before, therefore
          // break and declare this not correct. TODO: how?
          correct = false;
          break;
        }
      }

      if(!correct) {
        break;
      }

      prev.insert(num);
    }

    if(correct) {
      auto halfway = words[words.size()/2];
      sum += stoi(halfway);
    }
  }

  file.close();
  
  return sum;
}

unordered_map<int, vector<int>> parseFile(const string& filename) {
  unordered_map<int, vector<int>> rules;
  ifstream file(filename);

  string line;
  while( getline(file, line) ) {
    if(line == "") {
      break;
    }

    size_t delimiterPos = line.find('|'); // Find position of '|'

    string left = line.substr(0, delimiterPos); // Extract left part
    string right = line.substr(delimiterPos + 1); // Extract right part

    int leftNumber = stoi(left); // Convert to integer
    int rightNumber = stoi(right); // Convert to integer

    if(rules.find(leftNumber) != rules.end()) {
      vector<int> temp = rules[leftNumber];
      temp.push_back(rightNumber);
      rules[leftNumber] = temp;
    } else {
      vector<int> temp;
      temp.push_back(rightNumber);
      rules[leftNumber] = temp;
    }
  } 

  return rules;
}

// Function to perform topological sorting on a directed graph
void dfs(int node, unordered_map<int, vector<int>>& graph, unordered_set<int>& visited, stack<int>& result) {
    if (visited.count(node)) return;
    visited.insert(node);
    
    for (int neighbor : graph[node]) {
        dfs(neighbor, graph, visited, result);
    }
    
    result.push(node);
}

// Function to build the subgraph restricted to numbers in the input vector
unordered_map<int, vector<int>> buildSubgraph(const vector<int>& nums, unordered_map<int, vector<int>>& graph) {
    unordered_map<int, vector<int>> subgraph;
    unordered_set<int> numSet(nums.begin(), nums.end());
    
    for (int num : nums) {
        if (graph.count(num)) {
            for (int neighbor : graph[num]) {
                if (numSet.count(neighbor)) {
                    subgraph[num].push_back(neighbor);
                }
            }
        }
    }
    
    return subgraph;
}

// Function to sort a vector according to precedence rules
vector<int> sortAccordingToRules(vector<int> nums, unordered_map<int, vector<int>>& precedenceRules) {
    // Step 1: Build the subgraph
    unordered_map<int, vector<int>> subgraph = buildSubgraph(nums, precedenceRules);
    
    // Step 2: Perform topological sort
    unordered_set<int> visited;
    stack<int> resultStack;
    
    for (int num : nums) {
        if (!visited.count(num)) {
            dfs(num, subgraph, visited, resultStack);
        }
    }
    
    // Step 3: Construct the result
    vector<int> sortedResult;
    while (!resultStack.empty()) {
        sortedResult.push_back(resultStack.top());
        resultStack.pop();
    }
    
    return sortedResult;
}

// Function to check if a line is empty or contains only whitespace
bool isEmptyLine(const string& line) {
    for (char ch : line) {
        if (!isspace(ch)) {
            return false; // Line has non-whitespace characters
        }
    }
    return true; // Line is empty or contains only whitespace
}

int partTwo(const string& filename) {
  unordered_map<int, vector<int>> rules = parseFile(filename);

  ifstream file(filename);
  string line;
  
  // Read through the file until an empty line is found
  while ( getline(file, line) ) {
      if (isEmptyLine(line)) {
          break; // Found an empty line
      }
  }

  int sum = 0;
  while( getline(file, line) ) {
    bool correct = true;
    unordered_set<int> prev;

    vector<string> words;
    boost::split(words, line, boost::is_any_of(","), boost::token_compress_on);

    vector<int> curLine;
    for (auto& number : words)
    {
      int num = stoi(number);
      curLine.push_back(num);

      for (auto& x: rules[num]) {
        if (prev.find(x) != prev.end()) {
          correct = false;
        }
      }

      prev.insert(num);
    }

    if(!correct) {
      curLine = sortAccordingToRules(curLine, rules);
      sum += curLine[curLine.size()/2];
    }
  }

  file.close();
  
  return sum;
}

int main() {
  cout << partTwo("input.txt") << endl;

    
  return 0;
}