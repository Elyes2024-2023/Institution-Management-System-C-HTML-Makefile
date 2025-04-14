#include "student.h"

void inputStudent(Student* student) {
    printf("Enter Student Details:\n");
    
    printf("Name: ");
    fgets(student->base.name, 100, stdin);
    student->base.name[strcspn(student->base.name, "\n")] = 0;
    
    printf("Gender (M/F): ");
    fgets(student->base.gender, 10, stdin);
    student->base.gender[strcspn(student->base.gender, "\n")] = 0;
    
    printf("Phone: ");
    fgets(student->base.phone, 15, stdin);
    student->base.phone[strcspn(student->base.phone, "\n")] = 0;
    
    printf("Email: ");
    fgets(student->base.email, 50, stdin);
    student->base.email[strcspn(student->base.email, "\n")] = 0;
    
    printf("Address: ");
    fgets(student->base.address, 200, stdin);
    student->base.address[strcspn(student->base.address, "\n")] = 0;
    
    printf("Class: ");
    fgets(student->class, 20, stdin);
    student->class[strcspn(student->class, "\n")] = 0;
    
    printf("Age: ");
    scanf("%d", &student->age);
    while (getchar() != '\n');
    
    printf("GPA: ");
    scanf("%f", &student->gpa);
    while (getchar() != '\n');
    
    printf("Enrollment Date (YYYY-MM-DD): ");
    fgets(student->enrollment_date, 11, stdin);
    student->enrollment_date[strcspn(student->enrollment_date, "\n")] = 0;
    
    printf("Attendance (days present): ");
    scanf("%d", &student->attendance);
    while (getchar() != '\n');
}

int isValidStudent(const Student* student) {
    if (strlen(student->base.name) < 2) return 0;
    if (strlen(student->base.gender) != 1 || (student->base.gender[0] != 'M' && student->base.gender[0] != 'F')) return 0;
    if (!isValidPhone(student->base.phone)) return 0;
    if (!isValidEmail(student->base.email)) return 0;
    if (strlen(student->base.address) < 5) return 0;
    if (strlen(student->class) < 2) return 0;
    if (student->age < 5 || student->age > 100) return 0;
    if (student->gpa < 0 || student->gpa > 4.0) return 0;
    if (!isValidDate(student->enrollment_date)) return 0;
    if (student->attendance < 0) return 0;
    
    return 1;
}

void saveStudent(FILE* fp, const Student* student) {
    fseek(fp, 0, SEEK_END);
    fwrite(student, sizeof(Student), 1, fp);
}

Student* loadStudent(FILE* fp, int rollNumber) {
    Student* student = malloc(sizeof(Student));
    if (!student) return NULL;
    
    fseek(fp, 0, SEEK_SET);
    while (fread(student, sizeof(Student), 1, fp) == 1) {
        if (student->roll_number == rollNumber) {
            return student;
        }
    }
    
    free(student);
    return NULL;
}

int getNextRollNumber(FILE* fp) {
    Student student;
    int maxRoll = 0;
    
    fseek(fp, 0, SEEK_SET);
    while (fread(&student, sizeof(Student), 1, fp) == 1) {
        if (student.roll_number > maxRoll) {
            maxRoll = student.roll_number;
        }
    }
    
    return maxRoll + 1;
}

void addStudent(FILE* fp) {
    Student student;
    student.roll_number = getNextRollNumber(fp);
    inputStudent(&student);
    
    if (!isValidStudent(&student)) {
        printf("Invalid student data!\n");
        return;
    }
    
    saveStudent(fp, &student);
    printf("Student added successfully!\n");
}

void deleteStudent(FILE* fp) {
    int rollNumber;
    printf("Enter Roll Number to delete: ");
    scanf("%d", &rollNumber);
    while (getchar() != '\n');
    
    Student* student = loadStudent(fp, rollNumber);
    if (!student) {
        printf("Student not found!\n");
        return;
    }
    
    // Create temporary file
    FILE* temp = fopen("temp.dat", "wb+");
    if (!temp) {
        printf("Error creating temporary file!\n");
        free(student);
        return;
    }
    
    // Copy all records except the one to delete
    Student current;
    fseek(fp, 0, SEEK_SET);
    while (fread(&current, sizeof(Student), 1, fp) == 1) {
        if (current.roll_number != rollNumber) {
            fwrite(&current, sizeof(Student), 1, temp);
        }
    }
    
    // Replace original file with temporary file
    fclose(fp);
    remove("students.dat");
    rename("temp.dat", "students.dat");
    fp = fopen("students.dat", "rb+");
    
    printf("Student deleted successfully!\n");
    free(student);
}

void modifyStudent(FILE* fp) {
    int rollNumber;
    printf("Enter Roll Number to modify: ");
    scanf("%d", &rollNumber);
    while (getchar() != '\n');
    
    Student* student = loadStudent(fp, rollNumber);
    if (!student) {
        printf("Student not found!\n");
        return;
    }
    
    printf("Enter new details (press Enter to keep current value):\n");
    inputStudent(student);
    
    if (!isValidStudent(student)) {
        printf("Invalid student data!\n");
        free(student);
        return;
    }
    
    // Update the record
    fseek(fp, -(long)sizeof(Student), SEEK_CUR);
    fwrite(student, sizeof(Student), 1, fp);
    
    printf("Student modified successfully!\n");
    free(student);
}

void displayStudentList(FILE* fp) {
    Student student;
    int found = 0;
    
    fseek(fp, 0, SEEK_SET);
    while (fread(&student, sizeof(Student), 1, fp) == 1) {
        printf("\nRoll Number: %d\n", student.roll_number);
        printf("Name: %s\n", student.base.name);
        printf("Gender: %s\n", student.base.gender);
        printf("Phone: %s\n", student.base.phone);
        printf("Email: %s\n", student.base.email);
        printf("Address: %s\n", student.base.address);
        printf("Class: %s\n", student.class);
        printf("Age: %d\n", student.age);
        printf("GPA: %.2f\n", student.gpa);
        printf("Enrollment Date: %s\n", student.enrollment_date);
        printf("Attendance: %d days\n", student.attendance);
        printf("----------------------------------------\n");
        found = 1;
    }
    
    if (!found) {
        printf("No students found!\n");
    }
}

void searchStudent(FILE* fp) {
    char name[100];
    printf("Enter student name to search: ");
    fgets(name, 100, stdin);
    name[strcspn(name, "\n")] = 0;
    
    Student student;
    int found = 0;
    
    fseek(fp, 0, SEEK_SET);
    while (fread(&student, sizeof(Student), 1, fp) == 1) {
        if (strstr(student.base.name, name) != NULL) {
            printf("\nRoll Number: %d\n", student.roll_number);
            printf("Name: %s\n", student.base.name);
            printf("Gender: %s\n", student.base.gender);
            printf("Phone: %s\n", student.base.phone);
            printf("Email: %s\n", student.base.email);
            printf("Address: %s\n", student.base.address);
            printf("Class: %s\n", student.class);
            printf("Age: %d\n", student.age);
            printf("GPA: %.2f\n", student.gpa);
            printf("Enrollment Date: %s\n", student.enrollment_date);
            printf("Attendance: %d days\n", student.attendance);
            printf("----------------------------------------\n");
            found = 1;
        }
    }
    
    if (!found) {
        printf("No students found with that name!\n");
    }
}

void displayStudentByClass(FILE* fp, const char* class) {
    Student student;
    int found = 0;
    
    fseek(fp, 0, SEEK_SET);
    while (fread(&student, sizeof(Student), 1, fp) == 1) {
        if (strcmp(student.class, class) == 0) {
            printf("\nRoll Number: %d\n", student.roll_number);
            printf("Name: %s\n", student.base.name);
            printf("Gender: %s\n", student.base.gender);
            printf("Phone: %s\n", student.base.phone);
            printf("Email: %s\n", student.base.email);
            printf("Address: %s\n", student.base.address);
            printf("Class: %s\n", student.class);
            printf("Age: %d\n", student.age);
            printf("GPA: %.2f\n", student.gpa);
            printf("Enrollment Date: %s\n", student.enrollment_date);
            printf("Attendance: %d days\n", student.attendance);
            printf("----------------------------------------\n");
            found = 1;
        }
    }
    
    if (!found) {
        printf("No students found in that class!\n");
    }
}

void updateAttendance(FILE* fp, int rollNumber, int present) {
    Student* student = loadStudent(fp, rollNumber);
    if (!student) {
        printf("Student not found!\n");
        return;
    }
    
    student->attendance = present;
    
    // Update the record
    fseek(fp, -(long)sizeof(Student), SEEK_CUR);
    fwrite(student, sizeof(Student), 1, fp);
    
    printf("Attendance updated successfully!\n");
    free(student);
}

void updateGrades(FILE* fp, int rollNumber, float gpa) {
    Student* student = loadStudent(fp, rollNumber);
    if (!student) {
        printf("Student not found!\n");
        return;
    }
    
    if (gpa < 0 || gpa > 4.0) {
        printf("Invalid GPA value!\n");
        free(student);
        return;
    }
    
    student->gpa = gpa;
    
    // Update the record
    fseek(fp, -(long)sizeof(Student), SEEK_CUR);
    fwrite(student, sizeof(Student), 1, fp);
    
    printf("Grades updated successfully!\n");
    free(student);
} 