#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include "management_system.h"

// Employee management functions
void addEmployee(FILE* fp);
void deleteEmployee(FILE* fp);
void modifyEmployee(FILE* fp);
void displayEmployeeList(FILE* fp);
void searchEmployee(FILE* fp);
void displayEmployeeByDepartment(FILE* fp, const char* department);
void displayEmployeeByGender(FILE* fp, const char* gender);

// Employee file operations
void saveEmployee(FILE* fp, const Employee* emp);
Employee* loadEmployee(FILE* fp, int id);
int getNextEmployeeId(FILE* fp);

// Employee validation
int isValidEmployee(const Employee* emp);
void inputEmployee(Employee* emp);

#endif // EMPLOYEE_H 