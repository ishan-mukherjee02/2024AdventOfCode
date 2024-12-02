#include <iostream>
#include <fstream>
#include <sstream>

#include <vector>

#include <algorithm>

using namespace std;

vector<vector<int>> parseFileTo2DVector(const string& filename) {
    vector<vector<int>> matrix;
    ifstream inputFile(filename);
    string line;

    if (!inputFile) {
        cerr << "Error: Could not open file " << filename << endl;
        return matrix;
    }

    // Read file line by line
    while (getline(inputFile, line)) {
        vector<int> row;
        stringstream ss(line);
        int num;

        // Split the line into integers
        while (ss >> num) {
            row.push_back(num);
        }

        // Add the row to the matrix
        matrix.push_back(row);
    }

    inputFile.close();
    return matrix;
}

int numSafe(const vector<int>& report) {
  if (report.size() < 2) return 1;

  bool is_increasing = adjacent_find(report.begin(), report.end(),
      [](int a, int b) { return b - a < 1 || b - a > 3; }) == report.end();

  bool is_decreasing = adjacent_find(report.begin(), report.end(),
      [](int a, int b) { return a - b < 1 || a - b > 3; }) == report.end();

  return (is_increasing || is_decreasing) ? 1 : 0;
}

int newNumSafe(const vector<int>& report) {
    if (report.size() < 2) return 1;

    auto isSafe = [](const vector<int>& r) {
        bool is_increasing = adjacent_find(r.begin(), r.end(),
            [](int a, int b) { return b - a < 1 || b - a > 3; }) == r.end();
        bool is_decreasing = adjacent_find(r.begin(), r.end(),
            [](int a, int b) { return a - b < 1 || a - b > 3; }) == r.end();
        return is_increasing || is_decreasing;
    };


    if (isSafe(report)) return 1;

    for (size_t i = 0; i < report.size(); ++i) {
        vector<int> modifiedReport = report;
        modifiedReport.erase(modifiedReport.begin() + i); 
        if (isSafe(modifiedReport)) return 1; 
    }


    return 0;
} 

int main() {
  vector<vector<int>> reports = parseFileTo2DVector("input.txt");

  int sum = 0;

  for(const auto& report: reports) {
    sum += newNumSafe(report);
  }

  cout << sum << "\n";

}