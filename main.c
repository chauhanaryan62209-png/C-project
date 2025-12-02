#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "students.dat"

typedef struct {
    int id;
    char name[30];
    float totalFee;
    float paidFee;   // how much student has paid so far
} Student;

// Function declarations
void addStudent();
void viewAllStudents();
void searchStudent();
void payFee();

int main() {
    int choice;

    do {
        printf("\n===== Student Fee Management System =====\n");
        printf("1. Add New Student\n");
        printf("2. View All Students\n");
        printf("3. Search Student by ID\n");
        printf("4. Pay Fee / Update Payment\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addStudent();
                break;
            case 2:
                viewAllStudents();
                break;
            case 3:
                searchStudent();
                break;
            case 4:
                payFee();
                break;
            case 0:
                printf("Exiting... Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }

    } while (choice != 0);

    return 0;
}

// Add a new student record
void addStudent() {
    FILE *fp = fopen(FILE_NAME, "ab");  // append in binary mode
    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    Student s;

    printf("\n--- Add New Student ---\n");
    printf("Enter Student ID: ");
    scanf("%d", &s.id);

    printf("Enter Name (single word): ");
    scanf("%s", s.name);

    printf("Enter Total Fee: ");
    scanf("%f", &s.totalFee);

    s.paidFee = 0;  // initially nothing paid

    fwrite(&s, sizeof(Student), 1, fp);
    fclose(fp);

    printf("Student added successfully!\n");
}

// View all student records
void viewAllStudents() {
    FILE *fp = fopen(FILE_NAME, "rb");
    if (fp == NULL) {
        printf("No student records found.\n");
        return;
    }

    Student s;
    int count = 0;

    printf("\n--- All Students ---\n");
    printf("%-5s %-15s %-10s %-10s %-10s\n",
           "ID", "Name", "TotalFee", "Paid", "Due");
    printf("-------------------------------------------------\n");

    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        float due = s.totalFee - s.paidFee;
        if (due < 0) due = 0; // just in case

        printf("%-5d %-15s %-10.2f %-10.2f %-10.2f\n",
               s.id, s.name, s.totalFee, s.paidFee, due);

        count++;
    }

    if (count == 0) {
        printf("No records to display.\n");
    }

    fclose(fp);
}

// Search a student by ID and display details
void searchStudent() {
    FILE *fp = fopen(FILE_NAME, "rb");
    if (fp == NULL) {
        printf("No student records found.\n");
        return;
    }

    int id, found = 0;
    Student s;

    printf("\nEnter Student ID to search: ");
    scanf("%d", &id);

    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        if (s.id == id) {
            float due = s.totalFee - s.paidFee;
            if (due < 0) due = 0;

            printf("\n--- Student Details ---\n");
            printf("ID       : %d\n", s.id);
            printf("Name     : %s\n", s.name);
            printf("TotalFee : %.2f\n", s.totalFee);
            printf("PaidFee  : %.2f\n", s.paidFee);
            printf("Due      : %.2f\n", due);

            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Student with ID %d not found.\n", id);
    }

    fclose(fp);
}

// Pay fee / update paid amount for a student
void payFee() {
    FILE *fp = fopen(FILE_NAME, "rb+");  // read + write mode
    if (fp == NULL) {
        printf("No student records found.\n");
        return;
    }

    int id, found = 0;
    float amount;
    Student s;

    printf("\nEnter Student ID to pay fee: ");
    scanf("%d", &id);

    // search for the student
    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        if (s.id == id) {
            found = 1;
            printf("\nCurrent Details:\n");
            float dueBefore = s.totalFee - s.paidFee;
            if (dueBefore < 0) dueBefore = 0;

            printf("Name     : %s\n", s.name);
            printf("TotalFee : %.2f\n", s.totalFee);
            printf("PaidFee  : %.2f\n", s.paidFee);
            printf("Due      : %.2f\n", dueBefore);

            printf("\nEnter amount to pay: ");
            scanf("%f", &amount);

            s.paidFee += amount;
            float dueAfter = s.totalFee - s.paidFee;
            if (dueAfter < 0) dueAfter = 0;

            // move file pointer back to overwrite this record
            fseek(fp, -sizeof(Student), SEEK_CUR);
            fwrite(&s, sizeof(Student), 1, fp);

            printf("\n--- Payment Receipt ---\n");
            printf("Student ID : %d\n", s.id);
            printf("Name       : %s\n", s.name);
            printf("Paid Now   : %.2f\n", amount);
            printf("Total Paid : %.2f\n", s.paidFee);
            printf("Remaining  : %.2f\n", dueAfter);

            break;
        }
    }

    if (!found) {
        printf("Student with ID %d not found.\n", id);
    }

    fclose(fp);
}
