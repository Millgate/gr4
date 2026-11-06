/*
 * Student Record Lookup System using a Hash Table (separate chaining).
 *
 * Each student has: Student ID, Name, Programme, Level, Telephone.
 * Operations: Insert, Search, Delete, Update, Display hash table.
 * The hash table is implemented from scratch (no std::unordered_map).
 *
 * Compile: g++ -std=c++11 student_hash_table.cpp -o student_hash_table
 * Run:     ./student_hash_table
 */

#include <iostream>
#include <string>
#include <iomanip>
#include <limits>
using namespace std;

// ---------------- Student record ----------------
struct Student {
    string studentId;
    string name;
    string programme;
    string level;
    string telephone;
};

// A node in a bucket's linked list
struct Node {
    Student student;
    Node* next;
    Node(const Student& s) : student(s), next(nullptr) {}
};

// ---------------- Hash table ----------------
class HashTable {
private:
    int size;        // number of buckets
    int count;       // number of students stored
    Node** buckets;  // array of linked-list heads

    // Division method: index = ID mod table size
    int hashFunction(const string& id) const {
        unsigned long long value = 0;
        for (char c : id) {
            value = (value * 10 + (c - '0')) % size;  // avoids overflow on long IDs
        }
        return (int)value;
    }

public:
    HashTable(int tableSize = 13) : size(tableSize), count(0) {
        buckets = new Node*[size];
        for (int i = 0; i < size; i++) buckets[i] = nullptr;
    }

    ~HashTable() {
        for (int i = 0; i < size; i++) {
            Node* current = buckets[i];
            while (current) {
                Node* temp = current;
                current = current->next;
                delete temp;
            }
        }
        delete[] buckets;
    }

    // Insert: returns false if the ID already exists
    bool insert(const Student& s) {
        if (search(s.studentId) != nullptr) return false;
        int index = hashFunction(s.studentId);
        Node* node = new Node(s);       // insert at head of chain: O(1)
        node->next = buckets[index];
        buckets[index] = node;
        count++;
        return true;
    }

    // Search: returns pointer to the student, or nullptr if not found
    Student* search(const string& id) {
        Node* current = buckets[hashFunction(id)];
        while (current) {
            if (current->student.studentId == id) return &current->student;
            current = current->next;
        }
        return nullptr;
    }

    // Delete: returns true if a student was removed
    bool remove(const string& id) {
        int index = hashFunction(id);
        Node* current = buckets[index];
        Node* previous = nullptr;
        while (current) {
            if (current->student.studentId == id) {
                if (previous) previous->next = current->next;
                else buckets[index] = current->next;
                delete current;
                count--;
                return true;
            }
            previous = current;
            current = current->next;
        }
        return false;
    }

    // Update: empty strings mean "keep the current value"
    bool update(const string& id, const string& name, const string& programme,
                const string& level, const string& telephone) {
        Student* s = search(id);
        if (!s) return false;
        if (!name.empty()) s->name = name;
        if (!programme.empty()) s->programme = programme;
        if (!level.empty()) s->level = level;
        if (!telephone.empty()) s->telephone = telephone;
        return true;
    }

    // Display every bucket and its chain
    void display() const {
        cout << "\nHash Table (size = " << size << ", students = " << count << ")\n";
        cout << string(70, '-') << "\n";
        for (int i = 0; i < size; i++) {
            cout << "[" << setw(2) << i << "] ";
            Node* current = buckets[i];
            if (!current) {
                cout << "empty";
            } else {
                while (current) {
                    cout << current->student.studentId << " (" << current->student.name << ")";
                    if (current->next) cout << " -> ";
                    current = current->next;
                }
            }
            cout << "\n";
        }
        cout << string(70, '-') << "\n";
    }
};

// ---------------- Input helpers ----------------
bool isDigits(const string& s) {
    if (s.empty()) return false;
    for (char c : s) if (c < '0' || c > '9') return false;
    return true;
}

string readId(const string& prompt = "Enter Student ID: ") {
    string id;
    while (true) {
        cout << prompt;
        getline(cin, id);
        if (isDigits(id)) return id;
        cout << "Student ID must contain digits only.\n";
    }
}

string readNonEmpty(const string& prompt) {
    string value;
    while (true) {
        cout << prompt;
        getline(cin, value);
        if (!value.empty()) return value;
        cout << "This field cannot be empty.\n";
    }
}

void printStudent(const Student& s) {
    cout << "ID: " << s.studentId << " | Name: " << s.name
         << " | Programme: " << s.programme << " | Level: " << s.level
         << " | Tel: " << s.telephone << "\n";
}

void showMenu() {
    cout << "\n===== STUDENT RECORD SYSTEM =====\n"
         << "1. Insert student\n"
         << "2. Search student\n"
         << "3. Delete student\n"
         << "4. Update student\n"
         << "5. Display hash table\n"
         << "6. Exit\n"
         << "Choose an option (1-6): ";
}

// ---------------- Main ----------------
int main() {
    HashTable table;

    // Sample records so you can test right away (remove if not wanted)
    table.insert({"10234567", "Ama Mensah", "Computer Science", "200", "0241234567"});
    table.insert({"10234580", "Kofi Boateng", "Information Technology", "300", "0201112222"});
    table.insert({"10234593", "Esi Owusu", "Mathematics", "100", "0553334444"});

    string choice;
    while (true) {
        showMenu();
        if (!getline(cin, choice)) break;  // end of input

        if (choice == "1") {
            string id = readId();
            if (table.search(id)) {
                cout << "A student with that ID already exists.\n";
                continue;
            }
            Student s;
            s.studentId = id;
            s.name = readNonEmpty("Name: ");
            s.programme = readNonEmpty("Programme: ");
            s.level = readNonEmpty("Level: ");
            s.telephone = readNonEmpty("Telephone: ");
            table.insert(s);
            cout << "Student inserted successfully.\n";

        } else if (choice == "2") {
            Student* s = table.search(readId());
            if (s) printStudent(*s);
            else cout << "Student not found.\n";

        } else if (choice == "3") {
            if (table.remove(readId())) cout << "Student deleted.\n";
            else cout << "Student not found.\n";

        } else if (choice == "4") {
            string id = readId();
            if (!table.search(id)) {
                cout << "Student not found.\n";
                continue;
            }
            string name, programme, level, telephone;
            cout << "Leave a field blank to keep its current value.\n";
            cout << "New name: ";       getline(cin, name);
            cout << "New programme: ";  getline(cin, programme);
            cout << "New level: ";      getline(cin, level);
            cout << "New telephone: ";  getline(cin, telephone);
            table.update(id, name, programme, level, telephone);
            cout << "Student updated.\n";

        } else if (choice == "5") {
            table.display();

        } else if (choice == "6") {
            cout << "Goodbye!\n";
            break;

        } else {
            cout << "Invalid choice. Enter a number from 1 to 6.\n";
        }
    }
    return 0;
}
