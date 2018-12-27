/*
./player [host_id] [player_index] [random_key]
host_id -> host
player_index -> {'A', 'B', 'C', 'D'}  != id
random_key -> to verify a response really from that player 


host1.FIFO ←−−read host1 −−−write → host1_A.FIFO ←−−read player_A −−−write→ host1.FIFO 

response to host:
[player_index] [random_key] [money]\n 

*/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

int
main(int argc, char **argv)
{
	if( argc != 4 ){
		fprintf(stderr,"error\n");
	}
	char Name[100];
	sprintf(Name, "host%s.FIFO", argv[1]);
	int writefd = open(Name, O_RDWR);
	sprintf(Name, "host%s_%s.FIFO", argv[1], argv[2]);
	int readfd = open(Name, O_RDWR);
	char input[100];
	for(int round=1;round<=10;round++){
		read(readfd, input, sizeof(input));
		int A,B,C,D;
		sscanf(input, "%d %d %d %d\n",&A,&B,&C,&D);
		int money = 0;
		if ( argv[2][0] == 'A' ){
			if( round%4 == 1 )
				money = A;
		}else if ( argv[2][0] == 'B' ){
			if( round%4 == 2 )
				money = B;
		}else if ( argv[2][0] == 'C' ){
			if( round%4 == 3 )
				money = C;
		}else{
			if( round%4 == 0 )
				money = D;
		}
		char message[100];
		sprintf(message, "%s %s %d\n", argv[2], argv[3], money);
		write(writefd, message, strlen(message));
	}
    exit(0);
}





