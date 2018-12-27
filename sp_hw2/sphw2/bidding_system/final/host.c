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
#include <sys/wait.h>


int
main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stdout, "usage: %s [host_num]\n", argv[0]);
        exit(1);
    }
    int    host_id;
    sscanf(argv[1], "%d", &host_id);
/////// create 5 FIFO
// host[host_id].FIFO: read responses from 4 players 
// host[host_id]_A.FIFO: write message to player_A 
// host[host_id]_B.FIFO: write message to player_B 
// host[host_id]_C.FIFO: write message to player_C 
// host[host_id]_D.FIFO: write message to player_D 

//host.FIFO
    char Name[100] = "host";
    strcat(Name, argv[1]);
    strcat(Name, ".FIFO");
    mkfifo(Name, 0777);
    int readfd = open(Name, O_RDWR);
    char code[4][2] = {"A", "B", "C", "D"};
    int writefd[4];
    for(int i=0;i<4;i++){
        strcpy(Name, "host");
        strcat(Name, argv[1]);
        strcat(Name, "_");
        strcat(Name, code[i]);
        strcat(Name, ".FIFO");
        mkfifo(Name, 0777);
        writefd[i] = open(Name, O_RDWR);
        // printf("%d\n",writefd[i]);
    }
//////// read 4 player id from bidding system    
    int     A, B, C, D;
    int     rankA=1, rankB=1, rankC=3, rankD=3;
    while(1) {
        //printf("host:%d\n", host_id);
        scanf("%d %d %d %d", &A, &B, &C, &D);
		//fprintf(stderr, "%d:[%d %d %d %d]\n",host_id,A,B,C,D);
/////// -1 -1 -1 -1 as complete
        if( A==-1 && A==B && B==C && C==D ){
            //fprintf(stderr, "host[%d]stop!\n", host_id );
    		strcpy(Name, "host");
   	 		strcat(Name, argv[1]);
   			strcat(Name, ".FIFO");
			//fprintf(stderr,"name=%s\n",Name);
    		remove(Name);
    		for(int i=0;i<4;i++){
        		strcpy(Name, "host");
        		strcat(Name, argv[1]);
        		strcat(Name, "_");
     		   	strcat(Name, code[i]);
        		strcat(Name, ".FIFO");
				remove(Name);
			}
			close(readfd);
			for(int i=0;i<4;i++)
				close(writefd[i]);
			//fprintf(stderr, "host out\n");
            exit(0);
        }
		char 	rand[4][5]={"1", "2", "3", "4"};
        int 	moneyA, moneyB, moneyC, moneyD;
        moneyA = moneyB = moneyC = moneyD = 1000;
/// fork 4 child A, B, C, D
//###A
		pid_t pidA, pidB, pidC, pidD;
		if( (pidA = fork()) < 0 ){
            fprintf(stderr, "fork error\n");
        }else if (pidA == 0){
             execlp("./player", "./player", argv[1], "A", rand[0], NULL);
         }
//###B
         if( (pidB = fork()) < 0 ){
             fprintf(stderr, "fork error\n");
         }else if (pidB == 0){
             execlp("./player", "./player", argv[1], "B", rand[1], NULL);
         }    
//###C
         if( (pidC = fork()) < 0 ){
             fprintf(stderr, "fork error\n");
         }else if (pidC == 0){
             execlp("./player", "./player", argv[1], "C", rand[2], NULL);
         }
//###D
        if( (pidD = fork()) < 0 ){
             fprintf(stderr, "fork error\n");
        }else if (pidD == 0){
             execlp("./player", "./player", argv[1], "D", rand[3], NULL);
        }
		char message[20];
		char playerreturn[20];
		for(int round=1;round<=10;round++){
			//fprintf(stderr, "round:%d\n", round);
			sprintf(message, "%d %d %d %d\n",moneyA,moneyB,moneyC,moneyD);
			for(int i=0;i<4;i++){
				write(writefd[i], message, strlen(message));
				read(readfd, playerreturn, 20);
				//fprintf(stderr, "player:%s\n", playerreturn);
			}
			if( round%4==1 ) moneyA = 0;
			if( round%4==2 ) moneyB = 0;
			if( round%4==3 ) moneyC = 0;
			if( round%4==0 ) moneyD = 0;
			moneyA+=1000;
			moneyB+=1000;
			moneyC+=1000;
			moneyD+=1000;
		}
		waitpid(pidA,NULL,0);
		waitpid(pidB,NULL,0);
		waitpid(pidC,NULL,0);
		waitpid(pidD,NULL,0);
        
		printf("%d %d\n%d %d\n%d %d\n%d %d\n",A,rankA,B,rankB,C,rankC,D,rankD);
		fflush(stdout);
    }
}









