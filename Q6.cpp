#include <iostream>
#include <string>
#include <regex>
using namespace std;

struct Node {
    int ID;             
    string name;         
    int burst;          
    int priority;        
    Node* next;          
};

class RoundRobinScheduler {
private:
    Node* head;
    Node* tail;
    int quantum;

public:
    RoundRobinScheduler(int q = 10) {
        head = tail = nullptr;
        quantum = q;
    }

    void addTask() {
        int id, priority;
        string name;
        string burstStr;

        cout << "Enter ID: ";
        cin >> id;
        cout << "Enter name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter burst time (minutes:seconds): ";
        cin >> burstStr;

        regex timeRegex(R"((\d+):(\d+))");
        smatch match;
        int burst = 0;

        if (regex_match(burstStr, match, timeRegex)) {
            int minutes = stoi(match[1]);
            int seconds = stoi(match[2]);
            burst = minutes * 60 + seconds;
        } else {
            cout << "Invalid time format! Use mm:ss (e.g. 2:30)\n";
            return;
        }

        cout << "Enter priority (1–10): ";
        cin >> priority;

        if (burst <= 0 || priority < 1 || priority > 10) {
            cout << "Invalid burst time or priority.\n";
            return;
        }

        Node* temp = head;
        if (temp != nullptr) {
            do {
                if (temp->ID == id) {
                    cout << "Task with this ID already exists.\n";
                    return;
                }
                temp = temp->next;
            } while (temp != head);
        }

        Node* newNode = new Node{id, name, burst, priority, nullptr};

        if (head == nullptr) {
            head = tail = newNode;
            newNode->next = head; 
        } else {
            tail->next = newNode;
            tail = newNode;
            tail->next = head;
        }

        cout << "Task added successfully.\n";
    }

    void deleteTask(int ID) {
        if (head == nullptr) {
            cout << "No tasks to delete.\n";
            return;
        }
        Node* curr = head;
        Node* prev = tail;
        do {
            if (curr->ID == ID) {
                if (curr == head && curr == tail) { 
                    delete curr;
                    head = tail = nullptr;
                } else {
                    prev->next = curr->next;
                    if (curr == head) head = head->next;
                    if (curr == tail) tail = prev;
                    delete curr;
                }
                cout << "Task with ID " << ID << " deleted.\n";
                return;
            }
            prev = curr;
            curr = curr->next;
        } while (curr != head);
        cout << "Task with ID " << ID << " not found.\n";
    }

    void executeNext() {
        if (head == nullptr) {
            cout << "No tasks to execute.\n";
            return;
        }
        head->burst -= quantum;
        cout << "Executing Task [" << head->ID << "] " << head->name
             << " for " << quantum << "ms.\n";
        if (head->burst <= 0) {
            cout << "Task [" << head->ID << "] finished.\n";
            deleteTask(head->ID);
        } else {
            tail = head;
            head = head->next;
        }
    }

    void displayTasks() {
        if (head == nullptr) {
            cout << "No tasks in scheduler.\n";
            return;
        }
        Node* temp = head;
        int index = 1;
        do {
            int minutes = temp->burst / 60;
            int seconds = temp->burst % 60;
            cout << index++ << ". [" << temp->ID << "] " << temp->name
                 << " (burst: " << minutes << "m " << seconds << "s, pri: " << temp->priority << ")\n";
            temp = temp->next;
        } while (temp != head);
    }

    void splitByPriority(int threshold) {
        if (head == nullptr) {
            cout << "No tasks to split.\n";
            return;
        }
        Node* temp = head;
        cout << "High Priority (>= " << threshold << "):\n";
        bool hasHigh = false, hasLow = false;
        do {
            if (temp->priority >= threshold) {
                cout << "[" << temp->ID << "] " << temp->name
                     << " (burst: " << temp->burst << "ms, pri: " << temp->priority << ")\n";
                hasHigh = true;
            }
            temp = temp->next;
        } while (temp != head);
        if (!hasHigh) cout << "Empty\n";

        temp = head;
        cout << "Low Priority (< " << threshold << "):\n";
        do {
            if (temp->priority < threshold) {
                cout << "[" << temp->ID << "] " << temp->name
                     << " (burst: " << temp->burst << "ms, pri: " << temp->priority << ")\n";
                hasLow = true;
            }
            temp = temp->next;
        } while (temp != head);
        if (!hasLow) cout << "Empty\n";
    }

    void clearAll() {
        if (head == nullptr) {
            cout << "No tasks to clear.\n";
            return;
        }
        Node* curr = head;
        do {
            Node* nextNode = curr->next;
            delete curr;
            curr = nextNode;
        } while (curr != head);
        head = tail = nullptr;
        cout << "All tasks cleared.\n";
    }

    bool findTask(int ID) {
        if (head == nullptr) return false;
        Node* temp = head;
        do {
            if (temp->ID == ID) return true;
            temp = temp->next;
        } while (temp != head);
        return false;
    }

    void menu() {
        int choice;
        do {
            cout << "\n==== Round-Robin Task Scheduler ====\n";
            cout << "1. Add New Task\n";
            cout << "2. Delete Task by ID\n";
            cout << "3. Execute Next Task (rotate / run quantum)\n";
            cout << "4. Display Tasks\n";
            cout << "5. Split Tasks by Priority\n";
            cout << "6. Clear All Tasks\n";
            cout << "7. Exit\n";
            cout << "Enter your choice (1–7): ";
            cin >> choice;

            switch (choice) {
                case 1:
                    addTask();
                    break;
                case 2: {
                    int ID;
                    cout << "Enter ID to delete: ";
                    cin >> ID;
                    deleteTask(ID);
                    break;
                }
                case 3:
                    executeNext();
                    break;
                case 4:
                    displayTasks();
                    break;
                case 5: {
                    int threshold;
                    cout << "Enter threshold: ";
                    cin >> threshold;
                    splitByPriority(threshold);
                    break;
                }
                case 6:
                    clearAll();
                    break;
                case 7:
                    cout << "Exiting...\n";
                    break;
                default:
                    cout << "Invalid choice. Please try again.\n";
            }
        } while (choice != 7);
    }
};

int main() {
    RoundRobinScheduler scheduler(10);
    scheduler.menu();
    return 0;
}
