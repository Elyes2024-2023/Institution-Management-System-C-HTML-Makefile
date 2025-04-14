/*
 * Copyright (c) 2024-2025 ELYES. All rights reserved.
 * 
 * This file is part of the ELYES Employee and Student Management System Library.
 * Created by ELYES
 */

#include "management_lib.h"
#include "management_system.h"
#include "student.h"
#include "employee.h"
#include <string.h>
#include <stdio.h>

// Constants - Done by ELYES
#define MAX_ERROR_LENGTH 256
#define MAX_BUFFER_SIZE 1024
#define MAX_NAME_LENGTH 100
#define MAX_STRING_LENGTH 256
#define STUDENT_FILE "elyes_students.dat"
#define EMPLOYEE_FILE "elyes_employees.dat"

// Global variables for file handles - Done by ELYES
static FILE* student_file = NULL;
static FILE* employee_file = NULL;
static char last_error[MAX_ERROR_LENGTH] = {0};

// Initialize the library - Done by ELYES
static int initialize() {
    if (!student_file) {
        student_file = openFile(STUDENT_FILE, "rb+");
        if (!student_file) {
            strncpy(last_error, "Failed to open elyes_students.dat", MAX_ERROR_LENGTH - 1);
            last_error[MAX_ERROR_LENGTH - 1] = '\0';
            return 0;
        }
    }
    
    if (!employee_file) {
        employee_file = openFile(EMPLOYEE_FILE, "rb+");
        if (!employee_file) {
            strncpy(last_error, "Failed to open elyes_employees.dat", MAX_ERROR_LENGTH - 1);
            last_error[MAX_ERROR_LENGTH - 1] = '\0';
            return 0;
        }
    }
    
    return 1;
}

// Validate string length
static int validate_string(const char* str, size_t max_length) {
    if (!str) return 0;
    size_t len = strlen(str);
    return len > 0 && len < max_length;
}

// Validate file handles
static int validate_file_handles() {
    if (!student_file || !employee_file) {
        return initialize();
    }
    return 1;
}

// Set error message
static void set_error(const char* message) {
    strncpy(last_error, message, MAX_ERROR_LENGTH - 1);
    last_error[MAX_ERROR_LENGTH - 1] = '\0';
}

// Get last error message
const char* get_last_error() {
    return last_error;
}

// Get library version
const char* get_version() {
    return MANAGEMENT_LIB_VERSION;
}

// Cleanup resources
void cleanup() {
    if (student_file) {
        fclose(student_file);
        student_file = NULL;
    }
    if (employee_file) {
        fclose(employee_file);
        employee_file = NULL;
    }
}

// Student management functions
int add_student(const char* name, const char* gender, const char* phone, 
                const char* email, const char* address, const char* class_name,
                int age, float gpa, const char* enrollment_date, int attendance) {
    if (!validate_file_handles()) return -1;
    
    // Validate input strings
    if (!validate_string(name, MAX_NAME_LENGTH) ||
        !validate_string(gender, MAX_STRING_LENGTH) ||
        !validate_string(phone, MAX_STRING_LENGTH) ||
        !validate_string(email, MAX_STRING_LENGTH) ||
        !validate_string(address, MAX_STRING_LENGTH) ||
        !validate_string(class_name, MAX_STRING_LENGTH) ||
        !validate_string(enrollment_date, MAX_STRING_LENGTH)) {
        set_error("Invalid input string length");
        return -2;
    }
    
    // Validate numeric values
    if (age < 0 || age > 150) {
        set_error("Invalid age");
        return -2;
    }
    if (gpa < 0.0f || gpa > 4.0f) {
        set_error("Invalid GPA");
        return -2;
    }
    if (attendance < 0 || attendance > 100) {
        set_error("Invalid attendance percentage");
        return -2;
    }
    
    Student student;
    student.roll_number = getNextRollNumber(student_file);
    
    // Copy data to student structure
    strncpy(student.base.name, name, sizeof(student.base.name) - 1);
    strncpy(student.base.gender, gender, sizeof(student.base.gender) - 1);
    strncpy(student.base.phone, phone, sizeof(student.base.phone) - 1);
    strncpy(student.base.email, email, sizeof(student.base.email) - 1);
    strncpy(student.base.address, address, sizeof(student.base.address) - 1);
    strncpy(student.class, class_name, sizeof(student.class) - 1);
    student.age = age;
    student.gpa = gpa;
    strncpy(student.enrollment_date, enrollment_date, sizeof(student.enrollment_date) - 1);
    student.attendance = attendance;
    
    // Validate student data
    if (!isValidStudent(&student)) {
        set_error("Invalid student data");
        return 0;
    }
    
    // Save student
    saveStudent(student_file, &student);
    return student.roll_number;
}

int delete_student(int roll_number) {
    if (!initialize()) return 0;
    
    Student* student = loadStudent(student_file, roll_number);
    if (!student) {
        set_error("Student not found");
        return 0;
    }
    
    // Create temporary file
    FILE* temp = fopen("temp.dat", "wb+");
    if (!temp) {
        set_error("Error creating temporary file");
        free(student);
        return 0;
    }
    
    // Copy all records except the one to delete
    Student current;
    fseek(student_file, 0, SEEK_SET);
    while (fread(&current, sizeof(Student), 1, student_file) == 1) {
        if (current.roll_number != roll_number) {
            fwrite(&current, sizeof(Student), 1, temp);
        }
    }
    
    // Replace original file with temporary file
    fclose(student_file);
    remove("elyes_students.dat");
    rename("temp.dat", "elyes_students.dat");
    student_file = fopen("elyes_students.dat", "rb+");
    
    free(student);
    return 1;
}

int modify_student(int roll_number, const char* name, const char* gender, 
                  const char* phone, const char* email, const char* address, 
                  const char* class_name, int age, float gpa, 
                  const char* enrollment_date, int attendance) {
    if (!initialize()) return 0;
    
    Student* student = loadStudent(student_file, roll_number);
    if (!student) {
        set_error("Student not found");
        return 0;
    }
    
    // Update student data
    strncpy(student->base.name, name, sizeof(student->base.name) - 1);
    strncpy(student->base.gender, gender, sizeof(student->base.gender) - 1);
    strncpy(student->base.phone, phone, sizeof(student->base.phone) - 1);
    strncpy(student->base.email, email, sizeof(student->base.email) - 1);
    strncpy(student->base.address, address, sizeof(student->base.address) - 1);
    strncpy(student->class, class_name, sizeof(student->class) - 1);
    student->age = age;
    student->gpa = gpa;
    strncpy(student->enrollment_date, enrollment_date, sizeof(student->enrollment_date) - 1);
    student->attendance = attendance;
    
    // Validate student data
    if (!isValidStudent(student)) {
        set_error("Invalid student data");
        free(student);
        return 0;
    }
    
    // Update the record
    fseek(student_file, -(long)sizeof(Student), SEEK_CUR);
    fwrite(student, sizeof(Student), 1, student_file);
    
    free(student);
    return 1;
}

int get_student_count() {
    if (!initialize()) return 0;
    
    Student student;
    int count = 0;
    
    fseek(student_file, 0, SEEK_SET);
    while (fread(&student, sizeof(Student), 1, student_file) == 1) {
        count++;
    }
    
    return count;
}

int get_student_by_index(int index, char* name, char* gender, char* phone, 
                        char* email, char* address, char* class_name, 
                        int* age, float* gpa, char* enrollment_date, 
                        int* attendance, int* roll_number) {
    if (!initialize()) return 0;
    
    Student student;
    int current_index = 0;
    
    fseek(student_file, 0, SEEK_SET);
    while (fread(&student, sizeof(Student), 1, student_file) == 1) {
        if (current_index == index) {
            strcpy(name, student.base.name);
            strcpy(gender, student.base.gender);
            strcpy(phone, student.base.phone);
            strcpy(email, student.base.email);
            strcpy(address, student.base.address);
            strcpy(class_name, student.class);
            *age = student.age;
            *gpa = student.gpa;
            strcpy(enrollment_date, student.enrollment_date);
            *attendance = student.attendance;
            *roll_number = student.roll_number;
            return 1;
        }
        current_index++;
    }
    
    set_error("Student not found at index");
    return 0;
}

int search_student_by_name(const char* name, int* results, int max_results) {
    if (!initialize()) return 0;
    
    Student student;
    int count = 0;
    
    fseek(student_file, 0, SEEK_SET);
    while (fread(&student, sizeof(Student), 1, student_file) == 1 && count < max_results) {
        if (strstr(student.base.name, name) != NULL) {
            results[count++] = student.roll_number;
        }
    }
    
    return count;
}

int get_student_by_roll(int roll_number, char* name, char* gender, char* phone, 
                       char* email, char* address, char* class_name, 
                       int* age, float* gpa, char* enrollment_date, 
                       int* attendance) {
    if (!initialize()) return 0;
    
    Student* student = loadStudent(student_file, roll_number);
    if (!student) {
        set_error("Student not found");
        return 0;
    }
    
    strcpy(name, student->base.name);
    strcpy(gender, student->base.gender);
    strcpy(phone, student->base.phone);
    strcpy(email, student->base.email);
    strcpy(address, student->base.address);
    strcpy(class_name, student->class);
    *age = student->age;
    *gpa = student->gpa;
    strcpy(enrollment_date, student->enrollment_date);
    *attendance = student->attendance;
    
    free(student);
    return 1;
}

// Employee management functions
int add_employee(const char* name, const char* gender, const char* phone, 
                 const char* email, const char* address, const char* designation,
                 float salary, const char* department, const char* join_date) {
    if (!initialize()) return 0;
    
    Employee emp;
    emp.base.id = getNextEmployeeId(employee_file);
    
    // Copy data to employee structure
    strncpy(emp.base.name, name, sizeof(emp.base.name) - 1);
    strncpy(emp.base.gender, gender, sizeof(emp.base.gender) - 1);
    strncpy(emp.base.phone, phone, sizeof(emp.base.phone) - 1);
    strncpy(emp.base.email, email, sizeof(emp.base.email) - 1);
    strncpy(emp.base.address, address, sizeof(emp.base.address) - 1);
    strncpy(emp.designation, designation, sizeof(emp.designation) - 1);
    emp.salary = salary;
    strncpy(emp.department, department, sizeof(emp.department) - 1);
    strncpy(emp.join_date, join_date, sizeof(emp.join_date) - 1);
    
    // Validate employee data
    if (!isValidEmployee(&emp)) {
        set_error("Invalid employee data");
        return 0;
    }
    
    // Save employee
    saveEmployee(employee_file, &emp);
    return emp.base.id;
}

int delete_employee(int id) {
    if (!initialize()) return 0;
    
    Employee* emp = loadEmployee(employee_file, id);
    if (!emp) {
        set_error("Employee not found");
        return 0;
    }
    
    // Create temporary file
    FILE* temp = fopen("temp.dat", "wb+");
    if (!temp) {
        set_error("Error creating temporary file");
        free(emp);
        return 0;
    }
    
    // Copy all records except the one to delete
    Employee current;
    fseek(employee_file, 0, SEEK_SET);
    while (fread(&current, sizeof(Employee), 1, employee_file) == 1) {
        if (current.base.id != id) {
            fwrite(&current, sizeof(Employee), 1, temp);
        }
    }
    
    // Replace original file with temporary file
    fclose(employee_file);
    remove("elyes_employees.dat");
    rename("temp.dat", "elyes_employees.dat");
    employee_file = fopen("elyes_employees.dat", "rb+");
    
    free(emp);
    return 1;
}

int modify_employee(int id, const char* name, const char* gender, const char* phone, 
                   const char* email, const char* address, const char* designation,
                   float salary, const char* department, const char* join_date) {
    if (!initialize()) return 0;
    
    Employee* emp = loadEmployee(employee_file, id);
    if (!emp) {
        set_error("Employee not found");
        return 0;
    }
    
    // Update employee data
    strncpy(emp->base.name, name, sizeof(emp->base.name) - 1);
    strncpy(emp->base.gender, gender, sizeof(emp->base.gender) - 1);
    strncpy(emp->base.phone, phone, sizeof(emp->base.phone) - 1);
    strncpy(emp->base.email, email, sizeof(emp->base.email) - 1);
    strncpy(emp->base.address, address, sizeof(emp->base.address) - 1);
    strncpy(emp->designation, designation, sizeof(emp->designation) - 1);
    emp->salary = salary;
    strncpy(emp->department, department, sizeof(emp->department) - 1);
    strncpy(emp->join_date, join_date, sizeof(emp->join_date) - 1);
    
    // Validate employee data
    if (!isValidEmployee(emp)) {
        set_error("Invalid employee data");
        free(emp);
        return 0;
    }
    
    // Update the record
    fseek(employee_file, -(long)sizeof(Employee), SEEK_CUR);
    fwrite(emp, sizeof(Employee), 1, employee_file);
    
    free(emp);
    return 1;
}

int get_employee_count() {
    if (!initialize()) return 0;
    
    Employee emp;
    int count = 0;
    
    fseek(employee_file, 0, SEEK_SET);
    while (fread(&emp, sizeof(Employee), 1, employee_file) == 1) {
        count++;
    }
    
    return count;
}

int get_employee_by_index(int index, char* name, char* gender, char* phone, 
                         char* email, char* address, char* designation,
                         float* salary, char* department, char* join_date, 
                         int* id) {
    if (!initialize()) return 0;
    
    Employee emp;
    int current_index = 0;
    
    fseek(employee_file, 0, SEEK_SET);
    while (fread(&emp, sizeof(Employee), 1, employee_file) == 1) {
        if (current_index == index) {
            strcpy(name, emp.base.name);
            strcpy(gender, emp.base.gender);
            strcpy(phone, emp.base.phone);
            strcpy(email, emp.base.email);
            strcpy(address, emp.base.address);
            strcpy(designation, emp.designation);
            *salary = emp.salary;
            strcpy(department, emp.department);
            strcpy(join_date, emp.join_date);
            *id = emp.base.id;
            return 1;
        }
        current_index++;
    }
    
    set_error("Employee not found at index");
    return 0;
}

int search_employee_by_name(const char* name, int* results, int max_results) {
    if (!initialize()) return 0;
    
    Employee emp;
    int count = 0;
    
    fseek(employee_file, 0, SEEK_SET);
    while (fread(&emp, sizeof(Employee), 1, employee_file) == 1 && count < max_results) {
        if (strstr(emp.base.name, name) != NULL) {
            results[count++] = emp.base.id;
        }
    }
    
    return count;
}

int get_employee_by_id(int id, char* name, char* gender, char* phone, 
                      char* email, char* address, char* designation,
                      float* salary, char* department, char* join_date) {
    if (!initialize()) return 0;
    
    Employee* emp = loadEmployee(employee_file, id);
    if (!emp) {
        set_error("Employee not found");
        return 0;
    }
    
    strcpy(name, emp->base.name);
    strcpy(gender, emp->base.gender);
    strcpy(phone, emp->base.phone);
    strcpy(email, emp->base.email);
    strcpy(address, emp->base.address);
    strcpy(designation, emp->designation);
    *salary = emp->salary;
    strcpy(department, emp->department);
    strcpy(join_date, emp->join_date);
    
    free(emp);
    return 1;
} 