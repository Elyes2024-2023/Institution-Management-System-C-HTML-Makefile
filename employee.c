#include "employee.h"

void inputEmployee(Employee* emp) {
    printf("Enter Employee Details:\n");
    
    printf("Name: ");
    fgets(emp->base.name, 100, stdin);
    emp->base.name[strcspn(emp->base.name, "\n")] = 0;
    
    printf("Gender (M/F): ");
    fgets(emp->base.gender, 10, stdin);
    emp->base.gender[strcspn(emp->base.gender, "\n")] = 0;
    
    printf("Phone: ");
    fgets(emp->base.phone, 15, stdin);
    emp->base.phone[strcspn(emp->base.phone, "\n")] = 0;
    
    printf("Email: ");
    fgets(emp->base.email, 50, stdin);
    emp->base.email[strcspn(emp->base.email, "\n")] = 0;
    
    printf("Address: ");
    fgets(emp->base.address, 200, stdin);
    emp->base.address[strcspn(emp->base.address, "\n")] = 0;
    
    printf("Designation: ");
    fgets(emp->designation, 50, stdin);
    emp->designation[strcspn(emp->designation, "\n")] = 0;
    
    printf("Department: ");
    fgets(emp->department, 50, stdin);
    emp->department[strcspn(emp->department, "\n")] = 0;
    
    printf("Salary: ");
    scanf("%f", &emp->salary);
    while (getchar() != '\n'); // Clear input buffer
    
    printf("Join Date (YYYY-MM-DD): ");
    fgets(emp->join_date, 11, stdin);
    emp->join_date[strcspn(emp->join_date, "\n")] = 0;
}

int isValidEmployee(const Employee* emp) {
    if (strlen(emp->base.name) < 2) return 0;
    if (strlen(emp->base.gender) != 1 || (emp->base.gender[0] != 'M' && emp->base.gender[0] != 'F')) return 0;
    if (!isValidPhone(emp->base.phone)) return 0;
    if (!isValidEmail(emp->base.email)) return 0;
    if (strlen(emp->base.address) < 5) return 0;
    if (strlen(emp->designation) < 2) return 0;
    if (strlen(emp->department) < 2) return 0;
    if (emp->salary <= 0) return 0;
    if (!isValidDate(emp->join_date)) return 0;
    
    return 1;
}

void saveEmployee(FILE* fp, const Employee* emp) {
    fseek(fp, 0, SEEK_END);
    fwrite(emp, sizeof(Employee), 1, fp);
}

Employee* loadEmployee(FILE* fp, int id) {
    Employee* emp = malloc(sizeof(Employee));
    if (!emp) return NULL;
    
    fseek(fp, 0, SEEK_SET);
    while (fread(emp, sizeof(Employee), 1, fp) == 1) {
        if (emp->base.id == id) {
            return emp;
        }
    }
    
    free(emp);
    return NULL;
}

int getNextEmployeeId(FILE* fp) {
    Employee emp;
    int maxId = 0;
    
    fseek(fp, 0, SEEK_SET);
    while (fread(&emp, sizeof(Employee), 1, fp) == 1) {
        if (emp.base.id > maxId) {
            maxId = emp.base.id;
        }
    }
    
    return maxId + 1;
}

void addEmployee(FILE* fp) {
    Employee emp;
    emp.base.id = getNextEmployeeId(fp);
    inputEmployee(&emp);
    
    if (!isValidEmployee(&emp)) {
        printf("Invalid employee data!\n");
        return;
    }
    
    saveEmployee(fp, &emp);
    printf("Employee added successfully!\n");
}

void deleteEmployee(FILE* fp) {
    int id;
    printf("Enter Employee ID to delete: ");
    scanf("%d", &id);
    while (getchar() != '\n');
    
    Employee* emp = loadEmployee(fp, id);
    if (!emp) {
        printf("Employee not found!\n");
        return;
    }
    
    // Create temporary file
    FILE* temp = fopen("temp.dat", "wb+");
    if (!temp) {
        printf("Error creating temporary file!\n");
        free(emp);
        return;
    }
    
    // Copy all records except the one to delete
    Employee current;
    fseek(fp, 0, SEEK_SET);
    while (fread(&current, sizeof(Employee), 1, fp) == 1) {
        if (current.base.id != id) {
            fwrite(&current, sizeof(Employee), 1, temp);
        }
    }
    
    // Replace original file with temporary file
    fclose(fp);
    remove("employees.dat");
    rename("temp.dat", "employees.dat");
    fp = fopen("employees.dat", "rb+");
    
    printf("Employee deleted successfully!\n");
    free(emp);
}

void modifyEmployee(FILE* fp) {
    int id;
    printf("Enter Employee ID to modify: ");
    scanf("%d", &id);
    while (getchar() != '\n');
    
    Employee* emp = loadEmployee(fp, id);
    if (!emp) {
        printf("Employee not found!\n");
        return;
    }
    
    printf("Enter new details (press Enter to keep current value):\n");
    inputEmployee(emp);
    
    if (!isValidEmployee(emp)) {
        printf("Invalid employee data!\n");
        free(emp);
        return;
    }
    
    // Update the record
    fseek(fp, -(long)sizeof(Employee), SEEK_CUR);
    fwrite(emp, sizeof(Employee), 1, fp);
    
    printf("Employee modified successfully!\n");
    free(emp);
}

void displayEmployeeList(FILE* fp) {
    Employee emp;
    int found = 0;
    
    fseek(fp, 0, SEEK_SET);
    while (fread(&emp, sizeof(Employee), 1, fp) == 1) {
        printf("\nEmployee ID: %d\n", emp.base.id);
        printf("Name: %s\n", emp.base.name);
        printf("Gender: %s\n", emp.base.gender);
        printf("Phone: %s\n", emp.base.phone);
        printf("Email: %s\n", emp.base.email);
        printf("Address: %s\n", emp.base.address);
        printf("Designation: %s\n", emp.designation);
        printf("Department: %s\n", emp.department);
        printf("Salary: %.2f\n", emp.salary);
        printf("Join Date: %s\n", emp.join_date);
        printf("----------------------------------------\n");
        found = 1;
    }
    
    if (!found) {
        printf("No employees found!\n");
    }
}

void searchEmployee(FILE* fp) {
    char name[100];
    printf("Enter employee name to search: ");
    fgets(name, 100, stdin);
    name[strcspn(name, "\n")] = 0;
    
    Employee emp;
    int found = 0;
    
    fseek(fp, 0, SEEK_SET);
    while (fread(&emp, sizeof(Employee), 1, fp) == 1) {
        if (strstr(emp.base.name, name) != NULL) {
            printf("\nEmployee ID: %d\n", emp.base.id);
            printf("Name: %s\n", emp.base.name);
            printf("Gender: %s\n", emp.base.gender);
            printf("Phone: %s\n", emp.base.phone);
            printf("Email: %s\n", emp.base.email);
            printf("Address: %s\n", emp.base.address);
            printf("Designation: %s\n", emp.designation);
            printf("Department: %s\n", emp.department);
            printf("Salary: %.2f\n", emp.salary);
            printf("Join Date: %s\n", emp.join_date);
            printf("----------------------------------------\n");
            found = 1;
        }
    }
    
    if (!found) {
        printf("No employees found with that name!\n");
    }
}

void displayEmployeeByDepartment(FILE* fp, const char* department) {
    Employee emp;
    int found = 0;
    
    fseek(fp, 0, SEEK_SET);
    while (fread(&emp, sizeof(Employee), 1, fp) == 1) {
        if (strcmp(emp.department, department) == 0) {
            printf("\nEmployee ID: %d\n", emp.base.id);
            printf("Name: %s\n", emp.base.name);
            printf("Gender: %s\n", emp.base.gender);
            printf("Phone: %s\n", emp.base.phone);
            printf("Email: %s\n", emp.base.email);
            printf("Address: %s\n", emp.base.address);
            printf("Designation: %s\n", emp.designation);
            printf("Department: %s\n", emp.department);
            printf("Salary: %.2f\n", emp.salary);
            printf("Join Date: %s\n", emp.join_date);
            printf("----------------------------------------\n");
            found = 1;
        }
    }
    
    if (!found) {
        printf("No employees found in that department!\n");
    }
}

void displayEmployeeByGender(FILE* fp, const char* gender) {
    Employee emp;
    int found = 0;
    
    fseek(fp, 0, SEEK_SET);
    while (fread(&emp, sizeof(Employee), 1, fp) == 1) {
        if (strcmp(emp.base.gender, gender) == 0) {
            printf("\nEmployee ID: %d\n", emp.base.id);
            printf("Name: %s\n", emp.base.name);
            printf("Gender: %s\n", emp.base.gender);
            printf("Phone: %s\n", emp.base.phone);
            printf("Email: %s\n", emp.base.email);
            printf("Address: %s\n", emp.base.address);
            printf("Designation: %s\n", emp.designation);
            printf("Department: %s\n", emp.department);
            printf("Salary: %.2f\n", emp.salary);
            printf("Join Date: %s\n", emp.join_date);
            printf("----------------------------------------\n");
            found = 1;
        }
    }
    
    if (!found) {
        printf("No employees found with that gender!\n");
    }
} 