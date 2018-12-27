/*
./host [host_id]
host[host_id].FIFO: read responses from 4 players 
host[host_id]_A.FIFO: write message to player_A 
host[host_id]_B.FIFO: write message to player_B 
host[host_id]_C.FIFO: write message to player_C 
host[host_id]_D.FIFO: write message to player_D 


read from stdin, (wait bidding system to assign 4 players)
after being assign, fork 4 child processes and execute 4 player programs
start 10-round competition

money = pre_money - pay + 1000

using FIFO to communicate

write to stdout as following format:
[playerA_id] [playerA_rank]\n 
[playerB_id] [playerB_rank]\n 
[playerC_id] [playerC_rank]\n 
[playerD_id] [playerD_rank]\n 

*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int
main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stdout, "usage: %s [host_num]\n", argv[0]);
        exit(1);
    }
    int    host_id;
    sscanf(argv[1], "%d", &host_id);
    // printf("%d\n",host_id);
/////// create 5 FIFO
// host[host_id].FIFO: read responses from 4 players 
// host[host_id]_A.FIFO: write message to player_A 
// host[host_id]_B.FIFO: write message to player_B 
// host[host_id]_C.FIFO: write message to player_C 
// host[host_id]_D.FIFO: write message to player_D 

//host.FIFO
    char Name[] = "host";
    strcat(Name, argv[1]);
    strcat(Name, ".FIFO");
    mkfifo(Name, 0777);
    int readfd = open(Name, O_RDONLY | O_CREAT);
    char code[4][2] = {"A", "B", "C", "D"};    
    int writefd[4];
    for(int i=0;i<4;i++){
        strcpy(Name, "host");
        strcat(Name, argv[1]);
        strcat(Name, "_");
        strcat(Name, code[i]);
        strcat(Name, ".FIFO");
        mkfifo(Name, 0777);
        writefd[i] = open(Name, O_WRONLY | O_CREAT);
        // printf("%d\n",writefd[i]);
    }

//////// read 4 player id from bidding system    
    // for(int i=0;i<10;i++){
        // char input[30];
        // read(STDIN_FILENO, input, 30);
        // printf("input = %s\n",input);
    int     A, B, C, D;
    int     rankA, rankB, rankC, rankD;  
    for(int i=0;i<10;i++){
        // sleep(1);
        printf("host:%d\n", host_id);
        int re;
        scanf("%d %d %d %d", &A, &B, &C, &D);
        fprintf(stderr, "%d:[%d %d %d %d]\n",host_id,A,B,C,D);
        printf("%d %d\n\
                %d %d\n\
                %d %d\n\
                %d %d\n",A,rankA,B,rankB,C,rankC,D,rankD);
        fflush(stdout);
/////// -1 -1 -1 -1 as complete
        if( A==-1 && A==B && B==C && C==D ){
            fprintf(stderr, "host[%d]stop!\n", host_id );
            exit(0);
        }
    }

            int moneyA, moneyB, moneyC, moneyD;
            moneyA = moneyB = moneyC = moneyD = 0;
/// fork 4 child A, B, C, D
// //###A
//         pid_t pidA, pidB, pidC, pidD;
//         if( (pidA = fork()) < 0 ){
//             fprintf(stderr, "fork error\n");
//         }else if (pidA == 0){
// ////////////////child
//             execlp("./player", "./player", host_id, "A", NULL);
//             exit(0);
//         }else{
// ///////////////parent

//         }
// //###B
//         if( (pidB = fork()) < 0 ){
//             fprintf(stderr, "fork error\n");
//         }else if (pidB == 0){
// ////////////////child
//             execlp("./player", "./player", host_id, "B", NULL);
//         }else{
// ///////////////parent

        // }    
// //###C
//         if( (pidC = fork()) < 0 ){
//             fprintf(stderr, "fork error\n");
//         }else if (pidC == 0){
// ////////////////child
//             execlp("./player", "./player", host_id, "C", NULL);
//         }else{
// ///////////////parent

//         }
// //###D
//         if( (pidD = fork()) < 0 ){
//             fprintf(stderr, "fork error\n");
//         }else if (pidD == 0){
// ////////////////child
//             execlp("./player", "./player", host_id, "D", NULL);
//         }else{
///////////////parent

        // }



    // }


//////// write 4 player's money to player
    
    exit(0);
}





