/*
 * ============================================================
 *         STUDENT MANAGEMENT SYSTEM
 *         A Console-Based C++ Project
 * ============================================================
 */

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <limits>
#include <vector>
#include <algorithm>

using namespace std;

// ─────────────────────────────────────────────
//  STUDENT STRUCTURE
// ─────────────────────────────────────────────
struct Student {
    int    id;
    string name;
    int    age;
    string course;
    float  gpa;
};

// ─────────────────────────────────────────────
//  CONSTANTS
// ─────────────────────────────────────────────
const string DATA_FILE = "students.dat";

// ─────────────────────────────────────────────
//  UTILITY FUNCTIONS
// ─────────────────────────────────────────────

void clearInputBuffer() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void printLine(char c = '-', int len = 60) {
    cout << string(len, c) << "\n";
}

void printHeader() {
    system("clear || cls");
    printLine('=');
    cout << setw(38) << " STUDENT MANAGEMENT SYSTEM\n";
    printLine('=');
}

void pause() {
    cout << "\n  Press Enter to continue...";
    clearInputBuffer();
    cin.get();
}

// ─────────────────────────────────────────────
//  FILE HANDLING HELPERS
// ─────────────────────────────────────────────

vector<Student> loadStudents() {
    vector<Student> students;
    ifstream fin(DATA_FILE);
    if (!fin.is_open()) return students;

    Student s;
    while (fin >> s.id >> ws) {
        getline(fin, s.name);
        fin >> s.age >> ws;
        getline(fin, s.course);
        fin >> s.gpa;
        fin.ignore();
        students.push_back(s);
    }
    fin.close();
    return students;
}

void saveStudents(const vector<Student>& students) {
    ofstream fout(DATA_FILE);
    for (const auto& s : students) {
        fout << s.id     << "\n"
             << s.name   << "\n"
             << s.age    << "\n"
             << s.course << "\n"
             << s.gpa    << "\n";
    }
    fout.close();
}

bool idExists(const vector<Student>& students, int id) {
    for (const auto& s : students)
        if (s.id == id) return true;
    return false;
}

// ─────────────────────────────────────────────
//  TABLE DISPLAY
// ─────────────────────────────────────────────

void printStudentRow(const Student& s) {
    cout << "  "
         << left  << setw(6)  << s.id
         << left  << setw(22) << s.name
         << left  << setw(5)  << s.age
         << left  << setw(18) << s.course
         << right << setw(5)  << fixed << setprecision(2) << s.gpa
         << "\n";
}

void printTableHeader() {
    printLine();
    cout << "  "
         << left  << setw(6)  << "ID"
         << left  << setw(22) << "Name"
         << left  << setw(5)  << "Age"
         << left  << setw(18) << "Course"
         << right << setw(5)  << "GPA"
         << "\n";
    printLine();
}

// ─────────────────────────────────────────────
//  MENU FUNCTIONS
// ─────────────────────────────────────────────

void addStudent() {
    printHeader();
    cout << "  [ ADD NEW STUDENT ]\n";
    printLine();

    vector<Student> students = loadStudents();
    Student s;

    cout << "  Enter Student ID   : ";
    while (!(cin >> s.id) || s.id <= 0) {
        cout << "  Invalid! Enter a positive integer ID: ";
        cin.clear(); clearInputBuffer();
    }
    if (idExists(students, s.id)) {
        cout << "\n  ERROR: ID " << s.id << " already exists!\n";
        pause(); return;
    }

    clearInputBuffer();
    cout << "  Enter Full Name    : "; getline(cin, s.name);

    cout << "  Enter Age          : ";
    while (!(cin >> s.age) || s.age < 5 || s.age > 100) {
        cout << "  Invalid! Age must be 5-100: ";
        cin.clear(); clearInputBuffer();
    }

    clearInputBuffer();
    cout << "  Enter Course       : "; getline(cin, s.course);

    cout << "  Enter GPA (0.0-4.0): ";
    while (!(cin >> s.gpa) || s.gpa < 0.0f || s.gpa > 4.0f) {
        cout << "  Invalid! GPA must be 0.0-4.0: ";
        cin.clear(); clearInputBuffer();
    }

    students.push_back(s);
    saveStudents(students);
    cout << "\n  SUCCESS: Student \"" << s.name << "\" added!\n";
    pause();
}

void displayAllStudents() {
    printHeader();
    cout << "  [ ALL STUDENTS ]\n";
    vector<Student> students = loadStudents();

    if (students.empty()) {
        printLine();
        cout << "  No records found.\n";
        pause(); return;
    }

    printTableHeader();
    for (const auto& s : students) printStudentRow(s);
    printLine();
    cout << "  Total: " << students.size() << " student(s)\n";
    pause();
}

void searchStudent() {
    printHeader();
    cout << "  [ SEARCH STUDENT ]\n";
    printLine();

    int id;
    cout << "  Enter Student ID: ";
    while (!(cin >> id) || id <= 0) {
        cout << "  Invalid ID: "; cin.clear(); clearInputBuffer();
    }

    for (const auto& s : loadStudents()) {
        if (s.id == id) {
            printLine();
            cout << "  FOUND:\n";
            printTableHeader();
            printStudentRow(s);
            printLine();
            pause(); return;
        }
    }
    cout << "\n  ERROR: ID " << id << " not found.\n";
    pause();
}

void updateStudent() {
    printHeader();
    cout << "  [ UPDATE STUDENT ]\n";
    printLine();

    int id;
    cout << "  Enter Student ID to update: ";
    while (!(cin >> id) || id <= 0) {
        cout << "  Invalid ID: "; cin.clear(); clearInputBuffer();
    }

    vector<Student> students = loadStudents();
    for (auto& s : students) {
        if (s.id == id) {
            cout << "\n  Current record:\n";
            printTableHeader(); printStudentRow(s); printLine();
            cout << "\n  Press Enter to keep current value.\n\n";
            clearInputBuffer();

            string tmp;
            cout << "  New Name [" << s.name << "]: ";
            getline(cin, tmp); if (!tmp.empty()) s.name = tmp;

            cout << "  New Age  [" << s.age << "]: ";
            getline(cin, tmp);
            if (!tmp.empty()) try { s.age = stoi(tmp); } catch (...) {}

            cout << "  New Course [" << s.course << "]: ";
            getline(cin, tmp); if (!tmp.empty()) s.course = tmp;

            cout << "  New GPA  [" << s.gpa << "]: ";
            getline(cin, tmp);
            if (!tmp.empty()) try {
                float g = stof(tmp);
                if (g >= 0.0f && g <= 4.0f) s.gpa = g;
            } catch (...) {}

            saveStudents(students);
            cout << "\n  SUCCESS: Record updated!\n";
            pause(); return;
        }
    }
    cout << "\n  ERROR: ID " << id << " not found.\n";
    pause();
}

void deleteStudent() {
    printHeader();
    cout << "  [ DELETE STUDENT ]\n";
    printLine();

    int id;
    cout << "  Enter Student ID to delete: ";
    while (!(cin >> id) || id <= 0) {
        cout << "  Invalid ID: "; cin.clear(); clearInputBuffer();
    }

    vector<Student> students = loadStudents();
    bool found = false;
    for (const auto& s : students) {
        if (s.id == id) {
            found = true;
            cout << "\n  Record to delete:\n";
            printTableHeader(); printStudentRow(s); printLine();
            break;
        }
    }
    if (!found) {
        cout << "\n  ERROR: ID " << id << " not found.\n";
        pause(); return;
    }

    char confirm;
    cout << "  Confirm delete? (y/n): "; cin >> confirm;
    if (tolower(confirm) != 'y') {
        cout << "\n  Cancelled.\n"; pause(); return;
    }

    auto sz = students.size();
    students.erase(remove_if(students.begin(), students.end(),
        [id](const Student& s){ return s.id == id; }), students.end());
    saveStudents(students);

    if (students.size() < sz)
        cout << "\n  SUCCESS: Student " << id << " deleted!\n";
    pause();
}

void displayStats() {
    printHeader();
    cout << "  [ STATISTICS ]\n";
    printLine();

    vector<Student> students = loadStudents();
    if (students.empty()) {
        cout << "  No data available.\n"; pause(); return;
    }

    float total = 0, hi = students[0].gpa, lo = students[0].gpa;
    Student top = students[0];
    for (const auto& s : students) {
        total += s.gpa;
        if (s.gpa > hi) { hi = s.gpa; top = s; }
        if (s.gpa < lo)   lo = s.gpa;
    }

    cout << "  Total Students : " << students.size() << "\n";
    cout << "  Average GPA    : " << fixed << setprecision(2) << total/students.size() << "\n";
    cout << "  Highest GPA    : " << hi << "\n";
    cout << "  Lowest GPA     : " << lo << "\n";
    cout << "  Top Student    : " << top.name
         << " (ID: " << top.id << ", GPA: " << top.gpa << ")\n";
    printLine();
    pause();
}

// ─────────────────────────────────────────────
//  MAIN
// ─────────────────────────────────────────────
int main() {
    int choice;
    while (true) {
        printHeader();
        cout << "\n";
        cout << "     1.  Add New Student\n";
        cout << "     2.  Display All Students\n";
        cout << "     3.  Search Student by ID\n";
        cout << "     4.  Update Student Record\n";
        cout << "     5.  Delete Student\n";
        cout << "     6.  View Statistics\n";
        cout << "     7.  Exit\n\n";
        printLine();
        cout << "  Enter your choice (1-7): ";

        if (!(cin >> choice)) { cin.clear(); clearInputBuffer(); continue; }

        switch (choice) {
            case 1: addStudent();         break;
            case 2: displayAllStudents(); break;
            case 3: searchStudent();      break;
            case 4: updateStudent();      break;
            case 5: deleteStudent();      break;
            case 6: displayStats();       break;
            case 7:
                cout << "\n  Goodbye! Thank you for using SMS.\n\n";
                return 0;
            default:
                cout << "\n  Invalid choice. Enter 1-7.\n"; pause();
        }
    }
}
