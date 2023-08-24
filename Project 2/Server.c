#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MEM_SIZE 5
struct Student* mem_head = NULL;
#define MAX_NAME_LENGTH 30
#define MAX_HOSTEL_LENGTH 20
#define MAX_STUDENTS 100


/*----------------------------------
    CREATING "STUDENT" STRUCT
------------------------------------*/
struct Student {
    int id;
    char firstname[20];
    char lastname[20];
    char hostel[20];
    char course[10];
    int room;
    char dob[15];
    int year_of_study;

    struct Student* next;

}*head;

char hostels[5][5] = {"BH-1", "BH-2", "BH-3", "BH-4", "BH-5"};
char courses[5][10] = {"BTech", "MTech", "MS", "PhD", "MBA"};



/*---------------------------
    FUNCTION TO COUNT NUMBER OF NODES
---------------------------------*/
int ctNodes(){
    struct Student* cur = mem_head;
    int ct=0;
    while (cur != NULL) {
        cur = cur->next;
        ct++;
    }
    return ct;
}


/*-----------------------------------------
    GENERATES RANDOM FIRST NAMES
--------------------------------------------*/
void gen_firstName(char firstNames[][MAX_NAME_LENGTH], int ct) {
    FILE *file = fopen("first_names.txt", "r");
    if (file == NULL) {
        printf("Error opening file: first_names.txt\n");
      
    }

    for (int i = 0; i < ct; i++) {
        fscanf(file, "%s", firstNames[i]);
    }

    fclose(file);
}

/*-----------------------------------------
    GENERATES RANDOM LAST NAMES
--------------------------------------------*/
void gen_lastName(char lastNames[][MAX_NAME_LENGTH], int ct) {
    FILE *file = fopen("last_names.txt", "r");
    if (file == NULL) {
        printf("Error opening file: last_names.txt\n");
        
    }

    for (int i = 0; i < ct; i++) {
        fscanf(file, "%s", lastNames[i]);
    }

    fclose(file);
}


/*-----------------------------------------
    CREATES A NEW STUDENT
--------------------------------------------*/
struct Student* generate_new() {
    struct Student* Student = (struct Student*)malloc(sizeof(struct Student));
    if (Student == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    srand(0);
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

    char first_names[20][MAX_NAME_LENGTH];
    char last_names[20][MAX_NAME_LENGTH];

    // GENERATES FIRST AND LAST NAMES
    gen_firstName(first_names,20);
    gen_lastName(last_names,20);


    // GENERATES RANDOM INFO OF STUDENT
    Student->id = new_id;
    strcpy(Student->firstname, first_names[rand() % 20]);
    strcpy(Student->lastname, last_names[rand() % 20]);
    strcpy(Student->hostel, hostels[rand() % 5]);
    strcpy(Student->course, courses[rand() % 5]);
    Student->room = rand() % 100;
    sprintf(Student->dob, "%02d-%02d-%04d", rand() % 29 + 1, rand() % 11 + 1, rand() % 10 + 1990);
    Student->year_of_study = rand() % 10 + 1988;

    return Student;
}


/*-----------------------------------------
    INSERTS NEW STUDENTS IN MAIN MEMORY
--------------------------------------------*/
void insert(struct Student* newStu) {

    if (mem_head == NULL) {
        mem_head = newStu;
    } 
    else {
        newStu->next = mem_head;
        mem_head = newStu;
    }
}


/*-----------------------------------------
    SEARCHES GIVEN STUDENT IN MEMORY
--------------------------------------------*/
struct Student* searchInCache(int id) {

    struct Student* cur = mem_head;
    while (cur != NULL) {
        if (cur->id == id)
            return cur;
        cur = cur->next;
    }
    return NULL;
}


/*-----------------------------------------
    DELETES THE LAST STUDENT FROM MEMORY TO FREE SPACE
--------------------------------------------*/
void delete_last(){
            struct Student* temp = mem_head;
            struct Student* prev;

            // TRAVERSES TO THE LAST NODE
            while(temp->next!=NULL){
                prev = temp;
                temp = temp->next;
            }
            if(temp == head) head=NULL;

            // FREE THE LAST NODE
            else prev->next=NULL;
            
    free(temp);
}


/*-----------------------------------------
    DELETES A GIVEN STUDENT FROM MEMEORY
--------------------------------------------*/
void delete(int id) {

    struct Student* cur = mem_head;
    struct Student* prev = NULL;

    while (cur != NULL) {

        // SEARCH THE MEMEORY FOR GIVEN STUDNT ID
        if (cur->id == id) {
            if (prev == NULL) {
                mem_head = cur->next;
            } 
            else {

                //FREE THE CURRENT NODE
                prev->next = cur->next;
            }
            free(cur);
            return;
        }
        prev = cur;
        cur = cur->next;
    }
}

/*-----------------------------------------
    DELETES THE WHOLE MEMEMORY
--------------------------------------------*/
void clear_mem() {
    struct Student* cur = mem_head;
    while (cur != NULL) {
        struct Student* temp = cur;
        cur = cur->next;
        free(temp);
    }
    mem_head = NULL;
}


/*-----------------------------------------
    REGUISTERS A NEW STUDENT
--------------------------------------------*/
void register_stu(){

    FILE* file = fopen("disk.txt", "w+");
    if (file == NULL) {
        printf("Error opening file disk.txt");
        return;
    } 
    else {
        printf("File created\n");
    }
    
    head = NULL;
    clear_mem();

    for (int i = 1; i <= 100; i++) {
        struct Student* Student = generate_new();

        Student->next = head;
        head = Student;
        if(i>=96 && i<=100){
            int numCache = ctNodes();
            if(numCache>5){
               delete_last();
            }
            else{
               insert(Student);
            }
        }

        fprintf(file, "%d\t%s\t%s\t%s\t%s\t%d\t%s\t%d\n", Student->id, Student->firstname, Student->lastname,
                Student->hostel, Student->course, Student->room, Student->dob, Student->year_of_study);
    }
    
    fclose(file);

    printf("Student data has been generated and saved to disk.txt.\n");
}


/*-----------------------------------------
    SEARCHES IF GIVEN STUDENT EXISTS IN MEMORY
--------------------------------------------*/
void search(int targetID) {
    
    struct Student* cachedStudent = searchInCache(targetID);
    if (cachedStudent != NULL) {
        printf("Student found in cache!\n");
        printf("ID: %d\n", cachedStudent->id);
        printf("FirstName: %s\n", cachedStudent->firstname);
        printf("LastName: %s\n", cachedStudent->lastname);
        printf("Hostel: %s\n", cachedStudent->hostel);
        printf("Course: %s\n", cachedStudent->course);
        printf("Room Number: %d\n", cachedStudent->room);
        printf("Date of Birth: %s\n", cachedStudent->dob);
        printf("Year of Study: %d\n", cachedStudent->year_of_study);
        return;
    } 
    else {
    struct Student* cur = head;
    while (cur != NULL) {
        if (cur->id == targetID) {
            printf("Student found!\n");
            printf("ID: %d\n", cur->id);
            printf("FirstName: %s\n", cur->firstname);
            printf("LastName: %s\n", cur->lastname);
            printf("Hostel: %s\n", cur->hostel);
            printf("Course: %s\n", cur->course);
            printf("Room Number: %d\n", cur->room);
            printf("Date of Birth: %s\n", cur->dob);
            printf("Year of Study: %d\n", cur->year_of_study);
            return;
        }
        cur = cur->next;
      }
    }
    printf("Student with ID %d not found.\n", targetID);
}


/*-----------------------------------------
    UPDQATWS THE ROOM NUMBER OF STUDENT
--------------------------------------------*/
void update(int targetID,int new_room) {

    struct Student* cachedStudent = searchInCache(targetID);
    if (cachedStudent != NULL) {
        cachedStudent->room = new_room;
    } 
    else {
        struct Student* cur = head;
        while (cur != NULL) {
        if (cur->id == targetID) {
    
            cur->room = new_room;

            FILE* file = fopen("disk.txt", "w");
            if (file == NULL) {
                printf("Error opening file");
                return;
            }

            struct Student* temp = head;
            while (temp != NULL) {
                fprintf(file, "%d\t%s\t%s\t%s\t%s\t%d\t%s\t%d\n", temp->id, temp->firstname, temp->lastname,
                        temp->hostel, temp->course, temp->room, temp->dob, temp->year_of_study);
                temp = temp->next;
            }

            fclose(file);

            printf("Student information updated!\n");
            return;
        }
        cur = cur->next;
      }
    }
    struct Student* cur = head;
    while (cur != NULL) {
        if (cur->id == targetID) {
            cur->room = cachedStudent->room;

                FILE* file = fopen("disk.txt" , "w");
            if (file == NULL) {
            printf("Error opening file");
            return;
        }
        struct Student* cur = head;
        while (cur != NULL) {
        fprintf(file, "%d\t%s\t%s\t%s\t%s\t%d\t%s\t%d\n", cur->id, cur->firstname, cur->lastname,
                cur->hostel, cur->course, cur->room, cur->dob, cur->year_of_study);
        cur = cur->next;
        }

            fclose(file);

            printf("Student information updated!\n");
            return;
        }
        cur = cur->next;
    }

    printf("Student with ID %d not found.\n", targetID);
}

/*-----------------------------------------
    DELETES THE STUDENT 
--------------------------------------------*/
void deleteStudent(int targetID) {

    struct Student* cur = head;
    struct Student* prev = NULL;

    delete(targetID);

    while (cur != NULL) {
        if (cur->id == targetID) {
            if (prev == NULL) {
                head = cur->next;
            } 
            else {
                prev->next = cur->next;
            }

            FILE* file = fopen("disk.txt", "w");
            if (file == NULL) {
                printf("Error opening file");
                return;
            }

            struct Student* temp = head;
            while (temp != NULL) {
                fprintf(file, "%d\t%s\t%s\t%s\t%s\t%d\t%s\t%d\n", temp->id, temp->firstname, temp->lastname,
                        temp->hostel, temp->course, temp->room, temp->dob, temp->year_of_study);
                temp = temp->next;
            }

            fclose(file); 
            free(cur);
            printf("Student with ID %d deleted.\n", targetID);
            return;
        }
        prev = cur;
        cur = cur->next;
    }

    printf("Student with ID %d not found.\n", targetID);
}


/*-----------------------------------------
    STARTS THE MAIN SERVER FUNCITON
--------------------------------------------*/
void server(int a,int b,int c){

    int op_id = a;
    int student_ID = b;
    int student_room = c;

    while (1) {
        if (op_id == 0) {
            register_stu();
            return;
        } 
        else if (op_id == 1) {
            search(student_ID);
            return;
        } 
        else if (op_id == 2) {
            update(student_ID,student_room);
            return;
        } 
        else if (op_id == 3) {
            deleteStudent(student_ID);
            return;
        } 
    } 
}

int main() {
    FILE *queueFile = fopen("operation_queue.txt", "r+");
    if (queueFile == NULL) {
        printf("Error opening file op_queue");
        return 1;
    }
    else {
        printf("File created\n");
    }

    printf("Starting 'server'\n");
    
    char operation[10];
    int studentId, operationId,updated_room, hours, minutes, seconds;

    int requestsProcessed = 0;
    clock_t start = clock(); 

    while (fscanf(queueFile, "%s %d %d %d %d:%d:%d", operation, &studentId, &operationId , &updated_room, &hours, &minutes, &seconds) == 7) {
        server(operationId , studentId , updated_room);
        requestsProcessed++;
    }    
    
    clear_mem();

    clock_t end = clock();
    double timeTaken = (double)(end - start) / CLOCKS_PER_SEC;

    fclose(queueFile);
     
    if (timeTaken > 0) {
        double requestsPerSecond = requestsProcessed / timeTaken;
        printf("Processed %d requests in %.2f seconds.\n", requestsProcessed, timeTaken);
        printf("Requests per second: %.2f\n", requestsPerSecond);
    } 
    else {
        printf("No requests processed or time taken was too short to measure.\n");
    }

    return 0;
}
