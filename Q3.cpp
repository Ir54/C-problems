#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>
using namespace std;

int readArray(vector<int>& nums) {
    string line;
    getline(cin, line);
    stringstream ss(line);
    int temp;

    while (ss >> temp) {
        nums.push_back(temp);
    }
    return nums.size();
}

int firstMissingPositive(vector<int>& nums) {
    int n = nums.size();

    // Step 1: Place each number at its correct index if possible
    for (int i = 0; i < n; i++) {
        while (nums[i] > 0 && nums[i] <= n && nums[nums[i] - 1] != nums[i]) {
            swap(nums[i], nums[nums[i] - 1]);
        }
    }

    // Step 2: Find first missing positive
    for (int i = 0; i < n; i++) {
        if (nums[i] != i + 1) {
            return i + 1;
        }
    }

    // Step 3: If all positions filled correctly, missing is n+1
    return n + 1;
}

int main() {
    vector<int> nums;   
    int n = readArray(nums); 

    cout << "Input: nums = [";
    for (int i = 0; i < n; i++) {
        cout << nums[i];
        if (i != n - 1) cout << ",";
    }
    cout << "]" << endl;

    int result = firstMissingPositive(nums);
    cout << "Output: " << result << endl;

    return 0;
}
