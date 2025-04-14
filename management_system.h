#ifndef MANAGEMENT_SYSTEM_H
#define MANAGEMENT_SYSTEM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Common structures
typedef struct {
    int id;
    char name[100];
    char gender[10];
    char phone[15];
    char email[50];
    char address[200];
} Person;

typedef struct {
    Person base;
    char designation[50];
    float salary;
    char department[50];
    char join_date[11];
} Employee;

typedef struct {
    Person base;
    int roll_number;
    char class[20];
    int age;
    float gpa;
    char enrollment_date[11];
    int attendance;
} Student;

// Function declarations
void clearScreen();
void printHeader(const char* title);
void printMenu(const char* title, const char* options[], int numOptions);
int getChoice(int maxOptions);

// File operations
FILE* openFile(const char* filename, const char* mode);
void closeFile(FILE* fp);

// Validation functions
int isValidEmail(const char* email);
int isValidPhone(const char* phone);
int isValidDate(const char* date);

#endif // MANAGEMENT_SYSTEM_H 