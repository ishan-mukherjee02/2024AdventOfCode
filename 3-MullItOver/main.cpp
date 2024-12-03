#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <numeric>

using namespace std;

string parseFileToString(const string& filename) {
    vector<vector<int>> matrix;
    ifstream inputFile(filename);
    string line;

    stringstream buffer;
    buffer << inputFile.rdbuf();

    inputFile.close();

    string fileContent = buffer.str();

    return fileContent;
}

int multiplyNums(const string& input) {
  size_t openParen = input.find('(');
  size_t comma = input.find(',');
  size_t closeParen = input.find(')');

  string num1Str = input.substr(openParen + 1, comma - openParen - 1);
  string num2Str = input.substr(comma + 1, closeParen - comma - 1);

  int num1 = stoi(num1Str);
  int num2 = stoi(num2Str);

  int result = num1 * num2;

  return result;
}

int partOne(const string& input) {
  regex pattern(R"(mul\([0-9]{1,4},[0-9]{1,4}\))");

  auto words_begin = sregex_iterator(input.begin(), input.end(), pattern);
  auto words_end = sregex_iterator();

  int sum = 0;
  for (sregex_iterator i = words_begin; i != words_end; ++i)
  {
      smatch match = *i;
      string match_str = match.str();
      sum += multiplyNums(match_str);
  }
  return sum;
}

vector<string> split(const string& str, const string& delimiter) {
    vector<string> parts;
    size_t start = 0;
    size_t end = str.find(delimiter);

    while (end != string::npos) {
        parts.push_back(str.substr(start, end - start));
        start = end + delimiter.length();
        end = str.find(delimiter, start);
    }
    parts.push_back(str.substr(start));
    return parts;
}

int partTwo(const string& s) {
    vector<string> do_substrs = split(s, "do()");

    int total_sum = 0;
    for (const string& substr : do_substrs) {
        vector<string> valid_parts = split(substr, "don't()");
        if (!valid_parts.empty()) {
            total_sum += partOne(valid_parts[0]);
        }
    }

    return total_sum;
}

int main() {
  string text = parseFileToString("input.txt");

  cout << "Sum is: " << partOne(text) << endl;
  cout << "New sum is: " << partTwo(text) << endl;

  return 0;
}