#include <stdio.h>
#include <stdint.h>
#include <sched.h>
#include <string.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>


#define ROW (50)
#define COL ROW

long ms;
int count, cpuid, parentPid;
pid_t *pid;

struct sched_attr
{
	uint32_t size;
	uint32_t sched_policy;
	uint64_t sched_flags;
	int32_t sched_nice;


	uint32_t sched_priority;


	uint64_t sched_runtime;
	uint64_t sched_deadline;
	uint64_t sched_period;
};

static int sched_setattr(pid_t pid, const struct sched_attr *attr, unsigned int flags)
{
	return syscall(SYS_sched_setattr, pid, attr, flags);
}

void calc(int time, int cpu)
{
	struct timespec begin, end;

	int matrixA[ROW][COL];
	int matrixB[ROW][COL];
	int matrixC[ROW][COL];

	int i,j,k;
	cpuid = cpu;
	int flag=1;

	
	clock_gettime(CLOCK_MONOTONIC, &begin);

	while(1)
	{
		for(int i=0; i< ROW; i++)
		{
			for(int j=0; j< COL; j++)
			{
				for(k=0; k < COL; k++) matrixC[i][j] += matrixA[i][k] * matrixB[k][j];
			}
		}
		count++;

		
		clock_gettime(CLOCK_MONOTONIC, &end);
		
		ms = (end.tv_sec - begin.tv_sec)*1000 + (end.tv_nsec - begin.tv_nsec)/1.0e6;
		
		if(ms>=(100*flag))
		{
			printf("PROCESS #%02d count = %02d %d\n", cpuid, count, 100);
			flag++;
		}
		if(ms >= time*1000) break;
	}
	printf("DONE!! PROCESS #%02d : %06d %ld\n", cpu, count, ms);
	exit(0);
}
void sigint_handler(int signo)
{
	if(getpid()==parentPid) {}
	else
	{
		printf("DONE!! PROCESS #%02d : %06d %ld\n", cpuid, count, ms);
	}
	exit(0);
}

int main(int argc, char *argv[])
{
	parentPid = getpid();
	signal(SIGINT, sigint_handler);

	int result;


	struct sched_attr attr;


	memset(&attr, 0, sizeof(attr));
	attr.size = sizeof(struct sched_attr);


	attr.sched_priority = 95;
	attr.sched_policy = SCHED_RR;


	result = sched_setattr(getpid(), &attr, 0);


	if(result == -1)
	{
		perror("Error calling sched_setattr.");
	}

	int ProcessNum = atoi(argv[1]);
	int cpuTime = atoi(argv[2]);
	int status;
	int runProcess =0;
	
	pid = (pid_t *)malloc(sizeof(pid_t) * ProcessNum);	

	while(runProcess < ProcessNum)
	{
		printf("Creating Process: #%d\n", runProcess);
		pid[runProcess] = fork();
		
	
		if(pid[runProcess] < 0)
		{
			return -1;
		}

	
		else if(pid[runProcess] == 0)
		{
			calc(cpuTime, runProcess);
		}
		else
		{
			runProcess++;
		}

	}
	for(int i=0; i<ProcessNum; i++)
		wait(0);
	return 0;
}

