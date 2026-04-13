

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <stdexcept>
#include <limits>
#include <sstream>

using namespace std;


namespace Color {
    const string RESET   = "\033[0m";
    const string BOLD    = "\033[1m";
    const string RED     = "\033[31m";
    const string GREEN   = "\033[32m";
    const string YELLOW  = "\033[33m";
    const string BLUE    = "\033[34m";
    const string CYAN    = "\033[36m";
    const string MAGENTA = "\033[35m";
    const string WHITE   = "\033[37m";
    const string BG_BLUE = "\033[44m";
    const string BG_NAVY = "\033[34m";
}


class GradeConverter {
public:
    static double letterToPoint(const string& grade) {
        if (grade == "O")  return 10.0;
        if (grade == "A+") return 9.0;
        if (grade == "A")  return 8.0;
        if (grade == "B+") return 7.0;
        if (grade == "B")  return 6.0;
        if (grade == "C")  return 5.0;
        if (grade == "F")  return 0.0;
        throw invalid_argument("Invalid grade: " + grade + ". Use O/A+/A/B+/B/C/F");
    }

    static string pointToLetter(double gp) {
        if (gp >= 10.0) return "O";
        if (gp >= 9.0)  return "A+";
        if (gp >= 8.0)  return "A";
        if (gp >= 7.0)  return "B+";
        if (gp >= 6.0)  return "B";
        if (gp >= 5.0)  return "C";
        return "F";
    }

    static string cgpaToPercentage(double cgpa) {
       
        double pct = cgpa * 9.5;
        ostringstream oss;
        oss << fixed << setprecision(2) << pct << "%";
        return oss.str();
    }

    static string cgpaToClass(double cgpa) {
        if (cgpa >= 9.0) return "First Class with Distinction";
        if (cgpa >= 7.5) return "First Class";
        if (cgpa >= 6.0) return "Second Class";
        if (cgpa >= 5.0) return "Pass Class";
        return "Fail";
    }

    static void printGradingTable() {
    cout << Color::CYAN << Color::BOLD;
    cout << "\n  ┌─────────┬──────────────┬─────────────┬──────────────────┐\n";
    cout << "  │  Grade  │  Marks Range │  Grade Pt   │  Performance     │\n";
    cout << "  ├─────────┼──────────────┼─────────────┼──────────────────┤\n";
    cout << Color::RESET;

    vector<tuple<string,string,string,string>> table = {
        {"O",  "90-100", "10.0", "Outstanding"},
        {"A+", "80-89",  "9.0",  "Excellent"},
        {"A",  "70-79",  "8.0",  "Very Good"},
        {"B+", "60-69",  "7.0",  "Good"},
        {"B",  "55-59",  "6.0",  "Above Average"},
        {"C",  "50-54",  "5.0",  "Average"},
        {"F",  "< 50",   "0.0",  "Fail"},
    };

    for (auto& [g, m, p, perf] : table) {
        cout << Color::CYAN << "  │" << Color::RESET;
        cout << "  " << setw(5) << left << g
             << Color::CYAN << "  │" << Color::RESET
             << "  " << setw(10) << left << m
             << Color::CYAN << "  │" << Color::RESET
             << "  " << setw(9) << left << p
             << Color::CYAN << "  │" << Color::RESET
             << "  " << setw(16) << left << perf
             << Color::CYAN << "│\n" << Color::RESET;
    }

    cout << Color::CYAN << "  └─────────┴──────────────┴─────────────┴──────────────────┘\n" << Color::RESET;
}
};


class Course {
private:
    string name;
    int    credits;
    string grade;
    double gradePoint;

public:
   
    Course(const string& n, int c, const string& g)
        : name(n), credits(c), grade(g) {
        gradePoint = GradeConverter::letterToPoint(g);
    }

   
    string getName()       const { return name; }
    int    getCredits()    const { return credits; }
    string getGrade()      const { return grade; }
    double getGradePoint() const { return gradePoint; }
    double getWeighted()   const { return gradePoint * credits; }

    void display(int idx) const {
        cout << "  " << Color::YELLOW << setw(3) << idx << Color::RESET
             << "  " << setw(24) << left << name
             << setw(8)  << credits
             << setw(8)  << grade
             << setw(10) << fixed << setprecision(1) << gradePoint
             << setw(10) << fixed << setprecision(1) << getWeighted()
             << "\n";
    }

    string serialize() const {
        return name + "|" + to_string(credits) + "|" + grade;
    }
};


class AcademicRecord {
public:
    virtual double calculateGPA()  const = 0;
    virtual int    totalCredits()  const = 0;
    virtual void   display()       const = 0;
    virtual string getSummary()    const = 0;
    virtual ~AcademicRecord() {}
};


class Semester : public AcademicRecord {
private:
    int             semNumber;
    vector<Course>  courses;

public:
    explicit Semester(int n) : semNumber(n) {}


    void addCourse(const Course& c) {
        courses.push_back(c);
    }

    int getSemNumber() const { return semNumber; }
    const vector<Course>& getCourses() const { return courses; }
    bool isEmpty() const { return courses.empty(); }

    double calculateGPA() const override {
        if (courses.empty()) return 0.0;
        double totalWeighted = 0.0;
        int    totalCreds    = 0;
        for (const auto& c : courses) {
            totalWeighted += c.getWeighted();
            totalCreds    += c.getCredits();
        }
        return (totalCreds > 0) ? (totalWeighted / totalCreds) : 0.0;
    }
    int totalCredits() const override {
        int sum = 0;
        for (const auto& c : courses) sum += c.getCredits();
        return sum;
    }

    double totalWeightedPoints() const {
        double sum = 0.0;
        for (const auto& c : courses) sum += c.getWeighted();
        return sum;
    }

    void display() const override {
        cout << "\n" << Color::BG_BLUE << Color::BOLD
             << "  ══ SEMESTER " << semNumber << " ══"
             << Color::RESET << "\n\n";
        cout << Color::BOLD;
        cout << "  " << setw(5) << "#"
             << setw(24) << left << "Course Name"
             << setw(8)  << "Credits"
             << setw(8)  << "Grade"
             << setw(10) << "Gr.Pt"
             << setw(10) << "Weighted"
             << "\n";
        cout << Color::RESET;
        cout << "  " << string(65, '-') << "\n";
        int i = 1;
        for (const auto& c : courses) c.display(i++);
        cout << "  " << string(65, '-') << "\n";
        cout << Color::BOLD << Color::GREEN
             << "  Total Credits : " << totalCredits() << "\n"
             << "  SGPA          : " << fixed << setprecision(2) << calculateGPA()
             << "  (" << GradeConverter::pointToLetter(calculateGPA()) << ")\n"
             << Color::RESET;
    }

    
    string getSummary() const override {
        ostringstream oss;
        oss << "Sem " << semNumber
            << " | Credits: " << totalCredits()
            << " | SGPA: " << fixed << setprecision(2) << calculateGPA();
        return oss.str();
    }

  
    string serialize() const {
        string result = "SEM:" + to_string(semNumber) + "\n";
        for (const auto& c : courses)
            result += c.serialize() + "\n";
        return result;
    }
};


class Student : public AcademicRecord {
private:
    string           name;
    string           rollNo;
    string           department;
    vector<Semester> semesters;

public:
    Student(const string& n, const string& r, const string& dept)
        : name(n), rollNo(r), department(dept) {}

    string getName()       const { return name; }
    string getRollNo()     const { return rollNo; }
    string getDepartment() const { return department; }
    int    semCount()      const { return (int)semesters.size(); }

    void addSemester(const Semester& s) {
        semesters.push_back(s);
    }

    Semester& getLastSemester() {
        if (semesters.empty()) throw runtime_error("No semesters added yet.");
        return semesters.back();
    }

    const vector<Semester>& getSemesters() const { return semesters; }

    double calculateGPA() const override {
        double totalW = 0.0;
        int    totalC = 0;
        for (const auto& sem : semesters) {
            totalW += sem.totalWeightedPoints();
            totalC += sem.totalCredits();
        }
        return (totalC > 0) ? (totalW / totalC) : 0.0;
    }

    int totalCredits() const override {
        int sum = 0;
        for (const auto& s : semesters) sum += s.totalCredits();
        return sum;
    }

    void display() const override {
        printHeader();
        for (const auto& s : semesters) s.display();
        printCGPASummary();
    }

    string getSummary() const override {
        ostringstream oss;
        oss << name << " (" << rollNo << ") | "
            << department << " | "
            << semesters.size() << " semesters | "
            << "CGPA: " << fixed << setprecision(2) << calculateGPA();
        return oss.str();
    }

    void printHeader() const {
        cout << "\n" << Color::BOLD << Color::CYAN;
        cout << "  ╔══════════════════════════════════════════════════════════╗\n";
        cout << "  ║              STUDENT ACADEMIC TRANSCRIPT                 ║\n";
        cout << "  ╚══════════════════════════════════════════════════════════╝\n";
        cout << Color::RESET;
        cout << Color::BOLD
             << "  Name       : " << Color::WHITE << name        << "\n" << Color::RESET
             << Color::BOLD
             << "  Roll No    : " << Color::WHITE << rollNo      << "\n" << Color::RESET
             << Color::BOLD
             << "  Department : " << Color::WHITE << department  << "\n" << Color::RESET;
    }

    void printCGPASummary() const {
        double cgpa = calculateGPA();
        cout << "\n" << Color::YELLOW << Color::BOLD;
        cout << "  ┌─────────────────────────────────────────┐\n";
        cout << "  │            CGPA SUMMARY                 │\n";
        cout << "  ├─────────────────────────────────────────┤\n";

        // Per-semester SGPA
        for (const auto& s : semesters) {
            cout << "  │  " << setw(35) << left
                 << ("Sem " + to_string(s.getSemNumber()) + " SGPA: "
                     + to_string(s.calculateGPA()).substr(0,4))
                 << "     │\n";
        }
        cout << "  ├─────────────────────────────────────────┤\n";
        cout << "  │  Total Credits  : " << setw(5) << totalCredits() << "                  │\n";
        cout << "  │  Overall CGPA   : " << fixed << setprecision(2)
             << cgpa << "                    │\n";
        cout << "  │  Percentage     : "
             << setw(7) << GradeConverter::cgpaToPercentage(cgpa) << "               │\n";
        cout << "  │  Classification : "
             << setw(22) << left << GradeConverter::cgpaToClass(cgpa) << "│\n";
        cout << "  └─────────────────────────────────────────┘\n";
        cout << Color::RESET;
    }

    void whatIfSimulation(double hypotheticalGP, int hypotheticalCredits) const {
        double totalW = 0.0;
        int    totalC = 0;
        for (const auto& sem : semesters) {
            totalW += sem.totalWeightedPoints();
            totalC += sem.totalCredits();
        }
        totalW += hypotheticalGP * hypotheticalCredits;
        totalC += hypotheticalCredits;
        double newCGPA = (totalC > 0) ? (totalW / totalC) : 0.0;

        cout << "\n" << Color::MAGENTA << Color::BOLD;
        cout << "  ┌──────────────────────────────────────────┐\n";
        cout << "  │          WHAT-IF SIMULATION              │\n";
        cout << "  ├──────────────────────────────────────────┤\n";
        cout << "  │  Hypothetical GPA  : " << setw(6) << fixed << setprecision(1) << hypotheticalGP
             << "              │\n";
        cout << "  │  Hypothetical Cred : " << setw(6) << hypotheticalCredits
             << "              │\n";
        cout << "  │  Projected CGPA    : " << setw(6) << fixed << setprecision(2) << newCGPA
             << "              │\n";
        cout << "  │  Classification    : "
             << setw(22) << left << GradeConverter::cgpaToClass(newCGPA) << "│\n";
        cout << "  └──────────────────────────────────────────┘\n";
        cout << Color::RESET;
    }

    string serialize() const {
        string result = "STUDENT:" + name + "|" + rollNo + "|" + department + "\n";
        for (const auto& s : semesters) result += s.serialize();
        result += "END\n";
        return result;
    }
};


class FileManager {
public:
    static void saveStudent(const Student& student, const string& filename) {
        ofstream file(filename);
        if (!file.is_open())
            throw runtime_error("Cannot open file for writing: " + filename);
        file << student.serialize();
        file.close();
        cout << Color::GREEN << "\n  ✓ Data saved to \"" << filename << "\"\n" << Color::RESET;
    }

    static void exportReport(const Student& student, const string& filename) {
        ofstream file(filename);
        if (!file.is_open())
            throw runtime_error("Cannot open file: " + filename);

        file << "========================================\n";
        file << "       CGPA CALCULATOR — REPORT         \n";
        file << "========================================\n";
        file << "Name       : " << student.getName()       << "\n";
        file << "Roll No    : " << student.getRollNo()      << "\n";
        file << "Department : " << student.getDepartment()  << "\n\n";

        for (const auto& sem : student.getSemesters()) {
            file << "--- Semester " << sem.getSemNumber() << " ---\n";
            for (const auto& c : sem.getCourses()) {
                file << "  " << left << setw(22) << c.getName()
                     << " Credits: " << c.getCredits()
                     << "  Grade: "  << c.getGrade()
                     << "  GP: "     << fixed << setprecision(1) << c.getGradePoint()
                     << "\n";
            }
            file << "  SGPA: " << fixed << setprecision(2) << sem.calculateGPA()
                 << "  (Credits: " << sem.totalCredits() << ")\n\n";
        }

        file << "========================================\n";
        file << "  Overall CGPA   : " << fixed << setprecision(2) << student.calculateGPA() << "\n";
        file << "  Percentage     : " << GradeConverter::cgpaToPercentage(student.calculateGPA()) << "\n";
        file << "  Classification : " << GradeConverter::cgpaToClass(student.calculateGPA()) << "\n";
        file << "  Total Credits  : " << student.totalCredits() << "\n";
        file << "========================================\n";
        file.close();

        cout << Color::GREEN << "  ✓ Report exported to \"" << filename << "\"\n" << Color::RESET;
    }
};


class InputHandler {
public:
    static int getInt(const string& prompt, int minVal = 1, int maxVal = 100) {
        int val;
        while (true) {
            cout << prompt;
            if (cin >> val && val >= minVal && val <= maxVal) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return val;
            }
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << Color::RED << "  ✗ Invalid input. Enter a number between "
                 << minVal << " and " << maxVal << ".\n" << Color::RESET;
        }
    }

    static string getString(const string& prompt) {
        string val;
        cout << prompt;
        getline(cin, val);
        while (val.empty()) {
            cout << Color::RED << "  ✗ Cannot be empty. " << Color::RESET;
            cout << prompt;
            getline(cin, val);
        }
        return val;
    }

    static string getGrade(const string& prompt) {
        vector<string> valid = {"O","A+","A","B+","B","C","F"};
        while (true) {
            cout << prompt;
            string g;
            cin >> g;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            // Uppercase
            transform(g.begin(), g.end(), g.begin(), ::toupper);
            if (find(valid.begin(), valid.end(), g) != valid.end())
                return g;
            cout << Color::RED << "  ✗ Invalid grade. Use: O / A+ / A / B+ / B / C / F\n" << Color::RESET;
        }
    }
};


class CGPACalculatorApp {
private:
    Student* currentStudent = nullptr;

    void printBanner() {
        cout << Color::CYAN << Color::BOLD << R"(
  ╔═══════════════════════════════════════════════════════════╗
  ║                                                           ║
  ║        ██████╗ ██████╗ ██████╗  █████╗                   ║
  ║       ██╔════╝██╔════╝ ██╔══██╗██╔══██╗                  ║
  ║       ██║     ██║  ███╗██████╔╝███████║                  ║
  ║       ██║     ██║   ██║██╔═══╝ ██╔══██║                  ║
  ║       ╚██████╗╚██████╔╝██║     ██║  ██║                  ║
  ║        ╚═════╝ ╚═════╝ ╚═╝     ╚═╝  ╚═╝                  ║
  ║                                                           ║
  ║          C G P A   C A L C U L A T O R                   ║
  ║                   ║
  ╚═══════════════════════════════════════════════════════════╝
)" << Color::RESET;
    }

    void printMenu() {
        cout << Color::BOLD << Color::BLUE
             << "\n  ┌─────────────── MAIN MENU ───────────────┐\n"
             << Color::RESET;
        cout << "  │  1. Add New Student                     │\n"
             << "  │  2. Add Semester                        │\n"
             << "  │  3. View Full Transcript                │\n"
             << "  │  4. View CGPA Summary                   │\n"
             << "  │  5. What-If CGPA Simulator              │\n"
             << "  │  6. View Grading Scale                  │\n"
             << "  │  7. Export Report to File               │\n"
             << "  │  8. Save Data                           │\n"
             << "  │  9. Exit                                │\n";
        cout << Color::BLUE << "  └─────────────────────────────────────────┘\n" << Color::RESET;
        cout << Color::BOLD << "  Choice: " << Color::RESET;
    }

    void createStudent() {
        cout << Color::YELLOW << Color::BOLD << "\n  ── New Student Registration ──\n" << Color::RESET;
        string name = InputHandler::getString("  Enter Name       : ");
        string roll = InputHandler::getString("  Enter Roll No    : ");
        string dept = InputHandler::getString("  Enter Department : ");

        delete currentStudent;
        currentStudent = new Student(name, roll, dept);
        cout << Color::GREEN << "\n  ✓ Student \"" << name << "\" created successfully!\n" << Color::RESET;
    }

    void addSemester() {
        if (!currentStudent) {
            cout << Color::RED << "  ✗ Please create a student first (Option 1).\n" << Color::RESET;
            return;
        }
        int semNum = currentStudent->semCount() + 1;
        cout << Color::YELLOW << Color::BOLD << "\n  ── Adding Semester " << semNum << " ──\n" << Color::RESET;

        Semester sem(semNum);
        int numCourses = InputHandler::getInt("  How many courses in Semester " + to_string(semNum) + "? ", 1, 15);

        for (int i = 1; i <= numCourses; i++) {
            cout << Color::CYAN << "\n  [ Course " << i << " ]\n" << Color::RESET;
            string cname   = InputHandler::getString("    Course Name   : ");
            int    credits = InputHandler::getInt("    Credit Hours  : ", 1, 6);
            cout << "    Grade (O/A+/A/B+/B/C/F): ";

            while (true) {
                try {
                    string grade = InputHandler::getGrade("");
                    Course course(cname, credits, grade);
                    sem.addCourse(course);
                    cout << Color::GREEN
                         << "    ✓ Added — Grade Point: " << fixed << setprecision(1)
                         << course.getGradePoint()
                         << " | Weighted: " << course.getWeighted()
                         << Color::RESET << "\n";
                    break;
                } catch (const invalid_argument& e) {
                    cout << Color::RED << "  ✗ " << e.what() << "\n" << Color::RESET;
                }
            }
        }

        cout << Color::GREEN << Color::BOLD
             << "\n  Semester " << semNum << " SGPA: "
             << fixed << setprecision(2) << sem.calculateGPA()
             << " (" << GradeConverter::pointToLetter(sem.calculateGPA()) << ")\n"
             << Color::RESET;

        currentStudent->addSemester(sem);
    }

    void viewTranscript() {
        if (!currentStudent) {
            cout << Color::RED << "  ✗ No student data. Create a student first.\n" << Color::RESET;
            return;
        }
        if (currentStudent->semCount() == 0) {
            cout << Color::RED << "  ✗ No semesters added yet.\n" << Color::RESET;
            return;
        }
        currentStudent->display();
    }

    void viewCGPASummary() {
        if (!currentStudent || currentStudent->semCount() == 0) {
            cout << Color::RED << "  ✗ No data available.\n" << Color::RESET;
            return;
        }
        currentStudent->printHeader();
        currentStudent->printCGPASummary();
    }

    void whatIfSimulator() {
        if (!currentStudent || currentStudent->semCount() == 0) {
            cout << Color::RED << "  ✗ Please add at least one semester first.\n" << Color::RESET;
            return;
        }
        cout << Color::YELLOW << Color::BOLD << "\n  ── What-If CGPA Simulator ──\n" << Color::RESET;
        cout << "  (Simulate the effect of a hypothetical future semester)\n";

        cout << "  Enter hypothetical SGPA (e.g. 8 for Grade A, 9 for A+): ";
        double gp;
        while (!(cin >> gp) || gp < 0 || gp > 10) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << Color::RED << "  ✗ Enter value between 0 and 10: " << Color::RESET;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        int credits = InputHandler::getInt("  Enter hypothetical credit hours: ", 1, 40);
        currentStudent->whatIfSimulation(gp, credits);
    }

    void exportReport() {
        if (!currentStudent || currentStudent->semCount() == 0) {
            cout << Color::RED << "  ✗ No data to export.\n" << Color::RESET;
            return;
        }
        string fname = currentStudent->getRollNo() + "_report.txt";
        try {
            FileManager::exportReport(*currentStudent, fname);
        } catch (const exception& e) {
            cout << Color::RED << "  ✗ Error: " << e.what() << "\n" << Color::RESET;
        }
    }

    void saveData() {
        if (!currentStudent) {
            cout << Color::RED << "  ✗ No student data to save.\n" << Color::RESET;
            return;
        }
        string fname = currentStudent->getRollNo() + "_data.txt";
        try {
            FileManager::saveStudent(*currentStudent, fname);
        } catch (const exception& e) {
            cout << Color::RED << "  ✗ Error: " << e.what() << "\n" << Color::RESET;
        }
    }

public:
    ~CGPACalculatorApp() {
        delete currentStudent;
    }

    void run() {
        printBanner();
        int choice;
        do {
            printMenu();
            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                choice = 0;
            } else {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            switch (choice) {
                case 1: createStudent();           break;
                case 2: addSemester();             break;
                case 3: viewTranscript();          break;
                case 4: viewCGPASummary();         break;
                case 5: whatIfSimulator();         break;
                case 6: GradeConverter::printGradingTable(); break;
                case 7: exportReport();            break;
                case 8: saveData();                break;
                case 9:
                    cout << Color::CYAN << Color::BOLD
                         << "\n  Thank you for using CGPA Calculator. Goodbye!\n"
                         << Color::RESET;
                    break;
                default:
                    cout << Color::RED << "  ✗ Invalid choice. Enter 1-9.\n" << Color::RESET;
            }
        } while (choice != 9);
    }
};


int main() {
    try {
        CGPACalculatorApp app;
        app.run();
    } catch (const exception& e) {
        cerr << Color::RED << "\n  Fatal Error: " << e.what() << "\n" << Color::RESET;
        return 1;
    }
    return 0;
}