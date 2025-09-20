#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <regex>
using namespace std;

struct Song {
    string title, artist;
    int duration; 
    Song* next;
    Song(string t, string a, int d) : title(t), artist(a), duration(d), next(nullptr) {}
};

class Playlist {
private:
    Song* head;

public:
    Playlist() : head(nullptr) {}

    // 1. Add new song
    void addSong() {
        string title, artist;
        int duration;
        cout << "Enter title: ";
        getline(cin >> ws, title);
        cout << "Enter artist: ";
        getline(cin >> ws, artist);

        while (true) {
        cout << "Enter duration(m:s) :";
        string input;
        getline(cin, input);

        regex pattern(R"(^(\d+):(\d{1,2})$)");
        smatch match;

        if (regex_match(input, match, pattern)) {
            int minutes = stoi(match[1]);
            int seconds = stoi(match[2]);

            if (seconds >= 60) {
                cout << "Seconds must be less than 60.\n";
                continue;
            }

            duration = minutes * 60 + seconds;
            break;
        } else {
            cout << "Invalid format.\n";
        }
    }

        Song* newSong = new Song(title, artist, duration);
        if (head == nullptr) {
            head = newSong;
        } else {
            Song* temp = head;
            while (temp->next) temp = temp->next;
            temp->next = newSong;
        }
        cout << "Song added!\n";
    }

    // 2. Delete song(s) by title
    void deleteSong() {
        if (head == nullptr) {
            cout << "Playlist is empty.\n";
            return;
        }

        string title;
        cout << "Enter title to delete: ";
        getline(cin >> ws, title);

        Song* temp = head;
        Song* prev = nullptr;
        bool found = false;

        while (temp != nullptr) {
            if (temp->title == title) {
                found = true;
                if (temp == head) {
                    head = head->next;
                    delete temp;
                    temp = head;
                } else {
                    prev->next = temp->next;
                    delete temp;
                    temp = prev->next;
                }
            } else {
                prev = temp;
                temp = temp->next;
            }
        }

        if (!found) cout << "No song titled '" << title << "' found.\n";
        else cout << "Song(s) deleted.\n";
    }

    // 3. Play next song
    void playNext() {
        if (head == nullptr) {
            cout << "Playlist is empty.\n";
            return;
        }

        Song* playing = head;
        head = head->next;

        if (head == nullptr) {
            cout << "Now playing: " << playing->title << " by " << playing->artist
                 << ". Remaining: 0 songs.\n";
            return;
        }

        // Move the playing song to the end
        Song* temp = head;
        while (temp->next != nullptr) temp = temp->next;
        temp->next = playing;
        playing->next = nullptr;

        // Count remaining songs
        int count = 0;
        temp = head;
        while (temp != nullptr) { count++; temp = temp->next; }

        cout << "Now playing: " << playing->title << " by " << playing->artist
             << ". Remaining: " << count << " songs.\n";
    }

    // 4. Display playlist
    void display() {
        if (head == nullptr) {
            cout << "Playlist is empty.\n";
            return;
        }

        Song* temp = head;
        int idx = 1;
        while (temp) {
        int totalSeconds = temp->duration;
        int minutes = totalSeconds / 60;
        int seconds = totalSeconds % 60;

        cout << idx++ << ". " << temp->title << " by " << temp->artist 
             << " [" << minutes << ":" << (seconds < 10 ? "0" : "") << seconds << "]\n";
        temp = temp->next;
    }
    }

    // 5. Shuffle playlist
    void shuffle() {
        if (head == nullptr || head->next == nullptr) {
            cout << "Playlist has less than 2 songs, cannot shuffle.\n";
            return;
        }

        vector<Song*> arr;
        Song* temp = head;
        while (temp != nullptr) { arr.push_back(temp); temp = temp->next; }

        srand(time(0));
        for (int i = arr.size() - 1; i > 0; i--) {
            int j = rand() % (i + 1);
            swap(arr[i], arr[j]);
        }

        for (size_t i = 0; i < arr.size() - 1; i++) arr[i]->next = arr[i + 1];
        arr.back()->next = nullptr;
        head = arr[0];

        cout << "Playlist shuffled.\n";
    }

    // 6. Reverse playlist
    void reverse() {
        if (head == nullptr) {
            cout << "Playlist is empty.\n";
            return;
        }

        Song* prev = nullptr;
        Song* curr = head;
        Song* next = nullptr;

        while (curr != nullptr) {
            next = curr->next;
            curr->next = prev;
            prev = curr;
            curr = next;
        }
        head = prev;

        cout << "Playlist order reversed.\n";
    }

    // Destructor
    ~Playlist() {
        while (head != nullptr) {
            Song* temp = head;
            head = head->next;
            delete temp;
        }
    }
};

// Main program
int main() {
    Playlist pl;
    int choice;

    do {
        cout << "\n==== Music Playlist Manager ====\n";
        cout << "1. Add New Song\n2. Delete Song by Title\n3. Play Next Song\n";
        cout << "4. Display Playlist\n5. Shuffle Playlist\n6. Reverse Playlist\n7. Exit\n";
        cout << "Enter your choice (1–7): ";
        cin >> choice; cin.ignore();

        switch (choice) {
            case 1: pl.addSong(); break;
            case 2: pl.deleteSong(); break;
            case 3: pl.playNext(); break;
            case 4: pl.display(); break;
            case 5: pl.shuffle(); break;
            case 6: pl.reverse(); break;
            case 7: cout << "Goodbye!\n"; break;
            default: cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 7);

    return 0;
}
