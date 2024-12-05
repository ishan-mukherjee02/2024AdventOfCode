#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

vector<vector<char>> loadGridFromFile(const string& filename) {
    vector<vector<char>> grid;
    ifstream file(filename);
    
    vector<char> row;
    char ch;
    while (file.get(ch)) {
        if (ch == '\n') {
            if (!row.empty()) {
                grid.push_back(row);
                row.clear();
            }
        } else {
            row.push_back(ch);
        }
    }
    if (!row.empty()) grid.push_back(row);
    file.close();
    return grid;
}

int countWordOccurrencesInGrid(const vector<vector<char>>& grid, const string& word) {
    int m = grid.size(), n = grid[0].size(), totalCount = 0;
    vector<int> dx = { -1, -1, -1, 0, 0, 1, 1, 1 };
    vector<int> dy = { -1, 0, 1, -1, 1, -1, 0, 1 };
    
    for (int row = 0; row < m; row++) {
        for (int col = 0; col < n; col++) {
            if (grid[row][col] != word[0]) continue;
            
            for (int dir = 0; dir < 8; dir++) {
                int k, currX = row + dx[dir], currY = col + dy[dir];
                for (k = 1; k < word.size(); k++) {
                    if (currX < 0 || currX >= m || currY < 0 || currY >= n || grid[currX][currY] != word[k]) break;
                    currX += dx[dir];
                    currY += dy[dir];
                }
                if (k == word.size()) totalCount++;
            }
        }
    }
    return totalCount;
}

bool checkMASPattern(const vector<vector<char>>& grid, int row, int col, const int direction[]) {
    string word = "MAS";
    for (int i = 0; i < word.size(); i++) {
        if (row < 0 || row >= grid.size() || col < 0 || col >= grid[row].size() || grid[row][col] != word[i])
            return false;
        row += direction[0];
        col += direction[1];
    }
    return true;
}

int countMASCrosses(const vector<vector<char>>& grid) {
    int rows = grid.size(), cols = grid[0].size(), count = 0;
    int directions[4][2] = { { -1, 1 }, { 1, -1 }, { 1, 1 }, { -1, -1 } };
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (grid[i][j] == 'A') {
                if ((checkMASPattern(grid, i + 1, j - 1, directions[0]) || checkMASPattern(grid, i - 1, j + 1, directions[1])) &&
                    (checkMASPattern(grid, i - 1, j - 1, directions[2]) || checkMASPattern(grid, i + 1, j + 1, directions[3]))) {
                    count++;
                }
            }
        }
    }
    return count;
}

int main() {
    vector<vector<char>> grid = loadGridFromFile("input.txt");
    cout << countMASCrosses(grid) << endl;
}
