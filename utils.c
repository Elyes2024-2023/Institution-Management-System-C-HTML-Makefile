#include "management_system.h"

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void printHeader(const char* title) {
    clearScreen();
    printf("\n");
    printf("=================================================\n");
    printf("               %s\n", title);
    printf("=================================================\n\n");
}

void printMenu(const char* title, const char* options[], int numOptions) {
    printHeader(title);
    for (int i = 0; i < numOptions; i++) {
        printf("%d. %s\n", i + 1, options[i]);
    }
    printf("\nEnter your choice (1-%d): ", numOptions);
}

int getChoice(int maxOptions) {
    int choice;
    while (1) {
        if (scanf("%d", &choice) == 1 && choice >= 1 && choice <= maxOptions) {
            return choice;
        }
        printf("Invalid choice. Please enter a number between 1 and %d: ", maxOptions);
        while (getchar() != '\n'); // Clear input buffer
    }
}

FILE* openFile(const char* filename, const char* mode) {
    FILE* fp = fopen(filename, mode);
    if (!fp && strcmp(mode, "rb+") == 0) {
        // If file doesn't exist in read mode, try to create it
        fp = fopen(filename, "wb+");
    }
    return fp;
}

void closeFile(FILE* fp) {
    if (fp) {
        fclose(fp);
    }
}

int isValidEmail(const char* email) {
    int hasAt = 0;
    int hasDot = 0;
    int len = strlen(email);
    
    if (len < 5) return 0; // email@a.b
    
    for (int i = 0; i < len; i++) {
        if (email[i] == '@') hasAt = 1;
        if (email[i] == '.' && hasAt) hasDot = 1;
    }
    
    return hasAt && hasDot;
}

int isValidPhone(const char* phone) {
    int len = strlen(phone);
    if (len < 10 || len > 15) return 0;
    
    for (int i = 0; i < len; i++) {
        if (!isdigit(phone[i]) && phone[i] != '+' && phone[i] != '-') {
            return 0;
        }
    }
    
    return 1;
}

int isValidDate(const char* date) {
    // Format: YYYY-MM-DD
    if (strlen(date) != 10) return 0;
    
    int year, month, day;
    if (sscanf(date, "%d-%d-%d", &year, &month, &day) != 3) {
        return 0;
    }
    
    if (year < 1900 || year > 2100) return 0;
    if (month < 1 || month > 12) return 0;
    if (day < 1 || day > 31) return 0;
    
    // Simple month validation
    if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) {
        return 0;
    }
    
    // February validation
    if (month == 2) {
        int isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        if (day > (isLeap ? 29 : 28)) {
            return 0;
        }
    }
    
    return 1;
} 