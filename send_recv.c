#include "mpi.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/* According to POSIX.1-2001 */
#include <sys/select.h>

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include <signal.h> // for signal()
#include <unistd.h> // for alarm()
#include <assert.h> // for assert()

#define SIZE 4

int rank;
int g_i;

char *buff;
int bufsize, tag=1;
char data[15000000];

int num_of_receives;
long send_buffsize;

volatile int alarmCheck;

int PostCopy_Recv(char *buff, int buffsize, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Status *stat );

extern int select();
void MyWait(long msec) {
    
}

float a[1000][1000];
float b[1000][1000];
float c[1000][1000];


void 
sigalrm_handler(int signo)
{
    alarmCheck = 1;
    return;
}

void 
calc(long sleep)
{
    sig_t result = signal(SIGALRM, sigalrm_handler);
    assert(result != SIG_ERR);
    alarmCheck = 0;
    ualarm(sleep, 0);

    int size = 1000;

    int i, j;
    for(i = 0; i < size; i++)
    {
        for(j = 0; j < size; j++)
        {
            if(alarmCheck == 1)
            {
                alarmCheck = 0;
                return;
            }
            a[i][j] = 1 + (int)(70.0 * rand() / (RAND_MAX + 1.0));
            b[i][j] = (double) (rand() % 5);
        }
    }

    while(alarmCheck == 0)
    {
        int k;
        for(i = 0; i < size; i++)
        {
            for(j = 0; j < size; j++)
            {
                c[i][j] = 0.0;
                for(k = 0; k < size; k++)
                {
                    if(alarmCheck == 1)
                    {
                        alarmCheck = 0;
                        return;
                    }
                    c[i][j] = c[i][j] + a[i][k] * b[k][j];
                }
            }
        }
    }

    alarmCheck = 0;
}


void comm(long ssleep, long rsleep) {
    MPI_Status stat;
    int i,j,k,l,m;
    if(rank%2 == 0) {
       calc(ssleep);
       MPI_Send(buff, bufsize, MPI_CHAR, rank+1, tag, MPI_COMM_WORLD);
    }
    else {
        calc(rsleep);
        MPI_Recv(buff, bufsize, MPI_CHAR, rank-1, tag, MPI_COMM_WORLD, &stat);
    }
}

int main(argc,argv)
int argc;
char *argv[];  {
    int numtasks, source=0, dest, i, j;
    long ssleep = 1;
    long rsleep = 1;
    int provided;
    char *endstr;

    int val = 1;
    int iter;

    bufsize = strtol(argv[1], &endstr, 10);
    num_of_receives = strtol(argv[2], &endstr, 10);
    printf("%d\n", argc);
    if(argc > 3) {
        ssleep = strtol(argv[3], &endstr, 10);
        if(argc == 5) 
            rsleep = strtol(argv[4], &endstr, 10);
    }

    printf("Buffsize:%d  #Receives:%d  RecvSleep:%ld  SendSleep:%ld\n", bufsize, num_of_receives, ssleep, rsleep);
#ifndef USE_HYBRID
    send_buffsize = bufsize;
#endif

    g_i = 0;

    double t1, t2;
    MPI_Status stat;
    MPI_Request req1;

    buff = data;

    MPI_Init( &argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);

    for(iter = 0;iter<num_of_receives;iter++) {
        comm(ssleep,rsleep);
    }

    MPI_Finalize();
    return 0;
}

