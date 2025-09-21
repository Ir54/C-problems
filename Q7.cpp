#include <bits/stdc++.h>
using namespace std;

struct Node {
    int data;
    Node* prev;
    Node* next;
    Node(int val) : data(val), prev(nullptr), next(nullptr) {}
};

class DoublyLinkedList {
    Node* head;
    Node* tail;
    int sz;

    Node* getNode(int index) {
        if (index < 0 || index >= sz) return nullptr;
        Node* cur;
        if (index < sz / 2) {
            cur = head;
            for (int i = 0; i < index; i++) cur = cur->next;
        } else {
            cur = tail;
            for (int i = sz - 1; i > index; i--) cur = cur->prev;
        }
        return cur;
    }

    Node* merge(Node* left, Node* right) {
        if (!left) return right;
        if (!right) return left;
        if (left->data <= right->data) {
            left->next = merge(left->next, right);
            left->next->prev = left;
            left->prev = nullptr;
            return left;
        } else {
            right->next = merge(left, right->next);
            right->next->prev = right;
            right->prev = nullptr;
            return right;
        }
    }

    Node* split(Node* head) {
        Node* fast = head, *slow = head;
        while (fast->next && fast->next->next) {
            fast = fast->next->next;
            slow = slow->next;
        }
        Node* temp = slow->next;
        slow->next = nullptr;
        if (temp) temp->prev = nullptr;
        return temp;
    }

    Node* mergeSortRec(Node* node) {
        if (!node || !node->next) return node;
        Node* second = split(node);
        node = mergeSortRec(node);
        second = mergeSortRec(second);
        return merge(node, second);
    }

public:
    DoublyLinkedList() : head(nullptr), tail(nullptr), sz(0) {}

    void insertAt(int index, int value) {
        if (index < 0 || index > sz) {
            cout << "Invalid index.\n";
            return;
        }
        Node* cur = head;
        while (cur) {
            if (cur->data == value) {
                cout << "Value already exists. Duplicate not allowed.\n";
                return;
            }
            cur = cur->next;
        }
        Node* newNode = new Node(value);
        if (sz == 0) {
            head = tail = newNode;
        } else if (index == 0) {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        } else if (index == sz) {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        } else {
            Node* cur = getNode(index);
            newNode->prev = cur->prev;
            newNode->next = cur;
            cur->prev->next = newNode;
            cur->prev = newNode;
        }
        sz++;
        cout << "Inserted " << value << " at index " << index << ".\n";
    }

    void deleteRange(int l, int r) {
        if (l < 0 || r >= sz || l > r) {
            cout << "Invalid range.\n";
            return;
        }
        Node* start = getNode(l);
        Node* end = getNode(r);
        Node* before = start->prev;
        Node* after = end->next;
        Node* cur = start;
        while (cur != after) {
            Node* nxt = cur->next;
            delete cur;
            sz--;
            if (cur == end) break;
            cur = nxt;
        }
        if (before) before->next = after; else head = after;
        if (after) after->prev = before; else tail = before;
        cout << "Deleted range [" << l << ".." << r << "].\n";
    }

    void splice(int l1, int r1, int pos2) {
        if (l1 < 0 || r1 >= sz || l1 > r1 || pos2 < 0 || pos2 > sz) {
            cout << "Invalid splice params.\n";
            return;
        }
        if (pos2 >= l1 && pos2 <= r1 + 1) {
            cout << "Invalid splice: overlapping.\n";
            return;
        }
        Node* start = getNode(l1);
        Node* end = getNode(r1);
        Node* before = start->prev;
        Node* after = end->next;
        if (before) before->next = after; else head = after;
        if (after) after->prev = before; else tail = before;
        if (pos2 == 0) {
            start->prev = nullptr;
            end->next = head;
            if (head) head->prev = end;
            head = start;
        } else if (pos2 == sz) {
            tail->next = start;
            start->prev = tail;
            end->next = nullptr;
            tail = end;
        } else {
            Node* posNode = getNode(pos2);
            Node* posPrev = posNode->prev;
            posPrev->next = start;
            start->prev = posPrev;
            end->next = posNode;
            posNode->prev = end;
        }
        cout << "Spliced range [" << l1 << ".." << r1 << "] before index " << pos2 << ".\n";
    }

    void reverseRange(int l, int r) {
        if (l < 0 || r >= sz || l > r) {
            cout << "Invalid range.\n";
            return;
        }
        Node* left = getNode(l);
        Node* right = getNode(r);
        while (left != right && left->prev != right) {
            swap(left->data, right->data);
            left = left->next;
            right = right->prev;
        }
        cout << "Reversed range [" << l << ".." << r << "].\n";
    }

    void mergeSort() {
        if (!head || !head->next) return;
        head = mergeSortRec(head);
        tail = head;
        while (tail->next) tail = tail->next;
        cout << "List sorted in ascending order.\n";
    }

    void display() {
        Node* cur = head;
        int idx = 0;
        while (cur) {
            cout << idx << ": " << cur->data << "\n";
            cur = cur->next;
            idx++;
        }
        if (sz == 0) cout << "Sequence is empty.\n";
    }

    void clear() {
        Node* cur = head;
        while (cur) {
            Node* nxt = cur->next;
            delete cur;
            cur = nxt;
        }
        head = tail = nullptr;
        sz = 0;
        cout << "Sequence cleared.\n";
    }
};

int main() {
    DoublyLinkedList dll;
    int choice;
    while (true) {
        cout << "==== Doubly Linked Sequence Manager ====\n";
        cout << "1. Insert at index\n";
        cout << "2. Delete range [l..r]\n";
        cout << "3. Splice range [l1..r1] before index pos2\n";
        cout << "4. Reverse range [l..r]\n";
        cout << "5. Stable merge sort (ascending)\n";
        cout << "6. Display sequence\n";
        cout << "7. Clear sequence\n";
        cout << "8. Exit\n";
        cout << "Enter your choice (1–8): ";
        cin >> choice;
        if (choice == 1) {
            int idx, val;
            cout << "Enter index and value: ";
            cin >> idx >> val;
            dll.insertAt(idx, val);
        } else if (choice == 2) {
            int l, r;
            cout << "Enter l and r: ";
            cin >> l >> r;
            dll.deleteRange(l, r);
        } else if (choice == 3) {
            int l1, r1, pos2;
            cout << "Enter l1 r1 pos2: ";
            cin >> l1 >> r1 >> pos2;
            dll.splice(l1, r1, pos2);
        } else if (choice == 4) {
            int l, r;
            cout << "Enter l and r: ";
            cin >> l >> r;
            dll.reverseRange(l, r);
        } else if (choice == 5) {
            dll.mergeSort();
        } else if (choice == 6) {
            dll.display();
        } else if (choice == 7) {
            dll.clear();
        } else if (choice == 8) {
            cout << "Exiting...\n";
            break;
        } else {
            cout << "Invalid choice. Please try again.\n";
        }
    }
    return 0;
}
