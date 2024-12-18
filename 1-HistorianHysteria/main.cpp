#include <iostream>
#include <fstream>

#include <vector>
#include <unordered_set>

#include <algorithm>
#include <numeric>
#include <cmath>

#include <stdio.h>
#include <stdlib.h>

using namespace std;

int distances(vector<int> list1, vector<int> list2) {
  sort(list1.begin(), list1.end());
  sort(list2.begin(), list2.end());

  return inner_product(
        list1.begin(), list1.end(), list2.begin(), 0,
        plus<>(), [](int a, int b) { return abs(a - b); }
  );
}

int similarityScore(vector<int> list1, vector<int> list2) {
  unordered_set<int> set(list1.begin(), list1.end());
  int sum = 0;

  for (int num : list2) {
      if (set.find(num) != set.end()) {
          sum += num;
      }
  }

  return sum;
}

int main(int argc, char *argv[]) {
  ifstream input("input.txt");
 
  vector<int> list1, list2;
  int num1, num2;

  while(input >> num1 >> num2) {
    list1.push_back(num1);
    list2.push_back(num2); 
  }

  input.close();

  int answer = distances(list1, list2);

  cout << "Answer is: " << answer << "\n";

  int similarity = similarityScore(list1, list2);

  cout << "Similarity score is: " << similarity << "\n";

  return 0;
}