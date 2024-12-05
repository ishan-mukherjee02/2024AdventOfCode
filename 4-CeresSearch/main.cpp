#include <iostream>
#include <fstream>

#include <vector>
#include <string>
#include <set>

#include <stdio.h>
#include <stdlib.h>

using namespace std;

vector<vector<char>> parseFileTo2DVector(const string& filename) {  
    vector<vector<char>> grid;

    ifstream file(filename);
    
    // Read character by character and populate the grid
    vector<char> row;
    char ch;
    while (file.get(ch)) {
        if (ch == '\n') {
            // End of row, add the row to the grid and reset the row vector
            if (!row.empty()) {
                grid.push_back(row);
                row.clear();
            }
        } else {
            row.push_back(ch);
        }
    }

    // Add the last row if the file doesn't end with a newline
    if (!row.empty()) {
        grid.push_back(row);
    }
    
    // Close the file
    file.close();
    
    return grid;
}

int search2D(vector<vector<char>> grid, int row, int col, string word) {
    unsigned int m = grid.size();
    unsigned int n = grid[0].size();
    
    if (grid[row][col] != word[0])
      return false;
 
    int len = word.size();
    int totalNum = 0;

    vector<int>x = { -1, -1, -1, 0, 0, 1, 1, 1 };
    vector<int>y = { -1, 0, 1, -1, 1, -1, 0, 1 };

    for (unsigned int dir = 0; dir < 8; dir++) {
      
        int k, currX = row + x[dir], currY = col + y[dir];
 
        for (k = 1; k < len; k++) {
          
            // break if out of bounds
            if (currX >= m || currX < 0 || currY >= n || currY < 0)
                break;
 
            if (grid[currX][currY] != word[k])
                break;
 
            currX += x[dir], currY += y[dir];
        }
 
        if (k == len)
            totalNum += 1;
    }
    
    return totalNum;
}

int searchWord(vector<vector<char>>grid, string word){
    unsigned int m = grid.size();
    unsigned int n = grid[0].size();
    
    vector<vector<int>>ans;
    int sum = 0;
    
    for(unsigned int i = 0; i < m; i++){
        for(unsigned int j = 0; j < n; j++){
            
            sum += search2D(grid, i, j, word);
        }
    }
    
    return sum;
}

bool findMAS(vector<vector<char>> grid, int row, int col, int direction[]) {
    string word = "MAS";

    for (int i = 0; i < word.size(); i++) {
        if ((row >= grid.size()) || (col >= grid[row].size()) || grid[row][col] != word[i]) {
            return false;
        }

        row += direction[0];
        col += direction[1];
    }
    return true;
}


int count_x_mas(const vector<vector<char>>& grid) {
    int rows = grid.size(), cols = grid[0].size();
    int count = 0;
    
    int directions[4][2]  = {
        { -1,  1 }, // 1 = ↗
        {  1, -1 }, // 5 = ↙
        {  1,  1 }, // 3 = ↘
        { -1, -1 }  // 7 = ↖
    };
    
    for (int i = 0; i < grid.size(); i++) {
        for(int j = 0; j < grid[0].size(); j++) {
            if (grid[i][j] == 'A') {
                if(
                    (
                        // check north east to south west
                        findMAS(grid, i + 1, j - 1, directions[0]) ||
                        findMAS(grid, i - 1, j + 1, directions[1])
                    ) && (
                        // check north west to south east
                        findMAS(grid, i - 1, j - 1, directions[2]) ||
                        findMAS(grid, i + 1, j + 1, directions[3])
                    )
                ) {
                    count++;
                }
            }
        }
    }
    
    return count;
}


int main() {
  vector<vector<char>> grid = parseFileTo2DVector("input.txt");

  string word = "MAS";
  
  cout << count_x_mas(grid) << endl;
  

}