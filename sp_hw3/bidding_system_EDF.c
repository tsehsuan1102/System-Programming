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
#include <sys/time.h>


#define SIZE 1000
typedef void Sigfunc(int);

pid_t 	pid;
int 	fd;
int 	serial[3];
int 	deadline[3];
struct 	timeval	starttime;
struct	timeval nowtime;
struct 	timespec sleeptime[3];
#define ms 1000000
#define SIGUSR3 SIGWINCH
#define eps 100000

static 
void ordinary()
{
	printf("ordinary\n");
	gettimeofday(&nowtime, NULL);
	int deadtime = (nowtime.tv_sec-starttime.tv_sec)*1000000 +
				   ((int)nowtime.tv_usec - (int)starttime.tv_usec);
	deadline[0] = deadtime + 2000000;
	sleeptime[0].tv_sec = 0;
	sleeptime[0].tv_nsec = 500 * ms;
	return	;
}
static 
void patient()
{
	printf("patient\n");
	gettimeofday(&nowtime, NULL);
	gettimeofday(&nowtime, NULL);
	int deadtime = (nowtime.tv_sec-starttime.tv_sec)*1000000 +
				   ((int)nowtime.tv_usec - (int)starttime.tv_usec);
	deadline[1] = deadtime + 3000000;
	sleeptime[1].tv_sec = 1;
	sleeptime[1].tv_nsec = 0;
	return	;
}
static 
void impatient()
{
	printf("impatient\n");
	gettimeofday(&nowtime, NULL);
	gettimeofday(&nowtime, NULL);
	int deadtime = (nowtime.tv_sec-starttime.tv_sec)*1000000 +
				   ((int)nowtime.tv_usec - (int)starttime.tv_usec);
	deadline[2] = deadtime + 300000;
	sleeptime[2].tv_sec = 0;
	sleeptime[2].tv_nsec = 200 * ms;
	return	;
}

int 
main(int argc, char **argv)
{
	gettimeofday(&starttime, NULL);

	signal(SIGUSR1, ordinary);
	signal(SIGUSR2, patient);
	signal(SIGUSR3, impatient);
    
    int 	pipe_c2p[2];
    pipe(pipe_c2p);
	
	char 	buf[SIZE] = {};
	int 	processing[3] = {0, 0, 0};
	for (int i=0;i<3;i++){
		sleeptime[i].tv_sec = sleeptime[i].tv_nsec = 0;
		deadline[i] = 2147483647;
	}
	
	serial[0] = serial[1] = serial[2] = 1;
//open bidding_system_log
	fd = open("bidding_system_log", O_WRONLY | O_CREAT | O_TRUNC, 0666 );
	int 	flags = fcntl(fd, F_GETFL, 0);
	fcntl(pipe_c2p[0], F_SETFL, flags | O_NONBLOCK);
		
    if( (pid=fork()) == 0 ){
// child exec
        close(pipe_c2p[0]);
        dup2( pipe_c2p[1], STDOUT_FILENO );
        close(pipe_c2p[1]);
        execl("./customer_EDF","customer_EDF", argv[1], NULL);
	}
	close(pipe_c2p[1]);

	while( read(pipe_c2p[0], buf, 100) != 0 ){
		int 	mintime = deadline[0];
		int 	index = 0;
		for (int i=1;i<3;i++){
			if ( abs(deadline[i] - mintime ) < eps ){
				if (i==1){
					index = 1;
				}
			}
			else if (deadline[i] < mintime){
				index = i;
				mintime = deadline[i];
			}
		}
		if( deadline[index]!=2147483647 && processing[index] == 0 ){
			sprintf(buf, "receive %d %d\n", index,serial[index]);
			write(fd, buf, strlen(buf));
			processing[index] = 1;
		}
		if ( deadline[index]!= 2147483647 && nanosleep( &sleeptime[index], &sleeptime[index] ) == 0 ){
			sprintf(buf,"finish %d %d\n", index, serial[index]++);
			write(fd, buf, strlen(buf));
			deadline[index] = 2147483647;
			processing[index] = 0;

			if (index == 0)
				kill(pid, SIGUSR1);
			else if(index == 1)
				kill(pid, SIGUSR2);
			else
				kill(pid, SIGUSR3);
		}

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

	if (sigaction(signo, &act, &oact) < 0)
		return (SIG_ERR);
	return (oact.sa_handler);
}




