#include <iostream>
using namespace std;
// Node representing a person in the circle
struct PersonNode {
    int personID;
    PersonNode* next;

    PersonNode(int id) : personID(id), next(nullptr) {}
};

class JosephusCircle {
private:
    PersonNode* head;

public:
    JosephusCircle() : head(nullptr) {}

    ~JosephusCircle() {
        if (head == nullptr) return;
        PersonNode* current = head;
        PersonNode* tail = head;
        while (tail->next != head) {
            tail = tail->next;
        }
        tail->next = nullptr; // Break loop for safe dynamic deletion
        while (current != nullptr) {
            PersonNode* temp = current->next;
            delete current;
            current = temp;
        }
    }

    // Create Circle of N people
    void createCircle(int N) {
        if (N <= 0) return;
        
        head = new PersonNode(1);
        PersonNode* prev = head;

        for (int i = 2; i <= N; ++i) {
            PersonNode* newNode = new PersonNode(i);
            prev->next = newNode;
            prev = newNode;
        }
        prev->next = head; // Form the circular link
    }
    // Elimination Process
    void simulateElimination(int N, int k) {
        if (N <= 0 || k <= 0) {
            cout << "Invalid input parameters.\n";
            return;
        }
        createCircle(N);
        cout << "\n--- Josephus Elimination Process (N = " << N << ", k = " << k << ") ---\n";
        cout << "Eliminated Order: ";
        PersonNode* current = head;
        PersonNode* prev = nullptr;
        // Find the last node (tail) to keep track of predecessor initially
        while (current->next != head) {
            current = current->next;
        }
        prev = current;
        current = head;
        // Eliminate until only 1 person remains
        while (current->next != current) {
            // Count up to k
            for (int count = 1; count < k; ++count) {
                prev = current;
                current = current->next;
            }
            // Current is the k-th person to be eliminated
            cout << current->personID << " ";
            prev->next = current->next;
            delete current;
            current = prev->next;
        }
        head = current; // Survivor
        cout << "\nSurvivor: Person " << head->personID << "\n";
    }
};
int main() {
    int N = 7, k = 3;    
    JosephusCircle game;
    game.simulateElimination(N, k);
    return 0;
}