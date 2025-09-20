#include <iostream>
using namespace std;

void reverseArray(int A[], int start, int end) {
    while (start < end) {
        swap(A[start], A[end]);
        start++;
        end--;
    }
}

void rotateRight(int A[], int n, int k) {
    k = k % n;  // normalize k (in case k > n)

    // Step 1: Reverse whole array
    reverseArray(A, 0, n - 1);

    // Step 2: Reverse first k elements
    reverseArray(A, 0, k - 1);

    // Step 3: Reverse remaining elements
    reverseArray(A, k, n - 1);
}

int main() {
    cout<<"Inputs:\n";
    int n, k;
    cin >> n;              // size of array
    int A[n];
    
    for (int i = 0; i < n; i++) {
        cin >> A[i];       // array inputs
    }
    cin >> k;              // number of rotations

    rotateRight(A, n, k);
    
    cout<<"Output: ";
    for (int i = 0; i < n; i++) {
        cout << A[i] << " ";
    }
    return 0;
}
