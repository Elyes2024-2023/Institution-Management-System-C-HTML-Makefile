/*
 * Copyright (c) 2024-2025 ELYES. All rights reserved.
 * 
 * This file is part of the ELYES Employee and Student Management System Library.
 * Created by ELYES
 */

#ifndef MANAGEMENT_LIB_H
#define MANAGEMENT_LIB_H

#ifdef __cplusplus
extern "C" {
#endif

// Version information
#define MANAGEMENT_LIB_VERSION "1.0.0"
#define MANAGEMENT_LIB_VERSION_MAJOR 1
#define MANAGEMENT_LIB_VERSION_MINOR 0
#define MANAGEMENT_LIB_VERSION_PATCH 0
#define LIBRARY_NAME "ELYES Management System"

// Export functions with proper calling convention
#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

// Student management functions - Done by ELYES
EXPORT int add_student(const char* name, const char* gender, const char* phone, 
                      const char* email, const char* address, const char* class_name,
                      int age, float gpa, const char* enrollment_date, int attendance);
EXPORT int delete_student(int roll_number);
EXPORT int modify_student(int roll_number, const char* name, const char* gender, 
                         const char* phone, const char* email, const char* address, 
                         const char* class_name, int age, float gpa, 
                         const char* enrollment_date, int attendance);
EXPORT int get_student_count();
EXPORT int get_student_by_index(int index, char* name, char* gender, char* phone, 
                               char* email, char* address, char* class_name, 
                               int* age, float* gpa, char* enrollment_date, 
                               int* attendance, int* roll_number);
EXPORT int search_student_by_name(const char* name, int* results, int max_results);
EXPORT int get_student_by_roll(int roll_number, char* name, char* gender, char* phone, 
                              char* email, char* address, char* class_name, 
                              int* age, float* gpa, char* enrollment_date, 
                              int* attendance);

// Employee management functions - Done by ELYES
EXPORT int add_employee(const char* name, const char* gender, const char* phone, 
                       const char* email, const char* address, const char* designation,
                       float salary, const char* department, const char* join_date);
EXPORT int delete_employee(int id);
EXPORT int modify_employee(int id, const char* name, const char* gender, const char* phone, 
                          const char* email, const char* address, const char* designation,
                          float salary, const char* department, const char* join_date);
EXPORT int get_employee_count();
EXPORT int get_employee_by_index(int index, char* name, char* gender, char* phone, 
                                char* email, char* address, char* designation,
                                float* salary, char* department, char* join_date, 
                                int* id);
EXPORT int search_employee_by_name(const char* name, int* results, int max_results);
EXPORT int get_employee_by_id(int id, char* name, char* gender, char* phone, 
                             char* email, char* address, char* designation,
                             float* salary, char* department, char* join_date);

// Utility functions - Done by ELYES
EXPORT const char* get_last_error();
EXPORT void cleanup();
EXPORT const char* get_version();

#ifdef __cplusplus
}
#endif

#endif // MANAGEMENT_LIB_H 