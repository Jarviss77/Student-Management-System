#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MEM_SIZE 5
struct Student* mem_head = NULL;
#define NAME_LEN 30
#define HOSTEL_LEN 20


// STRUCTURE FOR STUDENT LINKED LIST 
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

// COUNTING THE NUMBER OF NODES USING CTNODES(COUNTNODES) FUNCTION 
int ctNodes(){
    struct Student* cur = mem_head;
    int ct=0;
    while (cur != NULL) {
        cur = cur->next;
        ct++;
    }
    return ct;
}

// LIST OF HOSTEL NAMES AND COURSES 
char hostels[5][5] = {"BH-1", "BH-2", "BH-3", "BH-4", "BH-5"};
char courses[5][10] = {"BTech", "MTech", "MS", "PhD", "MBA"};

// FUNCTION TO RANDOMLY GENERATE FIRSTNAME FROM FILE OF FIRST_NAME.TXT 
void gen_firstName(char firstNames[][NAME_LEN], int count) {
    FILE *file = fopen("first_names.txt", "r");
    if (file == NULL) {
        printf("Error opening file\n"); 
    }
    for (int i = 0; i < count; i++) {
        fscanf(file, "%s", firstNames[i]);
    }
    fclose(file);
}


// FUNCTION TO RANDOMLY GENERATE LASTNAME FROM FILE OF LAST_NAME.TXT 
void gen_lastName(char lastNames[][NAME_LEN], int count) {
    FILE *file = fopen("last_names.txt", "r");
    if (file == NULL) {
        printf("Error opening file\n");   
    }
    for (int i = 0; i < count; i++) {
        fscanf(file, "%s", lastNames[i]);
    }
    fclose(file);
}

// CLEARING THE WHOLE CACHE EVENTUALLY CLEARING THE FULL LINKED LIST
void clear_mem() {
    struct Student* cur = mem_head;
    while (cur != NULL) {
        struct Student* temp = cur;
        cur = cur->next;
        free(temp);
    }
    mem_head = NULL;
}

// INSERTING THE NEW STUDENT DATA IN CACHE
void insert(struct Student* newMember) {
    if (mem_head == NULL) {
        mem_head = newMember;
    } 
    else {
        newMember->next = mem_head;
        mem_head = newMember;
    }
}

// RANDOMLY GENERATING THE NEW RECORD
struct Student* generate_new() {
    struct Student* member = (struct Student*)malloc(sizeof(struct Student));
    if (member == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    srand(0); // GENERTAING DIFFERBNT NUMBER EVERYTIME
    int ID;
    do {
        ID = rand() % 100 + 1;
        struct Student* temp = head;
        while (temp != NULL) {
            if (temp->id == ID) {
                ID = 0;
                break;
            }
            temp = temp->next;
        }
    } while (ID == 0);
    char first_names[20][NAME_LEN];
    char last_names[20][NAME_LEN];
    gen_firstName(first_names,20);
    gen_lastName(last_names,20);
    member->id = ID;
    strcpy(member->firstname, first_names[rand() % 20]);
    strcpy(member->lastname, last_names[rand() % 20]);
    strcpy(member->hostel, hostels[rand() % 5]);
    strcpy(member->course, courses[rand() % 5]);
    member->room = rand() % 100;
    sprintf(member->dob, "%02d-%02d-%04d", rand() % 29 + 1, rand() % 11 + 1, rand() % 10 + 1990);
    member->year_of_study = rand() % 10 + 1988;
    return member;
}


// FUNCTION TO SEARCH IN CACHE
struct Student* searchInCache(int id) {
    struct Student* cur = mem_head;
    while (cur != NULL) {
        if (cur->id == id)
            return cur;
        cur = cur->next;
    }
    return NULL;
}

// DELETING THE LAST NODE IN LINKED LIST 
void delete_last(){
    struct Student* temp = mem_head;
    struct Student* prev;
    while(temp->next!=NULL){
        prev = temp;
        temp = temp->next;
    }
    if(temp == head){
        head=NULL;
    }
    else{
        prev->next=NULL;
    }
    free(temp);
}

// DELETING DATA WITH SOME PARTICULAR ID 
void delete(int id) {

    struct Student* cur = mem_head;
    struct Student* prev = NULL;

    while (cur != NULL) {
        if (cur->id == id) {
            if (prev == NULL) {
                mem_head = cur->next;
            } 
            else {
                prev->next = cur->next;
            }
            free(cur);
            return;
        }
        prev = cur;
        cur = cur->next;
    }
}

// REGISTERING DATA OF STUDENT AND WRITING IT IN FILE
void register_stu() {

    FILE* ptr = fopen("disk.txt", "w+");
    if (ptr == NULL) {
        printf("Error opening file");
        return;
    } 
    
    head = NULL;
    clear_mem();

    for (int i = 1; i <= 100; i++) {
        struct Student* member = generate_new();

        member->next = head;
        head = member;
        if(i>=96 && i<=100){
            int numCache = ctNodes();
            if(numCache>5){
               delete_last();
            }
            else{
               insert(member);
            }
        }

        fprintf(ptr, "%d\t%s\t%s\t%s\t%s\t%d\t%s\t%d\n", member->id, member->firstname, member->lastname,
                member->hostel, member->course, member->room, member->dob, member->year_of_study);
    }
    
    fclose(ptr);
    printf("Record generated\n");
}


// MAIN FUNCTION 
void main(){
    register_stu();
}
