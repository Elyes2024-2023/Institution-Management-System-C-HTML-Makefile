#ifndef STUDENT_H
#define STUDENT_H

#include "management_system.h"

// Student management functions
void addStudent(FILE* fp);
void deleteStudent(FILE* fp);
void modifyStudent(FILE* fp);
void displayStudentList(FILE* fp);
void searchStudent(FILE* fp);
void displayStudentByClass(FILE* fp, const char* class);
void updateAttendance(FILE* fp, int rollNumber, int present);
void updateGrades(FILE* fp, int rollNumber, float gpa);

// Student file operations
void saveStudent(FILE* fp, const Student* student);
Student* loadStudent(FILE* fp, int rollNumber);
int getNextRollNumber(FILE* fp);

// Student validation
int isValidStudent(const Student* student);
void inputStudent(Student* student);

#endif // STUDENT_H 