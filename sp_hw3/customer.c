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

typedef void Sigfunc(int);
#define FileSize 1000000
char input[FileSize];

struct 	timeval	starttime;
pid_t 	ppid;
int 	readfile;
int 	logfile;
int 	id;
double 	arrtime;
int 	serial[3] = {1, 1, 1};
int 	istimeout = 1;
int 	cntsend;
int 	cntfinish;

static
void ordinary()
{
	//gettimeofday(&starttime, NULL);
	fprintf(stderr, "ordinary!\n");
	char 	buf[100]={};
	sprintf(buf, "finish 0 %d\n", serial[0]++);
	write(logfile, buf, sizeof(buf));
	cntfinish++;

	return ;
}

static
void member()
{
	//gettimeofday(&starttime, NULL);
	istimeout = 0;
	fprintf(stderr, "member!\n");
	char 	buf[100]={};
	sprintf(buf, "finish 1 %d\n", serial[1]++);
	write(logfile, buf, sizeof(buf));
	alarm(0);
	cntfinish++;

	return ;
}

static
void VIP()
{
	//gettimeofday(&starttime, NULL);
	fprintf(stderr, "VIP!\n");
	char 	buf[100]={};
	sprintf(buf, "finish 2 %d\n", serial[2]++);
	write(logfile, buf, sizeof(buf));
	cntfinish++;
	
	return ;
}

static
void checkalarm()
{
	if( istimeout ){
		char buf[100]={};
		sprintf(buf, "timeout 1 %d\n", serial[1]);
		write(logfile, buf, sizeof(buf));
		exit(0);
	}
}

int
main(int argc, char **argv)
{
	cntsend = cntfinish = 0;
	ppid = getppid();
	//fprintf(stderr,"%d",ppid);
	logfile = open("customer_log", O_WRONLY | O_CREAT | O_TRUNC , 0666);
	readfile = open(argv[1], O_RDONLY );

	//ordinary
	signal(SIGINT, ordinary);
	//member
	signal(SIGUSR1, member);
	//VIP
	signal(SIGUSR2, VIP);

	signal(SIGALRM, checkalarm);
	
	read(readfile, input, FileSize);
	
	int 	offset;
	char 	*start = input;
	
	struct timespec sleeptime;
	long long 	nowtime=0ll, pretime=0ll;
	//pretime.tv_sec = pretime.tv_nsec = 0;
	sleeptime.tv_sec = sleeptime.tv_nsec = 0;

	while( sscanf(start, "%d %lf %n", &id, &arrtime, &offset) == 2){
		//fprintf(stderr, "arr: %d %lg\n",id, arrtime);
		start += offset;
		long long 	tmp = 10*(arrtime);
		long long 	nowtime = tmp * 100000000;

		//fprintf(stderr, "now:%lld\n",nowtime);

		sleeptime.tv_sec = ((long long)(nowtime)-pretime)/1000000000;
		sleeptime.tv_nsec = ((long long)(nowtime)-pretime)%1000000000;
		pretime = nowtime;

		fprintf(stderr,"%lld %lld\n",sleeptime.tv_sec, sleeptime.tv_nsec);
		while( nanosleep(&sleeptime, &sleeptime) == -1);
		char buf[100]={};
		sprintf(buf, "send %d %d\n", id, serial[id]);
		write(logfile, buf, sizeof(buf));

		if( id == 0 ){
			fprintf(stdout, "ordinary\n");
			cntsend++;
			fflush(stdout);
		}
		else if( id == 1 ){
			istimeout = 1;
			kill(ppid, SIGUSR1);
			cntsend++;
			alarm(1);
		}
		else{
			kill(ppid, SIGUSR2);
			cntsend++;
		}
	}
	
	while( cntfinish < cntsend ){
		sleep(1);
	}

	
	
	
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

































