#include <iostream>
#include <string>

using namespace std;

// Node structure storing song details
struct SongNode {
    int songID;
    string songName;
    string duration; // Format e.g. "3:45"
    SongNode* prev;
    SongNode* next;

    SongNode(int id, string name, string dur) 
        : songID(id), songName(name), duration(dur), prev(nullptr), next(nullptr) {}
};

class Playlist {
private:
    SongNode* head;
    SongNode* tail;
    SongNode* currentTrack;

public:
    Playlist() : head(nullptr), tail(nullptr), currentTrack(nullptr) {}

    ~Playlist() {
        SongNode* current = head;
        while (current != nullptr) {
            SongNode* nextNode = current->next;
            delete current;
            current = nextNode;
        }
    }

    // 1. Add Song - Insert at the end of the playlist
    void addSong(int id, string name, string duration) {
        SongNode* newNode = new SongNode(id, name, duration);
        if (head == nullptr) {
            head = tail = newNode;
            currentTrack = head;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        cout << "Song '" << name << "' added successfully!\n";
    }

    // 2. Delete Song - Remove a song by its ID
    void deleteSong(int id) {
        if (head == nullptr) {
            cout << "Playlist is empty.\n";
            return;
        }

        SongNode* temp = head;
        while (temp != nullptr && temp->songID != id) {
            temp = temp->next;
        }

        if (temp == nullptr) {
            cout << "Song with ID " << id << " not found.\n";
            return;
        }

        // If current track is to be deleted, adjust currentTrack pointer
        if (currentTrack == temp) {
            currentTrack = (temp->next != nullptr) ? temp->next : temp->prev;
        }

        if (temp == head && temp == tail) {
            head = tail = nullptr;
        } else if (temp == head) {
            head = head->next;
            head->prev = nullptr;
        } else if (temp == tail) {
            tail = tail->prev;
            tail->next = nullptr;
        } else {
            temp->prev->next = temp->next;
            temp->next->prev = temp->prev;
        }

        delete temp;
        cout << "Song with ID " << id << " deleted successfully.\n";
    }

    // 3. Display Playlist Forward
    void displayForward() const {
        if (head == nullptr) {
            cout << "Playlist is empty.\n";
            return;
        }
        cout << "\n--- Playlist (Forward) ---\n";
        SongNode* temp = head;
        while (temp != nullptr) {
            cout << "ID: " << temp->songID << " | Name: " << temp->songName 
                 << " | Duration: " << temp->duration;
            if (temp == currentTrack) cout << "  <-- Currently Playing";
            cout << "\n";
            temp = temp->next;
        }
    }

    // 4. Display Playlist Backward
    void displayBackward() const {
        if (tail == nullptr) {
            cout << "Playlist is empty.\n";
            return;
        }
        cout << "\n--- Playlist (Backward) ---\n";
        SongNode* temp = tail;
        while (temp != nullptr) {
            cout << "ID: " << temp->songID << " | Name: " << temp->songName 
                 << " | Duration: " << temp->duration;
            if (temp == currentTrack) cout << "  <-- Currently Playing";
            cout << "\n";
            temp = temp->prev;
        }
    }

    // 5. Search Song by ID
    void searchSong(int id) const {
        SongNode* temp = head;
        while (temp != nullptr) {
            if (temp->songID == id) {
                cout << "\nSong Found: ID: " << temp->songID 
                     << " | Name: " << temp->songName 
                     << " | Duration: " << temp->duration << "\n";
                return;
            }
            temp = temp->next;
        }
        cout << "Song with ID " << id << " not found in playlist.\n";
    }

    // 6. Play Next / Previous Song
    void playNext() {
        if (currentTrack == nullptr) {
            cout << "Playlist is empty.\n";
        } else if (currentTrack->next != nullptr) {
            currentTrack = currentTrack->next;
            cout << "Now Playing: " << currentTrack->songName << "\n";
        } else {
            cout << "End of playlist reached. Cannot play next track.\n";
        }
    }

    void playPrevious() {
        if (currentTrack == nullptr) {
            cout << "Playlist is empty.\n";
        } else if (currentTrack->prev != nullptr) {
            currentTrack = currentTrack->prev;
            cout << "Now Playing: " << currentTrack->songName << "\n";
        } else {
            cout << "Already at the beginning of playlist.\n";
        }
    }

    // 7. Reverse Playlist in place using pointer manipulation
    void reversePlaylist() {
        if (head == nullptr || head->next == nullptr) return;

        SongNode* current = head;
        SongNode* temp = nullptr;

        // Swap next and prev pointers for all nodes
        while (current != nullptr) {
            temp = current->prev;
            current->prev = current->next;
            current->next = temp;
            current = current->prev; // move to next node (which is in prev pointer now)
        }

        // Adjust head and tail
        if (temp != nullptr) {
            tail = head;
            head = temp->prev;
        }
        cout << "Playlist reversed successfully.\n";
    }
};

int main() {
    Playlist myPlaylist;

    myPlaylist.addSong(101, "First Light", "5:55");
    myPlaylist.addSong(102, "Barricades", "6:30");
    myPlaylist.addSong(103, "Aandaz e karam", "8:02");
    myPlaylist.addSong(104, "Love me again", "3:04");

    myPlaylist.displayForward();
    myPlaylist.displayBackward();

    cout << "\n--- Music Player Navigation ---\n";
    myPlaylist.playNext();
    myPlaylist.playNext();
    myPlaylist.playPrevious();

    cout << "\n--- Search & Delete Operations ---\n";
    myPlaylist.searchSong(103);
    myPlaylist.deleteSong(102);
    myPlaylist.displayForward();

    cout << "\n--- Reversing Playlist ---\n";
    myPlaylist.reversePlaylist();
    myPlaylist.displayForward();

    return 0;
}