#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define CACHE_SIZE 5
struct Student* cache_head = NULL;
#define MAX_NAME_LENGTH 30
#define MAX_HOSTEL_LENGTH 20
#define MAX_STUDENTS 100

struct Student {
    int id;
    char firstname[20];
    char lastname[20];
    char hostel[20];
    char course[10];
    int room_num;
    char dob[15];
    int year_of_study;

    struct Student* next;

}*head;

char first_names[20][20] = {"Shubham", "Om", "Ashutosh", "Sahil", "Jarvis", "Kushagra", "Ayush", "Malay", "Sid", "VeXe", "Siddhant", "Siddhartha", "Aman", "Aryan", "Aryaman", "Arya", "Aryabhatt", "Anushka", "Dipanshi", "Aryavartan"};
char last_names[20][20] = {"Gupta", "Buddhedev", "Sahu", "Sonawane", "Bruh", "Aggarwal", "Sharma", "Singh", "Kumar", "Chaudhary", "Jain", "Patil", "Mishra", "Patel", "Shah", "Verma", "Garg", "Sharma", "Singh", "Kumar"};
char hostels[5][5] = {"BH-1", "BH-2", "BH-3", "BH-4", "BH-5"};
char courses[5][10] = {"BTech", "MTech", "MS", "PhD", "MBA"};

// void First_names(char firstNames[][MAX_NAME_LENGTH], int count) {
//     FILE *file = fopen("first_names.txt", "w+");
//     if (file == NULL) {
//         printf("Error opening file: first_names.txt\n");
      
//     }

//     for (int i = 0; i < count; i++) {
//         fscanf(file, "%s", firstNames[i]);
//     }

//     fclose(file);
// }

// void Last_names(char lastNames[][MAX_NAME_LENGTH], int count) {
//     FILE *file = fopen("last_names.txt", "r");
//     if (file == NULL) {
//         printf("Error opening file: last_names.txt\n");
        
//     }

//     for (int i = 0; i < count; i++) {
//         fscanf(file, "%s", lastNames[i]);
//     }

//     fclose(file);
// }

struct Student* generate_random_student() {
    struct Student* member = (struct Student*)malloc(sizeof(struct Student));
    if (member == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    int new_id;
    do {
        new_id = rand() % 100 + 1;
        // Check if the generated id is already in use.
        struct Student* temp = head;
        while (temp != NULL) {
            if (temp->id == new_id) {
                new_id = 0; // Set to 0 to indicate the id is not unique.
                break;
            }
            temp = temp->next;
        }
    } while (new_id == 0);

    // char first_names[20][MAX_NAME_LENGTH];
    // char last_names[20][MAX_NAME_LENGTH];

    // First_names(first_names,20);
    // Last_names(last_names,20);

    member->id = new_id;
    strcpy(member->firstname, first_names[rand() % 20]);
    strcpy(member->lastname, last_names[rand() % 20]);
    strcpy(member->hostel, hostels[rand() % 5]);
    strcpy(member->course, courses[rand() % 5]);
    member->room_num = rand() % 100;
    sprintf(member->dob, "%02d-%02d-%04d", rand() % 29 + 1, rand() % 11 + 1, rand() % 10 + 1990);
    member->year_of_study = rand() % 10 + 1988;

    return member;
}

void insertInCache(struct Student* newStudent) {

    if (cache_head == NULL) {
        cache_head = newStudent;
    } 
    else {
        newStudent->next = cache_head;
        cache_head = newStudent;
    }
}

struct Student* searchInCache(int id) {

    struct Student* current = cache_head;
    while (current != NULL) {
        if (current->id == id)
            return current;
        current = current->next;
    }
    return NULL;
}

void removeFromCache(int id) {

    struct Student* current = cache_head;
    struct Student* prev = NULL;

    while (current != NULL) {
        if (current->id == id) {
            if (prev == NULL) {
                cache_head = current->next;
            } 
            else {
                prev->next = current->next;
            }
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}

void clearCache() {
    struct Student* current = cache_head;
    while (current != NULL) {
        struct Student* temp = current;
        current = current->next;
        free(temp);
    }
    cache_head = NULL;
}

void registerStudent() {

    FILE* file = fopen("disk.txt", "w+");
    if (file == NULL) {
        printf("Error opening file");
        return;
    } 
    else {
        printf("File created/opened successfully\n");
    }
    
    head = NULL;
    clearCache();

    for (int i = 1; i <= 100; i++) {
        struct Student* member = generate_random_student();

        member->next = head;
        head = member;
        insertInCache(member);

        fprintf(file, "%-12d%-12s%-12s%-12s%-12s%-12d%-12s%-12d\n", member->id, member->firstname, member->lastname,
                member->hostel, member->course, member->room_num, member->dob, member->year_of_study);
    }
    
    fclose(file);

    printf("Student data has been generated and saved to disk.txt.\n");
}

void searchStudent(int targetID) {
    
    struct Student* cachedStudent = searchInCache(targetID);
    if (cachedStudent != NULL) {
        printf("Student found in cache!\n");
        printf("ID: %d\n", cachedStudent->id);
        printf("FirstName: %s\n", cachedStudent->firstname);
        printf("LastName: %s\n", cachedStudent->lastname);
        printf("Hostel: %s\n", cachedStudent->hostel);
        printf("Course: %s\n", cachedStudent->course);
        printf("Room Number: %d\n", cachedStudent->room_num);
        printf("Date of Birth: %s\n", cachedStudent->dob);
        printf("Year of Study: %d\n", cachedStudent->year_of_study);
        return;
    } 
    else {
    struct Student* current = head;
    while (current != NULL) {
        if (current->id == targetID) {
            printf("Student found!\n");
            printf("ID: %d\n", current->id);
            printf("FirstName: %s\n", current->firstname);
            printf("LastName: %s\n", current->lastname);
            printf("Hostel: %s\n", current->hostel);
            printf("Course: %s\n", current->course);
            printf("Room Number: %d\n", current->room_num);
            printf("Date of Birth: %s\n", current->dob);
            printf("Year of Study: %d\n", current->year_of_study);
            return;
        }
        current = current->next;
      }
    }
    printf("Student with ID %d not found.\n", targetID);
}

void updateStudent(int targetID,int new_room_num) {

    struct Student* cachedStudent = searchInCache(targetID);
    if (cachedStudent != NULL) {
        cachedStudent->room_num = new_room_num;
    } 
    else {
        struct Student* current = head;
        while (current != NULL) {
        if (current->id == targetID) {
    
            current->room_num = new_room_num;

            FILE* file = fopen("disk.txt", "w");
            if (file == NULL) {
                printf("Error opening file");
                return;
            }

            struct Student* temp = head;
            while (temp != NULL) {
                fprintf(file, "%d\t%s\t%s\t%s\t%s\t%d\t%s\t%d\n", temp->id, temp->firstname, temp->lastname,
                        temp->hostel, temp->course, temp->room_num, temp->dob, temp->year_of_study);
                temp = temp->next;
            }

            fclose(file);

            printf("Student information updated!\n");
            return;
        }
        current = current->next;
      }
    }
    struct Student* current = head;
    while (current != NULL) {
        if (current->id == targetID) {
            current->room_num = cachedStudent->room_num;

                FILE* file = fopen("disk.txt" , "w");
            if (file == NULL) {
            printf("Error opening file");
            return;
        }
        struct Student* current = head;
        while (current != NULL) {
        fprintf(file, "%d\t%s\t%s\t%s\t%s\t%d\t%s\t%d\n", current->id, current->firstname, current->lastname,
                current->hostel, current->course, current->room_num, current->dob, current->year_of_study);
        current = current->next;
        }

            fclose(file);

            printf("Student information updated!\n");
            return;
        }
        current = current->next;
    }

    printf("Student with ID %d not found.\n", targetID);
}

void deleteStudent(int targetID) {

    struct Student* current = head;
    struct Student* prev = NULL;

    removeFromCache(targetID);

    while (current != NULL) {
        if (current->id == targetID) {
            if (prev == NULL) {
                head = current->next;
            } 
            else {
                prev->next = current->next;
            }

            FILE* file = fopen("disk.txt", "w");
            if (file == NULL) {
                printf("Error opening file");
                return;
            }

            struct Student* temp = head;
            while (temp != NULL) {
                fprintf(file, "%d\t%s\t%s\t%s\t%s\t%d\t%s\t%d\n", temp->id, temp->firstname, temp->lastname,
                        temp->hostel, temp->course, temp->room_num, temp->dob, temp->year_of_study);
                temp = temp->next;
            }

            fclose(file); 
            free(current);
            printf("Student with ID %d deleted.\n", targetID);
            return;
        }
        prev = current;
        current = current->next;
    }

    printf("Student with ID %d not found.\n", targetID);
}

void server(int a,int b,int c){

    int op_id = a;
    int student_ID = b;
    int student_room_num = c;

    while (1) {
        if (op_id == 0) {
            registerStudent();
            return;
        } 
        else if (op_id == 1) {
            searchStudent(student_ID);
            return;
        } 
        else if (op_id == 2) {
            updateStudent(student_ID,student_room_num);
            return;
        } 
        else if (op_id == 3) {
            deleteStudent(student_ID);
            return;
        } 
    } 
}

int main() {

    FILE *queueFile = fopen("operation_queue.txt", "r");
    if (queueFile == NULL) {
        printf("Error opening file");
        return 1;
    }
    else {
        printf("File created successfully\n");
    }
    
    char operation[10];
    int studentId, operationId,updated_room_num, hours, minutes, seconds;

    while (fscanf(queueFile, "%s %d %d %d %d:%d:%d", operation, &studentId, &operationId , &updated_room_num, &hours, &minutes, &seconds) == 7) {
        server(operationId , studentId , updated_room_num);
    }    
    
    clearCache();

    fclose(queueFile);

    return 0;
}