#include <iostream>
#include <map>
using namespace std;

// Case 1: Sliding window for non-negative arrays
int longestSubarrayNonNegative(int A[], int n, int K) {
    int left = 0, sum = 0, maxLen = 0;
    for (int right = 0; right < n; right++) {
        sum += A[right];
        while (sum > K && left <= right) {
            sum -= A[left++];
        }
        maxLen = max(maxLen, right - left + 1);
    }
    return maxLen;
}

// Case 2: Prefix sum + map for arrays with negatives
int longestSubarrayWithNegatives(int A[], int n, int K) {
    multimap<long long, int> prefixMap; // prefixSum -> index
    prefixMap.insert({0, -1});          // base case

    long long prefixSum = 0;
    int maxLen = 0;

    for (int i = 0; i < n; i++) {
        prefixSum += A[i];

        // find smallest prefixSum >= prefixSum - K
        auto it = prefixMap.lower_bound(prefixSum - K);
        if (it != prefixMap.end()) {
            maxLen = max(maxLen, i - it->second);
        }

        prefixMap.insert({prefixSum, i});
    }
    return maxLen;
}

// Unified function: decides which algorithm to use
int longestSubarray(int A[], int n, int K) {
    bool hasNegative = false;
    for (int i = 0; i < n; i++) {
        if (A[i] < 0) {
            hasNegative = true;
            break;
        }
    }

    if (!hasNegative) {
        // O(n), O(1)
        return longestSubarrayNonNegative(A, n, K);
    } else {
        // O(n log n), O(n)
        return longestSubarrayWithNegatives(A, n, K);
    }
}

int main() {
    int n, K;
    cin >> n;
    int A[n];

    for (int i = 0; i < n; i++) cin >> A[i];
    cin >> K;

    cout << longestSubarray(A, n, K) << endl;
    return 0;
}
