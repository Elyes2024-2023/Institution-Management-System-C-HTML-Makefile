#include "management_system.h"
#include "employee.h"
#include "student.h"

#define EMPLOYEE_FILE "employees.dat"
#define STUDENT_FILE "students.dat"

void displayMainMenu() {
    const char* options[] = {
        "Manage Students",
        "Manage Employees",
        "Generate Reports",
        "Exit"
    };
    printMenu("MAIN MENU", options, 4);
}

void displayStudentMenu() {
    const char* options[] = {
        "Add Student",
        "View All Students",
        "Edit Student",
        "Delete Student",
        "Search Student",
        "Update Attendance",
        "Update Grades",
        "Back to Main Menu"
    };
    printMenu("STUDENT MANAGEMENT", options, 8);
}

void displayEmployeeMenu() {
    const char* options[] = {
        "Add Employee",
        "View All Employees",
        "Edit Employee",
        "Delete Employee",
        "Search Employee",
        "View by Department",
        "View by Gender",
        "Back to Main Menu"
    };
    printMenu("EMPLOYEE MANAGEMENT", options, 8);
}

void handleStudentManagement(FILE* studentFile) {
    while (1) {
        displayStudentMenu();
        int choice = getChoice(8);
        
        switch (choice) {
            case 1: addStudent(studentFile); break;
            case 2: displayStudentList(studentFile); break;
            case 3: modifyStudent(studentFile); break;
            case 4: deleteStudent(studentFile); break;
            case 5: searchStudent(studentFile); break;
            case 6: {
                int rollNumber;
                printf("Enter Roll Number: ");
                scanf("%d", &rollNumber);
                updateAttendance(studentFile, rollNumber, 1);
                break;
            }
            case 7: {
                int rollNumber;
                float gpa;
                printf("Enter Roll Number: ");
                scanf("%d", &rollNumber);
                printf("Enter new GPA: ");
                scanf("%f", &gpa);
                updateGrades(studentFile, rollNumber, gpa);
                break;
            }
            case 8: return;
        }
    }
}

void handleEmployeeManagement(FILE* employeeFile) {
    while (1) {
        displayEmployeeMenu();
        int choice = getChoice(8);
        
        switch (choice) {
            case 1: addEmployee(employeeFile); break;
            case 2: displayEmployeeList(employeeFile); break;
            case 3: modifyEmployee(employeeFile); break;
            case 4: deleteEmployee(employeeFile); break;
            case 5: searchEmployee(employeeFile); break;
            case 6: {
                char department[50];
                printf("Enter Department: ");
                scanf("%s", department);
                displayEmployeeByDepartment(employeeFile, department);
                break;
            }
            case 7: {
                char gender[10];
                printf("Enter Gender: ");
                scanf("%s", gender);
                displayEmployeeByGender(employeeFile, gender);
                break;
            }
            case 8: return;
        }
    }
}

int main() {
    FILE *employeeFile = openFile(EMPLOYEE_FILE, "rb+");
    FILE *studentFile = openFile(STUDENT_FILE, "rb+");
    
    if (!employeeFile || !studentFile) {
        printf("Error opening files!\n");
        return 1;
    }

    while (1) {
        displayMainMenu();
        int choice = getChoice(4);
        
        switch (choice) {
            case 1: handleStudentManagement(studentFile); break;
            case 2: handleEmployeeManagement(employeeFile); break;
            case 3: {
                // TODO: Implement report generation
                printf("Report generation feature coming soon!\n");
                break;
            }
            case 4: {
                closeFile(employeeFile);
                closeFile(studentFile);
                return 0;
            }
        }
    }
    
    return 0;
} 