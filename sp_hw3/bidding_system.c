#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>


/*
bidding_system_log
receive:
receive[customer code][serial number]
finish:
finish[customer code][serial number]
EOF:
terminate\n
*/

/*
ordinary    SIGINT 
member      SIGUSR1 
VIP         SIGUSR2
*/


#define SIZE 1000
typedef void Sigfunc(int);
//Sigfunc *signal(int, Sigfunc *);
pid_t pid;
int fd;
int serial[3];

#define ms 1000000

static void ordinary()
{
	//printf("ordinary\n");
	char buf[100];
	sprintf(buf, "receive 0 %d\n",serial[0]);
	write(fd, buf, strlen(buf));

	struct timespec time1, time2;
	time1.tv_sec = 1;
	time1.tv_nsec = 0;
	time2.tv_sec = 0;
	time2.tv_nsec = 0;
	while( nanosleep(&time1, &time2) == -1 ){
		time1.tv_sec = time2.tv_sec;
		time1.tv_nsec = time2.tv_nsec;
		time2.tv_sec = 0;
		time2.tv_nsec = 0;
	}
	fprintf(stderr, "return SIGINT\n");
	kill(pid, SIGINT);

	sprintf(buf, "finish 0 %d\n", serial[0]);
	write(fd, buf, strlen(buf));
	serial[0]++;
	return ;
}
static void member()
{
	struct 	timespec time1, time2;
	time1.tv_sec = 0;
	time1.tv_nsec = 500 * ms;
	time2.tv_sec = 0;
	time2.tv_nsec = 0;

	//printf("member\n");
	char 	buf[100];
	sprintf(buf, "receive 1 %d\n",serial[1]);
	write(fd, buf, strlen(buf));

	while( nanosleep(&time1, &time2) == -1 ){
		time1.tv_sec = time2.tv_sec;
		time1.tv_nsec = time2.tv_nsec;
		time2.tv_sec = 0;
		time2.tv_nsec = 0;
	}

	kill(pid, SIGUSR1);
	sprintf(buf, "finish 1 %d\n", serial[1]);
	write(fd, buf, strlen(buf));
	serial[1]++;
	return ;
}
static void VIP()
{
	struct timespec time1, time2;
	time1.tv_sec = 0;
	time1.tv_nsec = 200 * ms;
	time2.tv_sec = 0;
	time2.tv_nsec = 0;

	//printf("VIP\n");
	char 	buf[100];
	sprintf(buf, "receive 2 %d\n",serial[2]);
	write(fd, buf, strlen(buf));

	nanosleep(&time1, &time2);
	kill(pid, SIGUSR2);
	sprintf(buf, "finish 2 %d\n", serial[2]);
	write(fd, buf, strlen(buf));
	serial[2]++;
	return ;
}

int 
main(int argc, char **argv)
{
	signal(SIGUSR1, member);
	signal(SIGUSR2, VIP);
    
    int 	pipe_c2p[2];
    pipe(pipe_c2p);
	
	serial[0] = serial[1] = serial[2] = 1;
//open bidding_system_log
	fd = open("bidding_system_log", O_WRONLY | O_CREAT | O_TRUNC, 0666 );

    if( (pid=fork()) == 0 ){
// child
        close(pipe_c2p[0]);
        dup2( pipe_c2p[1], STDOUT_FILENO );
        close(pipe_c2p[1]);
        execlp("./customer", "./customer", argv[1], NULL);
	}
	close(pipe_c2p[1]);

	char 	buf[SIZE] = {};
	while( read(pipe_c2p[0], buf, 100) > 0 ){
		ordinary();
	}

	wait(NULL);
    sprintf(buf, "terminate\n");
	write(fd, buf, sizeof(buf));
	exit(0);
}

Sigfunc*
signal(int signo, Sigfunc *func)
{
	struct sigaction 	act, oact;
	
	act.sa_handler = func;
	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_RESTART;
	//if (signo == SIGUSR2)
	sigaddset( &act.sa_mask, SIGUSR1);

	if (sigaction(signo, &act, &oact) < 0)
		return (SIG_ERR);
	return (oact.sa_handler);
}



