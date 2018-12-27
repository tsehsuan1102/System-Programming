/*
./bidding_system [host_num] [player_num] 
1 <= host_num <= 12
4 <= player_num <= 20

first:
fork and execute the number of hosts with id from 1 to host_num
and build pipe to communicate with them

hold competition:
C(player_num, 4)

message to host:
[player1_id] [player2_id] [player3_id] [player4_id]\n in ascending order

wait until a host is available
if all competition is done send -1 -1 -1 -1\n to all host

*/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include<sys/wait.h> 

void combinationUtil(int arr[],int n,int m,int index,int data[],int i);
void printCombination(int arr[], int n, int m);
void swap(int a,int b);
int     compe[5000][4];
int     idx;



typedef struct{
	int number;
	int score;
	int rk;
}player;
player players[21];

int
main(int argc, char** argv)
{
    if (argc != 3) {
        fprintf(stderr, "usage: %s [host_num] [player_num]\n", argv[0]);
        exit(1);
    }
    int     host_num;
    int     player_num;
    int     n;
    //pipe_in as read_end  : [0] for read & [1] for write
    //pipe_out as write_end  : [0] for read & [1] for write
    int     pipe_in[13][2];
    int     pipe_out[13][2];
    pid_t   pid[13];
    char    str_id[3];
	
    sscanf(argv[1], "%d", &host_num);
    sscanf(argv[2], "%d", &player_num);

		
	for (int i=1;i<=player_num;i++){
		players[i].number = i;
	}

    for (int i=1;i<=host_num;i++){
        sprintf(str_id, "%d", i);
// build pipe
        if( pipe(pipe_in[i]) < 0 ) fprintf(stderr, "inpipe error\n");
        if( pipe(pipe_out[i]) < 0 ) fprintf(stderr, "outpipe error\n");
// fork a child
        if( (pid[i] = fork()) < 0 ){
//error
            fprintf(stderr, "fork error\n");
            exit(0);
        }else if( pid[i] > 0 ){
// parent #######################################
			close(pipe_out[i][0]);
			close(pipe_in[i][1]);
        }else{
// child ########################################
            //改IO為pipe
			close(pipe_out[i][1]);
			close(pipe_in[i][0]);
            dup2(pipe_out[i][0], STDIN_FILENO);
            dup2(pipe_in[i][1], STDOUT_FILENO);
            close(pipe_out[i][0]);
            close(pipe_in[i][1]);
            execlp("./host", "./host", str_id, NULL);
            exit(0);
        }
    }
// communicate by pipe
    idx = 0;
    int arr[21];
    for(int i=0;i<player_num;i++)
        arr[i] = i+1;
    printCombination(arr, player_num, 4);

    // distribute to every host
    for(int t=1;t<=host_num;t++){
        if(t>idx) {
            break;
        }
        char message[100];
        sprintf(message, "%d %d %d %d\n", compe[t-1][0], compe[t-1][1], compe[t-1][2], compe[t-1][3]);
        write(pipe_out[t][1], message, strlen(message));
    }

	int recnt = 0;  // read conut
    //read FD_SET
    fd_set rset;
	int 	maxfd=0;
	for(int i=1;i<=host_num;i++){
		if( pipe_in[i][0] > maxfd )
			maxfd = pipe_in[i][0];
	}
	//printf("mxfd%d\n",maxfd);
    for(int round=host_num;round<idx;round++) {
        //printf("\n#################\n\ncom:{%d %d %d %d}\n", compe[round][0], compe[round][1], compe[round][2], compe[round][3]);
        while(1){
			FD_ZERO(&rset);
        	for(int i=1;i<=host_num;i++)
            	FD_SET(pipe_in[i][0], &rset);
            int 	re = select(maxfd+1, &rset, NULL, NULL, NULL);

            int		flag = 1;
			if( flag && re >= 0 ){
                for(int j=1;j<=host_num;j++){
                    //printf("%d\n", j);
                    if( FD_ISSET(pipe_in[j][0], &rset) ){
                        char hostreturn[100];
                        read(pipe_in[j][0], hostreturn, 100);
						recnt++;
						
						int thisid[4];
						int rank[4];
						sscanf(hostreturn,"%d %d\n%d %d\n%d %d\n%d %d\n",&thisid[0], &rank[0],&thisid[1],&rank[1],&thisid[2],&rank[2],&thisid[3],&rank[3]);
						int ranktoscore[5] = {0, 3, 2, 1, 0};
						for(int x=0;x<4;x++)
							players[ thisid[x] ].score += ranktoscore[rank[x]];
						
						//printf("hostreturn:%s\n", hostreturn);
                        //printf("pipe[%d] is ready\n",j);
                        char message[100];
                        sprintf(message, "%d %d %d %d\n", compe[round][0], compe[round][1], compe[round][2], compe[round][3]);
                        write(pipe_out[j][1], message, strlen(message));
                        flag = 0;
						break;
                    }
                }
            }
			if (flag==0) break;
        }
    }


	while( recnt < idx ){
		//printf("%d %d\n",recnt,idx);
        while(1){
			FD_ZERO(&rset);
        	for(int i=1;i<=host_num;i++)
            	FD_SET(pipe_in[i][0], &rset);
            int re = select(maxfd+1, &rset, NULL, NULL, NULL);
            int		flag = 1;
			//printf("re=%d\n", re);
			if( flag && re >= 0 ){
                for(int j=1;j<=host_num;j++){
                    //printf("%d\n", j);
                    if( FD_ISSET(pipe_in[j][0], &rset) ){
						//printf("host:%d\n", j);
                        char hostreturn[100];
                        read(pipe_in[j][0], hostreturn, 100);
						recnt++;
						//puts("?");
						int thisid[4];
						int rank[4];
						sscanf(hostreturn,"%d %d\n%d %d\n%d %d\n%d %d\n",&thisid[0], &rank[0],&thisid[1],&rank[1],&thisid[2],&rank[2],&thisid[3],&rank[3]);
						int ranktoscore[5] = {0, 3, 2, 1, 0};
						for(int x=0;x<4;x++)
							players[thisid[x]].score += ranktoscore[rank[x]];
						
						flag = 0;
						break;
					}
				}
			}
			if(flag == 0)
				break;
		}
	}

// send -1 -1 -1 -1
    char endmessage[] = "-1 -1 -1 -1";
    for(int i=1;i<=host_num;i++){
        write(pipe_out[i][1], endmessage, strlen(endmessage)+1);
		close(pipe_out[i][1]);
		close(pipe_in[i][0]);
    }
	

	for (int i=player_num;i>0;i--){
		for(int j=1;j<i;j++){
			if( players[j].score < players[j+1].score ){
				//swap(j,j+1);
				int t = players[j].score;
				players[j].score = players[j+1].score;
				players[j+1].score = t;

				t = players[j].number;
				players[j].number = players[j+1].number;
				players[j+1].number = t;

			}
		}
	}

	

	int nowid = 1;
	//int nowrk = 1;
	players[nowid].rk = 1;
	nowid = 2;
	while ( nowid <= player_num ){
		if( players[nowid].score == players[nowid-1].score ){
			players[nowid].rk = players[nowid-1].rk;
		}else{
			players[nowid].rk = nowid;
		}
		nowid++;
	}

	for(int i=1;i<=player_num;i++){
		printf("%d %d\n",players[i].number, players[i].rk);
	}





	for (int i=1;i<=host_num;i++){
		//printf("i,pid=%d %d\n", i,pid[i]);
		waitpid(pid[i], NULL, 0);
	}
   

/*
	for(int i=player_num;i>0;i--){
		for(int j=1;j<player_num;j++){
			if( score[j]>score[j+1] ){
				int t = score[j];
				score[j] = score[j+1];
				score[j+1] = t;
				t = index[j];
				index[j] = index[j+1];
				index[j+1] = t;
			}
		}
	}
	/*
	for(int i=1;i<=player_num;i++){
		printf("%d %d\n",index[i], score[i]);
	}*/
	/*int nowid = 1;
	int ranknumber = 1;
	while(nowid <= player_num){
		for(int i=1;i<=player_num;i++){
			//printf("i=%d",i);
			if( index[i] == nowid )
				printf("%d %d\n",nowid,ranknumber);
		}
		ranknumber++;
		if( score[nowid] == score[nowid-1] ) ranknumber--;
		nowid++;
	}
*/
	exit(0);
}

		

//###############################################################################################
void printCombination(int arr[], int n, int m) 
{
    int data[m]; 
    combinationUtil(arr, n, m, 0, data, 0); 
}
void combinationUtil(int arr[], int n, int m, int index, int data[], int i) 
{ 
    if (index == m){ 
        for (int j=0;j<m;j++)
            compe[idx][j] = data[j];
            // printf("%d ",data[j]);
        idx++;
        return; 
    } 
    if (i >= n) return;
    // current is included
    data[index] = arr[i]; 
    combinationUtil(arr, n, m, index+1, data, i+1);
    // current is excluded
    combinationUtil(arr, n, m, index, data, i+1);
} 

















