#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

#define MAX_STUDENTS 100


/*//////////////////////////////////////////////////////
    FUNCTION TO SEE IF THE NEW ID IS UNIQUE OR NOT
*//////////////////////////////////////////////////////

int is_uniqueID(int new_id){

    FILE* qfile= fopen("operation_queue.txt", "a+");
   
    char operation[10];
    int student_roll_no, operationId,room_no, hrs, min, sec;

    while (fscanf(qfile, "%s %d %d %d %d:%d:%d", operation, &student_roll_no, &operationId , &room_no , &hrs, &min, &sec) == 7) {
            if(student_roll_no == new_id){
               return 0;
            }
        } 
        return 1;
  }

/*//////////////////////////////////////////////////////
                   SERVER FUNCTION
*//////////////////////////////////////////////////////


void server(int pipe_r, int pipe_w) {
    // Initialization of data structures and variables for service management.
    int service_state[MAX_STUDENTS];   // 0: ready, 1: running, 2: blocked
    int service_priority[MAX_STUDENTS]; // 0: member, 1: manager, 2: admin
    int service_comp_time[MAX_STUDENTS];
    int service_wait_time[MAX_STUDENTS];
    int num_services_ready = 0;

    while (1) {
        // Reading incoming requests from pipe_r
        int req_seq_no, student_id, operation_id, submis_time;
        read(pipe_r, &req_seq_no, sizeof(int));
        read(pipe_r, &student_id, sizeof(int));
        read(pipe_r, &operation_id, sizeof(int));
        read(pipe_r, &submis_time, sizeof(int));

        // Process the requests and update service states
        int service_index = req_seq_no - 1;
        service_state[service_index] = 0; // Set to ready state

            // Update the count of ready services based on their priority
            if (service_priority[service_index] == 2) {
                num_services_ready++;  // Admin service has highest priority
            } 
            else if (service_priority[service_index] == 1 && num_services_ready <= 3) {
                num_services_ready++;  // Manager service has priority (up to 3)
            } 
            else if (num_services_ready <= MAX_STUDENTS) {
                num_services_ready++;  // Member service has priority
            }  

            // map the service as running
            service_state[service_index] = 1;

            sleep(1); 

            // Update service states, completion time, and total waiting time
            service_state[service_index] = 2; // Set to blocked state
            service_comp_time[service_index] = time(NULL);
            service_wait_time[service_index] = service_comp_time[service_index] - submis_time;

            // Update the number of services ready
            num_services_ready--;

            // Send back the response to the request generator via pipe_w
            int response[4] = {req_seq_no, submis_time, service_comp_time[service_index], service_wait_time[service_index]};
            write(pipe_w, response, sizeof(response));
    }
}
/*//////////////////////////////////////////////////////
                    MAIN FUNCTION
*//////////////////////////////////////////////////////
int main() {
    srand(time(NULL));

    int kern_to_serv[2];
    int serv_to_kern[2];
    pipe(kern_to_serv);
    pipe(serv_to_kern);     
    
    pid_t kern_pid = fork();
    if (kern_pid == 0) { 
    
    pid_t record_gen_pid = fork();
    if (record_gen_pid == 0) {
        execl("./Rec_Gen", "Rec_Gen", NULL);
        exit(1);
    }

    waitpid(record_gen_pid, NULL, 0);
           
        close(kern_to_serv[1]);
        close(serv_to_kern[0]);
         
        // Redirect stdin and stdout to pipes
        dup2(kern_to_serv[0], STDIN_FILENO);
        dup2(serv_to_kern[1], STDOUT_FILENO);
    }
    else{
    
        close(kern_to_serv[0]);
        close(serv_to_kern[1]);

        int pipe_r, pipe_w;

        //pipe for communication between main process and kernel process.
        int main_to_kern[2];
        pipe(main_to_kern);

        dup2(main_to_kern[0], STDIN_FILENO);
        //dup2(kern_to_serv[1], STDOUT_FILENO);

        pid_t admin_gen_pid = fork();
        if (admin_gen_pid == 0) {
        
        close(main_to_kern[1]);

        clock_t start_time_Admin = clock();
        FILE* qfile;
        srand(time(NULL)+ getpid());
        
        int conn_req = 1; //use any non-zero value to indicate a connection request.
        write(kern_to_serv[1], &conn_req, sizeof(int));

        int conn_response;
        read(serv_to_kern[0], &conn_response, sizeof(int));

        if (conn_response == 0) {
            printf("Request Generator Admin: Connection to server could not be established.\n");
            exit(1);
        } 
        else {
            read(serv_to_kern[0], &pipe_r, sizeof(int));
            read(serv_to_kern[0], &pipe_w, sizeof(int));
            printf("Request Generator Admin: Established connection with server.\n");

        }

        for (int i = 0; i < 12; i++) {
        
            int student_roll_no;
            while(1){
            student_roll_no = rand() % 100 + 1;
            int check = is_uniqueID(student_roll_no);
               if(check == 0){
                  continue;
               }
               else{
                break;
               }
            }
            
            int operationId = rand() % 4;

            time_t currentTime = time(NULL);
            struct tm* timeInfo = localtime(&currentTime);
            int hr = timeInfo->tm_hour;
            int min = timeInfo->tm_min;
            int sec = timeInfo->tm_sec;

            qfile= fopen("operation_queue.txt", "a+");

            if (operationId == 2) {
                int room_no
         = rand() % 100;
                
                printf("Generated: Admin %d %d (%d) %02d:%02d:%02d\n", student_roll_no, operationId, room_no
        ,hr, min, sec);
                fprintf(qfile, "Admin %d %d %d %02d:%02d:%02d\n", student_roll_no, operationId , room_no
         , hr, min, sec);
                fflush(qfile);
            } 
            else {
                printf("Generated: Admin %d %d %02d:%02d:%02d\n", student_roll_no, operationId, hr, min, sec);
                fprintf(qfile, "Admin %d %d %d %02d:%02d:%02d\n", student_roll_no, operationId , 0, hr, min, sec);
                fflush(qfile);
            }

            int req_seq_no = i + 1;  // A unique identifier for the request
            int student_id = student_roll_no;       
            int operation_id = operationId; 
            int submis_time = time(NULL);  // The current time as submission time

            // Send the request data to the server through the communication pipe
            write(pipe_w, &req_seq_no, sizeof(int));
            write(pipe_w, &student_id, sizeof(int));
            write(pipe_w, &operation_id, sizeof(int));
            write(pipe_w, &submis_time, sizeof(int));

            int waitTime = rand() % 4 + 2;
            sleep(waitTime);
        }

        clock_t end_time_Admin = clock();
        double time_taken_Admin = (double)(end_time_Admin - start_time_Admin) / CLOCKS_PER_SEC;
        printf("Time taken by Admin Request Generator: %f sec\n", time_taken_Admin);

        exit(0);
    }

    pid_t manager_gen_pid = fork();
        if (manager_gen_pid == 0) {

        close(main_to_kern[1]);

        clock_t start_time_Manager = clock();
        FILE* qfile;
        srand(time(NULL)+ getpid());

        int conn_req = 1;
        write(kern_to_serv[1], &conn_req, sizeof(int));

        int conn_response;
        read(serv_to_kern[0], &conn_response, sizeof(int));

        if (conn_response == 0) {
            printf("Request Generator Manager: Connection to server could not be established.\n");
            exit(1);
        } 
        else {
            int pipe_r, pipe_w;
            read(serv_to_kern[0], &pipe_r, sizeof(int));
            read(serv_to_kern[0], &pipe_w, sizeof(int));
            printf("Request Generator Manager: Established connection with server.\n");

        }

        for (int i = 0; i < 15; i++) {
            int student_roll_no;
            while(1){
            student_roll_no = rand() % 100 + 1;
            int check = is_uniqueID(student_roll_no);
               if(check == 0){
                  continue;
               }
               else{
                break;
               }
            }
            
            int operationId = rand() % 4;

            time_t currentTime = time(NULL);
            struct tm* timeInfo = localtime(&currentTime);
            int hr = timeInfo->tm_hour;
            int min = timeInfo->tm_min;
            int sec = timeInfo->tm_sec;

            qfile= fopen("operation_queue.txt", "a+");

            if (operationId == 2) {
                int room_no
         = rand() % 100;
                printf("Generated: Manager %d %d (%d) %02d:%02d:%02d\n", student_roll_no, operationId, room_no
        ,hr, min, sec);
                fprintf(qfile, "Manager %d %d %d %02d:%02d:%02d\n", student_roll_no, operationId , room_no
         , hr, min, sec);
                fflush(qfile);
            } 
            else {
                printf("Generated: Manager %d %d %02d:%02d:%02d\n", student_roll_no, operationId, hr, min, sec);
                fprintf(qfile, "Manager %d %d %d %02d:%02d:%02d\n", student_roll_no, operationId , 0, hr, min, sec);
                fflush(qfile);
            }

            int req_seq_no = i + 1;  
            int student_id = student_roll_no;       
            int operation_id = operationId;   
            int submis_time = time(NULL);  

            write(pipe_w, &req_seq_no, sizeof(int));
            write(pipe_w, &student_id, sizeof(int));
            write(pipe_w, &operation_id, sizeof(int));
            write(pipe_w, &submis_time, sizeof(int));

            int waitTime = rand() % 4 + 1;
            sleep(waitTime);
        }

        clock_t end_time_Manager = clock();
        double time_taken_Manager = (double)(end_time_Manager - start_time_Manager) / CLOCKS_PER_SEC;
        printf("Time taken by Manager Request Generator: %f sec\n", time_taken_Manager);

        exit(0);   
    }

    pid_t member_gen_pid = fork();
        if (member_gen_pid == 0) {

        close(main_to_kern[1]);

        clock_t start_time_Member = clock();
        FILE* qfile;
        srand(time(NULL)+ getpid());

        // Request the kernel for a connection
        int conn_req = 1;
        write(kern_to_serv[1], &conn_req, sizeof(int));

        // Receive the response from the kernel
        int conn_response;
        read(serv_to_kern[0], &conn_response, sizeof(int));

        if (conn_response == 0) {
            printf("Request Generator Member: Connection to server could not be established.\n");
            exit(1);
        } 
        else {
            int pipe_r, pipe_w;
            read(serv_to_kern[0], &pipe_r, sizeof(int));
            read(serv_to_kern[0], &pipe_w, sizeof(int));
            printf("Request Generator Member: Established connection with server.\n");

        }

        for (int i = 0; i < 35; i++) {
            int student_roll_no;
            while(1){
            student_roll_no = rand() % 100 + 1;
            int check = is_uniqueID(student_roll_no);
               if(check == 0){
                  continue;
               }
               else{
                break;
               }
            }
            
            int operationId = rand() % 4;

            time_t currentTime = time(NULL);
            struct tm* timeInfo = localtime(&currentTime);
            int hr = timeInfo->tm_hour;
            int min = timeInfo->tm_min;
            int sec = timeInfo->tm_sec;

            qfile= fopen("operation_queue.txt", "a+");

            if (operationId == 2) {
                int room_no
         = rand() % 100;
                printf("Generated: Member %d %d (%d) %02d:%02d:%02d\n", student_roll_no, operationId, room_no
        ,hr, min, sec);
                fprintf(qfile, "Member %d %d %d %02d:%02d:%02d\n", student_roll_no, operationId , room_no
         , hr, min, sec);
                fflush(qfile);
            } 
            else {
                printf("Generated: Member %d %d %02d:%02d:%02d\n", student_roll_no, operationId, hr, min, sec);
                fprintf(qfile, "Member %d %d %d %02d:%02d:%02d\n", student_roll_no, operationId , 0, hr, min, sec);
                fflush(qfile);
            }

            int req_seq_no = i + 1; 
            int student_id = student_roll_no;       
            int operation_id = operationId;   
            int submis_time = time(NULL);  

            write(pipe_w, &req_seq_no, sizeof(int));
            write(pipe_w, &student_id, sizeof(int));
            write(pipe_w, &operation_id, sizeof(int));
            write(pipe_w, &submis_time, sizeof(int));

            sleep(1);
        }

        clock_t end_time_Member = clock();
        double time_taken_Member = (double)(end_time_Member - start_time_Member) / CLOCKS_PER_SEC;
        printf("Time taken by Member Request Generator: %f sec\n", time_taken_Member);
   
        exit(0);
    }

    waitpid(admin_gen_pid, NULL, 0);
    waitpid(manager_gen_pid, NULL, 0);
    waitpid(member_gen_pid, NULL, 0);
    
    }

    if (fork() == 0) {
        execl("./Server", "Server", NULL);
        exit(1);
    } 
    else {
        wait(NULL);
    }

    return 0;
}
