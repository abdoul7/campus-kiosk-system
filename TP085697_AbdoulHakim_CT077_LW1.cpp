#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include <cstdio>
#include <chrono>
#include <limits>

using namespace std;
using namespace std::chrono;

struct Student {
    char studentID[16];
    char name[100];
    char programme[20];
    int yearOfStudy;
    double cgpa;
    char contactNo[32];
};

struct Node {
    Student data;
    Node* next;
};

class StudentLinkedList {
private:
    Node* head;

    void copyText(char destination[], const char source[], int maxSize) {
        if (maxSize <= 0) return;
        strncpy(destination, source, maxSize - 1);
        destination[maxSize - 1] = '\0';
    }

    bool isValidCGPA(double value) const {
        return value >= 0.0 && value <= 4.0;
    }

public:
    StudentLinkedList() {
        head = NULL;
    }

    ~StudentLinkedList() {
        clear();
    }

    bool isEmpty() const {
        return head == NULL;
    }

    void clear() {
        Node* current = head;
        while (current != NULL) {
            Node* temp = current;
            current = current->next;
            delete temp;
        }
        head = NULL;
    }

    int countRecords() const {
        int total = 0;
        Node* current = head;
        while (current != NULL) {
            total++;
            current = current->next;
        }
        return total;
    }

    Node* searchByID(const char targetID[]) const {
        Node* current = head;
        while (current != NULL) {
            if (strcmp(current->data.studentID, targetID) == 0) {
                return current;
            }
            current = current->next;
        }
        return NULL;
    }

    bool equalsIgnoreCase(const char a[], const char b[]) const {
        int i = 0;
        while (a[i] != '\0' && b[i] != '\0') {
            if (tolower((unsigned char)a[i]) != tolower((unsigned char)b[i])) {
                return false;
            }
            i++;
        }
        return a[i] == '\0' && b[i] == '\0';
    }

    Node* searchByName(const char targetName[]) const {
        Node* current = head;
        while (current != NULL) {
            if (equalsIgnoreCase(current->data.name, targetName)) {
                return current;
            }
            current = current->next;
        }
        return NULL;
    }

    bool canInsert(const Student& s, bool showMessage) const {
        if (strlen(s.studentID) == 0 || strlen(s.name) == 0 || strlen(s.programme) == 0) {
            if (showMessage) cout << "Insert rejected: ID, Name and Programme cannot be empty.\n";
            return false;
        }
        if (!isValidCGPA(s.cgpa)) {
            if (showMessage) cout << "Insert rejected: CGPA must be between 0.00 and 4.00.\n";
            return false;
        }
        if (searchByID(s.studentID) != NULL) {
            if (showMessage) cout << "Insert rejected: duplicate Student ID found.\n";
            return false;
        }
        return true;
    }

    bool insertAtBeginning(const Student& s, bool showMessage = true) {
        if (!canInsert(s, showMessage)) return false;

        Node* newNode = new Node;
        newNode->data = s;
        newNode->next = head;
        head = newNode;
        return true;
    }

    bool insertAtEnd(const Student& s, bool showMessage = true) {
        if (!canInsert(s, showMessage)) return false;

        Node* newNode = new Node;
        newNode->data = s;
        newNode->next = NULL;

        if (head == NULL) {
            head = newNode;
            return true;
        }

        Node* current = head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
        return true;
    }

    bool insertAtPosition(const Student& s, int position, bool showMessage = true) {
        int total = countRecords();
        if (position < 1 || position > total + 1) {
            if (showMessage) cout << "Insert rejected: invalid position. Valid range is 1 to " << total + 1 << ".\n";
            return false;
        }
        if (!canInsert(s, showMessage)) return false;

        if (position == 1) {
            Node* newNode = new Node;
            newNode->data = s;
            newNode->next = head;
            head = newNode;
            return true;
        }

        Node* current = head;
        for (int i = 1; i < position - 1; i++) {
            current = current->next;
        }

        Node* newNode = new Node;
        newNode->data = s;
        newNode->next = current->next;
        current->next = newNode;
        return true;
    }

    bool deleteByID(const char targetID[], bool showMessage = true) {
        if (head == NULL) {
            if (showMessage) cout << "Delete failed: the linked list is empty.\n";
            return false;
        }

        if (strcmp(head->data.studentID, targetID) == 0) {
            Node* temp = head;
            head = head->next;
            delete temp;
            return true;
        }

        Node* previous = head;
        Node* current = head->next;
        while (current != NULL) {
            if (strcmp(current->data.studentID, targetID) == 0) {
                previous->next = current->next;
                delete current;
                return true;
            }
            previous = current;
            current = current->next;
        }

        if (showMessage) cout << "Delete failed: Student ID was not found.\n";
        return false;
    }

    void displayStudent(const Student& s, ostream& out) const {
        out << left << setw(12) << s.studentID
            << setw(28) << s.name
            << setw(14) << s.programme
            << setw(8) << s.yearOfStudy
            << setw(8) << fixed << setprecision(2) << s.cgpa
            << setw(18) << s.contactNo << '\n';
    }

    void displayAll(ostream& out, int maxRows = 0) const {
        if (head == NULL) {
            out << "No records to display. The linked list is empty.\n";
            return;
        }

        out << left << setw(12) << "Student ID"
            << setw(28) << "Name"
            << setw(14) << "Programme"
            << setw(8) << "Year"
            << setw(8) << "CGPA"
            << setw(18) << "Contact" << '\n';
        for (int line = 0; line < 88; line++) out << '-';
        out << '\n';

        Node* current = head;
        int printed = 0;
        while (current != NULL) {
            displayStudent(current->data, out);
            printed++;
            if (maxRows > 0 && printed >= maxRows) {
                out << "... output limited to first " << maxRows << " records ...\n";
                return;
            }
            current = current->next;
        }
    }

    bool getRecordAtPosition(int position, Student& result) const {
        if (position < 1) return false;
        Node* current = head;
        int index = 1;
        while (current != NULL) {
            if (index == position) {
                result = current->data;
                return true;
            }
            current = current->next;
            index++;
        }
        return false;
    }
};

void removeNewLine(char text[]) {
    int length = strlen(text);
    while (length > 0 && (text[length - 1] == '\n' || text[length - 1] == '\r')) {
        text[length - 1] = '\0';
        length--;
    }
}

void trim(char text[]) {
    int start = 0;
    while (text[start] != '\0' && isspace((unsigned char)text[start])) start++;

    int end = strlen(text) - 1;
    while (end >= start && isspace((unsigned char)text[end])) end--;

    int j = 0;
    for (int i = start; i <= end; i++) {
        text[j++] = text[i];
    }
    text[j] = '\0';
}

bool parseIntegerStrict(const char text[], int& value) {
    if (text[0] == '\0') return false;
    char* endPtr;
    long result = strtol(text, &endPtr, 10);
    if (*endPtr != '\0') return false;
    value = (int)result;
    return true;
}

bool parseDoubleStrict(const char text[], double& value) {
    if (text[0] == '\0') return false;
    char* endPtr;
    double result = strtod(text, &endPtr);
    if (*endPtr != '\0') return false;
    value = result;
    return true;
}

bool splitCSVLine(const char line[], char fields[][128], int& fieldCount) {
    fieldCount = 0;
    int fieldIndex = 0;
    int charIndex = 0;

    for (int i = 0; line[i] != '\0'; i++) {
        if (line[i] == ',') {
            if (fieldIndex >= 6) return false;
            fields[fieldIndex][charIndex] = '\0';
            trim(fields[fieldIndex]);
            fieldIndex++;
            charIndex = 0;
        } else {
            if (charIndex >= 127) return false;
            fields[fieldIndex][charIndex++] = line[i];
        }
    }

    if (fieldIndex >= 6) return false;
    fields[fieldIndex][charIndex] = '\0';
    trim(fields[fieldIndex]);
    fieldCount = fieldIndex + 1;
    return true;
}

bool parseStudentLine(const char rawLine[], Student& s) {
    char line[512];
    strncpy(line, rawLine, sizeof(line) - 1);
    line[sizeof(line) - 1] = '\0';
    removeNewLine(line);

    char fields[6][128];
    int fieldCount = 0;
    if (!splitCSVLine(line, fields, fieldCount)) return false;
    if (fieldCount != 5 && fieldCount != 6) return false;

    int year;
    double cgpa;
    if (!parseIntegerStrict(fields[3], year)) return false;
    if (!parseDoubleStrict(fields[4], cgpa)) return false;

    if (strlen(fields[0]) == 0 || strlen(fields[1]) == 0 || strlen(fields[2]) == 0) return false;
    if (year < 1 || year > 10) return false;
    if (cgpa < 0.0 || cgpa > 4.0) return false;

    strncpy(s.studentID, fields[0], sizeof(s.studentID) - 1);
    s.studentID[sizeof(s.studentID) - 1] = '\0';
    strncpy(s.name, fields[1], sizeof(s.name) - 1);
    s.name[sizeof(s.name) - 1] = '\0';
    strncpy(s.programme, fields[2], sizeof(s.programme) - 1);
    s.programme[sizeof(s.programme) - 1] = '\0';
    s.yearOfStudy = year;
    s.cgpa = cgpa;
    if (fieldCount == 6) {
        strncpy(s.contactNo, fields[5], sizeof(s.contactNo) - 1);
        s.contactNo[sizeof(s.contactNo) - 1] = '\0';
    } else {
        strcpy(s.contactNo, "-");
    }
    return true;
}

Student makeStudent(const char id[], const char name[], const char programme[], int year, double cgpa, const char contact[]) {
    Student s;
    strncpy(s.studentID, id, sizeof(s.studentID) - 1);
    s.studentID[sizeof(s.studentID) - 1] = '\0';
    strncpy(s.name, name, sizeof(s.name) - 1);
    s.name[sizeof(s.name) - 1] = '\0';
    strncpy(s.programme, programme, sizeof(s.programme) - 1);
    s.programme[sizeof(s.programme) - 1] = '\0';
    s.yearOfStudy = year;
    s.cgpa = cgpa;
    strncpy(s.contactNo, contact, sizeof(s.contactNo) - 1);
    s.contactNo[sizeof(s.contactNo) - 1] = '\0';
    return s;
}

bool loadCSV(const char filename[], StudentLinkedList& list, int& loaded, int& malformed) {
    ifstream file(filename);
    loaded = 0;
    malformed = 0;

    if (!file.is_open()) {
        cout << "Could not open file: " << filename << "\n";
        return false;
    }

    list.clear();
    char line[512];

    if (!file.getline(line, sizeof(line))) {
        return true;
    }

    while (file.getline(line, sizeof(line))) {
        Student s;
        if (parseStudentLine(line, s)) {
            if (list.insertAtEnd(s, false)) {
                loaded++;
            } else {
                malformed++;
            }
        } else {
            malformed++;
        }
    }

    return true;
}

long long timeFunction(void (*operation)(StudentLinkedList&), StudentLinkedList& list) {
    high_resolution_clock::time_point start = high_resolution_clock::now();
    operation(list);
    high_resolution_clock::time_point end = high_resolution_clock::now();
    return duration_cast<microseconds>(end - start).count();
}

void printSearchResult(Node* result) {
    if (result == NULL) {
        cout << "Record not found.\n";
    } else {
        cout << "Record found:\n";
        cout << left << setw(12) << "Student ID" << setw(28) << "Name" << setw(14) << "Programme"
             << setw(8) << "Year" << setw(8) << "CGPA" << setw(18) << "Contact" << '\n';
        for (int line = 0; line < 88; line++) cout << '-';
        cout << '\n';
        cout << left << setw(12) << result->data.studentID
             << setw(28) << result->data.name
             << setw(14) << result->data.programme
             << setw(8) << result->data.yearOfStudy
             << setw(8) << fixed << setprecision(2) << result->data.cgpa
             << setw(18) << result->data.contactNo << '\n';
    }
}

void runEdgeCaseTests() {
    cout << "\n===== EDGE CASE TESTS =====\n";
    StudentLinkedList list;

    cout << "\n1) Empty structure operations\n";
    printSearchResult(list.searchByID("TP999999"));
    list.deleteByID("TP999999");
    list.displayAll(cout);

    cout << "\n2) Valid insert + duplicate ID\n";
    Student a = makeStudent("TPEDGE1", "Valid Student", "CS", 1, 3.20, "0123456789");
    Student duplicate = makeStudent("TPEDGE1", "Duplicate Student", "CS", 2, 3.70, "0123456789");
    cout << "First insert: " << (list.insertAtBeginning(a) ? "accepted" : "rejected") << '\n';
    cout << "Duplicate insert: " << (list.insertAtEnd(duplicate) ? "accepted" : "rejected") << '\n';

    cout << "\n3) Invalid CGPA\n";
    Student badLow = makeStudent("TPBAD01", "Bad Low", "CS", 1, -0.50, "-");
    Student badHigh = makeStudent("TPBAD02", "Bad High", "CS", 1, 4.50, "-");
    list.insertAtEnd(badLow);
    list.insertAtEnd(badHigh);

    cout << "\n4) Invalid insert position\n";
    Student posStudent = makeStudent("TPPOS01", "Position Student", "IT", 1, 3.00, "-");
    list.insertAtPosition(posStudent, 100);

    cout << "\n5) Delete non-existent record\n";
    list.deleteByID("TPNOPE1");

    cout << "\n6) Malformed CSV parser examples\n";
    const char goodLine[] = "TP900001,CSV Good,CS,1,3.50,0111111111";
    const char missingField[] = "TP900002,CSV Bad,CS,1";
    const char nonNumeric[] = "TP900003,CSV Bad,CS,YearTwo,3.2";
    Student parsed;
    cout << "Good line: " << (parseStudentLine(goodLine, parsed) ? "accepted" : "rejected") << '\n';
    cout << "Missing fields: " << (parseStudentLine(missingField, parsed) ? "accepted" : "rejected") << '\n';
    cout << "Non-numeric year: " << (parseStudentLine(nonNumeric, parsed) ? "accepted" : "rejected") << '\n';
}

long long measureInsertHead(StudentLinkedList& list, int n) {
    char id[16];
    sprintf(id, "TPX%06d", n);
    Student s = makeStudent(id, "Benchmark Insert Head", "CS", 1, 3.30, "-");
    high_resolution_clock::time_point start = high_resolution_clock::now();
    list.insertAtBeginning(s, false);
    high_resolution_clock::time_point end = high_resolution_clock::now();
    list.deleteByID(id, false);
    return duration_cast<microseconds>(end - start).count();
}

long long measureInsertEnd(StudentLinkedList& list, int n) {
    char id[16];
    sprintf(id, "TPY%06d", n);
    Student s = makeStudent(id, "Benchmark Insert End", "CS", 1, 3.30, "-");
    high_resolution_clock::time_point start = high_resolution_clock::now();
    list.insertAtEnd(s, false);
    high_resolution_clock::time_point end = high_resolution_clock::now();
    list.deleteByID(id, false);
    return duration_cast<microseconds>(end - start).count();
}

long long measureInsertMiddle(StudentLinkedList& list, int n) {
    char id[16];
    sprintf(id, "TPZ%06d", n);
    Student s = makeStudent(id, "Benchmark Insert Middle", "CS", 1, 3.30, "-");
    int position = (n / 2) + 1;
    high_resolution_clock::time_point start = high_resolution_clock::now();
    list.insertAtPosition(s, position, false);
    high_resolution_clock::time_point end = high_resolution_clock::now();
    list.deleteByID(id, false);
    return duration_cast<microseconds>(end - start).count();
}

long long measureDeleteTail(StudentLinkedList& list, const char targetID[], const Student& backup) {
    high_resolution_clock::time_point start = high_resolution_clock::now();
    list.deleteByID(targetID, false);
    high_resolution_clock::time_point end = high_resolution_clock::now();
    list.insertAtEnd(backup, false);
    return duration_cast<microseconds>(end - start).count();
}

long long measureSearchID(StudentLinkedList& list, const char targetID[]) {
    high_resolution_clock::time_point start = high_resolution_clock::now();
    volatile Node* result = list.searchByID(targetID);
    high_resolution_clock::time_point end = high_resolution_clock::now();
    (void)result;
    return duration_cast<microseconds>(end - start).count();
}

long long measureSearchName(StudentLinkedList& list, const char targetName[]) {
    high_resolution_clock::time_point start = high_resolution_clock::now();
    volatile Node* result = list.searchByName(targetName);
    high_resolution_clock::time_point end = high_resolution_clock::now();
    (void)result;
    return duration_cast<microseconds>(end - start).count();
}

long long measureDisplay(StudentLinkedList& list) {
    ofstream out("member5_display_benchmark.txt");
    high_resolution_clock::time_point start = high_resolution_clock::now();
    list.displayAll(out);
    high_resolution_clock::time_point end = high_resolution_clock::now();
    out.close();
    return duration_cast<microseconds>(end - start).count();
}

long long measureCount(StudentLinkedList& list) {
    high_resolution_clock::time_point start = high_resolution_clock::now();
    volatile int total = list.countRecords();
    high_resolution_clock::time_point end = high_resolution_clock::now();
    (void)total;
    return duration_cast<microseconds>(end - start).count();
}

void runBenchmark() {
    const char* files[4] = {
        "students_500.csv",
        "students_2000.csv",
        "students_8000.csv",
        "students_30000.csv"
    };

    int expectedN[4] = {500, 2000, 8000, 30000};

    cout << "\n===== MEMBER 5 LINKED LIST BENCHMARK =====\n";
    cout << "Place the four students_*.csv files in the same folder as this executable.\n";
    cout << "All times are measured in microseconds (us).\n\n";

    cout << left << setw(18) << "Dataset"
         << setw(12) << "Loaded"
         << setw(12) << "BadRows"
         << setw(14) << "InsHead"
         << setw(14) << "InsEnd"
         << setw(14) << "InsMid"
         << setw(14) << "Delete"
         << setw(14) << "SearchID"
         << setw(14) << "SearchName"
         << setw(14) << "Display"
         << setw(14) << "Count" << '\n';
    for (int line = 0; line < 154; line++) cout << '-';
    cout << '\n';

    for (int i = 0; i < 4; i++) {
        StudentLinkedList list;
        int loaded = 0;
        int malformed = 0;
        if (!loadCSV(files[i], list, loaded, malformed)) {
            cout << files[i] << " skipped because it was not found.\n";
            continue;
        }

        Student last;
        if (!list.getRecordAtPosition(loaded, last)) {
            cout << files[i] << " has no valid records.\n";
            continue;
        }

        long long insertHead = measureInsertHead(list, expectedN[i]);
        long long insertEnd = measureInsertEnd(list, expectedN[i]);
        long long insertMid = measureInsertMiddle(list, expectedN[i]);
        long long deleteTime = measureDeleteTail(list, last.studentID, last);
        long long searchID = measureSearchID(list, last.studentID);
        long long searchName = measureSearchName(list, "ZZZ_NAME_NOT_IN_DATASET_999999");
        long long displayTime = measureDisplay(list);
        long long countTime = measureCount(list);

        cout << left << setw(18) << files[i]
             << setw(12) << loaded
             << setw(12) << malformed
             << setw(14) << insertHead
             << setw(14) << insertEnd
             << setw(14) << insertMid
             << setw(14) << deleteTime
             << setw(14) << searchID
             << setw(14) << searchName
             << setw(14) << displayTime
             << setw(14) << countTime << '\n';
    }

    cout << "\nN/A rows for Member 5: Binary Search and Sort by CGPA are not applicable to a singly linked list.\n";
}

void readStudentFromKeyboard(Student& s) {
    char buffer[128];

    cout << "Student ID: ";
    cin.getline(s.studentID, sizeof(s.studentID));
    trim(s.studentID);

    cout << "Name: ";
    cin.getline(s.name, sizeof(s.name));
    trim(s.name);

    cout << "Programme: ";
    cin.getline(s.programme, sizeof(s.programme));
    trim(s.programme);

    cout << "Year of study: ";
    cin.getline(buffer, sizeof(buffer));
    trim(buffer);
    s.yearOfStudy = atoi(buffer);

    cout << "CGPA: ";
    cin.getline(buffer, sizeof(buffer));
    trim(buffer);
    s.cgpa = atof(buffer);

    cout << "Contact number (or -): ";
    cin.getline(s.contactNo, sizeof(s.contactNo));
    trim(s.contactNo);
    if (strlen(s.contactNo) == 0) strcpy(s.contactNo, "-");
}

void interactiveMenu() {
    StudentLinkedList list;
    int choice;

    do {
        cout << "\n===== STUDENT RECORD MANAGEMENT - MEMBER 5 LINKED LIST =====\n";
        cout << "1. Load CSV file\n";
        cout << "2. Insert at beginning\n";
        cout << "3. Insert at end\n";
        cout << "4. Insert at specified position\n";
        cout << "5. Delete by Student ID\n";
        cout << "6. Search by Student ID\n";
        cout << "7. Search by Name\n";
        cout << "8. Display all records\n";
        cout << "9. Count total records\n";
        cout << "10. Run edge-case tests\n";
        cout << "11. Run benchmark mode\n";
        cout << "0. Exit\n";
        cout << "Choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == 1) {
            char filename[128];
            int loaded, malformed;
            cout << "CSV filename: ";
            cin.getline(filename, sizeof(filename));
            if (loadCSV(filename, list, loaded, malformed)) {
                cout << "Loaded " << loaded << " records from " << filename << ". Malformed/skipped rows: " << malformed << ".\n";
            }
        } else if (choice >= 2 && choice <= 4) {
            Student s;
            readStudentFromKeyboard(s);
            bool ok = false;
            if (choice == 2) ok = list.insertAtBeginning(s);
            if (choice == 3) ok = list.insertAtEnd(s);
            if (choice == 4) {
                int position;
                cout << "Position (1-based): ";
                cin >> position;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                ok = list.insertAtPosition(s, position);
            }
            cout << (ok ? "Insert successful.\n" : "Insert failed.\n");
        } else if (choice == 5) {
            char id[16];
            cout << "Student ID to delete: ";
            cin.getline(id, sizeof(id));
            cout << (list.deleteByID(id) ? "Delete successful.\n" : "Delete failed.\n");
        } else if (choice == 6) {
            char id[16];
            cout << "Student ID to search: ";
            cin.getline(id, sizeof(id));
            printSearchResult(list.searchByID(id));
        } else if (choice == 7) {
            char name[100];
            cout << "Name to search: ";
            cin.getline(name, sizeof(name));
            printSearchResult(list.searchByName(name));
        } else if (choice == 8) {
            list.displayAll(cout, 50);
        } else if (choice == 9) {
            cout << "Total records: " << list.countRecords() << '\n';
        } else if (choice == 10) {
            runEdgeCaseTests();
        } else if (choice == 11) {
            runBenchmark();
        }
    } while (choice != 0);
}

int main() {
    interactiveMenu();
    return 0;
}
