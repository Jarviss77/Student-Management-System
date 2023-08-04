#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_STUDENTS 100

typedef struct {
    char first_name[20];
    char last_name[20];
    char hostel[5];
    char course[10];
    int room_number;
    char date_of_birth[11];
    int year_of_study;
} Student;

char first_names[20][20] = {"Shubham", "Om", "Ashutosh", "Sahil", "Jarvis", "Kushagra", "Ayush", "Malay", "Sid", "Siddharth", "Siddhant", "Siddhartha", "Aman", "Aryan", "Aryaman", "Arya", "Aryabhatt", "Aryavarta", "Aryavart", "Aryavartan"};
char last_names[20][20] = {"Gupta", "Buddhedev", "Sahu", "Sonawane", "Bruh", "Aggarwal", "Sharma", "Singh", "Kumar", "Chaudhary", "Jain", "Jha", "Mishra", "Patel", "Shah", "Verma", "Garg", "Sharma", "Singh", "Kumar"};
char hostels[5][5] = {"BH-1", "BH-2", "BH-3", "BH-4", "BH-5"};
char courses[5][10] = {"BTech", "MTech", "MS", "PhD", "MBA"};

Student generate_random_student(int student_id) {
    Student student;
    strcpy(student.first_name, first_names[rand() % 20]);
    strcpy(student.last_name, last_names[rand() % 20]);
    strcpy(student.hostel, hostels[rand() % 5]);
    strcpy(student.course, courses[rand() % 5]);
    student.room_number = rand() % 200 + 100; 
    sprintf(student.date_of_birth, "%02d-%02d-%04d", rand() % 12 + 1, rand() % 28 + 1, rand() % 10 + 1990);
    student.year_of_study = rand() % 5 + 1; 
    return student;
}

int main() {
    srand(time(NULL));


    FILE *file = fopen("disk.txt", "w");
    if (file == NULL) {
        printf("Error opening file.\n");
        return 1;
    }


    fprintf(file, "First Name  Last Name   Hostel  Course  Room Number  Date of Birth  Year of Study\n");
    for (int i = 1; i <= MAX_STUDENTS; i++) {
        Student student = generate_random_student(i);
        fprintf(file, "%-12s%-12s%-7s%-8s%-13d%-15s%d\n", student.first_name, student.last_name, student.hostel,
 student.course, student.room_number, student.date_of_birth, student.year_of_study);
    }

    fclose(file);

    printf("Student data has been generated and saved to disk.txt.\n");

    return 0;
}
