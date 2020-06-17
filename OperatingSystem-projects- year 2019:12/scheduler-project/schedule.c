#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <libgen.h>
#include <sys/time.h>
#include <errno.h>
#define MAX_ARGUMENTS 10
#define MAX_PRCESSES 100

int shift = 0;     //global variables for signal handlers
int termin = 0;
typedef struct process
{
   char *pName;
   char *args[MAX_ARGUMENTS + 2];
   pid_t pid;
   int numArgs;
}Process;
void launch(Process**,int,int);
void scheduler(Process**,int,int);
int checker(Process**,int);
void timerHandler(int);
void terminHandler(int);
int findP(int,char**);
int main(int argc, char*argv[])
{
   Process **cmdProc;
   char* ptr;
   int firstProcBool = 1;
   int index = 0;
   int argNo = 0;
   int i = 0;
   int numProc = 0;
   int quantum = 0;
   struct sigaction sa1;
   struct sigaction sa2;
   sa1.sa_handler = &timerHandler;
   sa1.sa_flags = SA_RESTART | SA_NOCLDSTOP;
   sa2.sa_handler = &terminHandler;
   sa2.sa_flags = SA_RESTART |  SA_NOCLDSTOP;   // flag to not call sigchld when process starts or stops
   sigaction(SIGALRM,&sa1,NULL);              // handler for the timer
   sigaction(SIGCHLD,&sa2,NULL);              // handler for termination
   quantum = strtol(argv[1],&ptr,10);
   numProc = findP(argc,argv);                 // finds the number of processes
   cmdProc = (Process**)calloc(numProc,sizeof(Process*)); //   DATA STRUCTURE: ARRAY OF STRUCTURE(Processes) POINTERS DYNAMICALLY ALLOCATED
   if (cmdProc == NULL)
   {
      printf("calloc gone wrong\n");
      perror(NULL);
   }
   for(i=0;i<numProc;i++)
   {
      cmdProc[i] = (Process*)calloc(1,sizeof(Process));
      if (cmdProc == NULL)
      {
         printf("calloc gone wrong\n");
         perror(NULL);
      }
   }
   for(i=2;i<argc;i++)                       // parsing of command lines
   {
      if( strcmp(argv[i],":") != 0)
      {
         if(firstProcBool == 1)
         {
            cmdProc[index]->pName = argv[i];
            cmdProc[index]->args[argNo] = argv[i];
            firstProcBool = 0;
            argNo = 1;
         }
         else
         {
            cmdProc[index]->args[argNo] = argv[i];
            argNo ++;
         }
      }
      else
      {
         firstProcBool = 1;
         cmdProc[index]->numArgs = argNo;
         cmdProc[index]->args[argNo] = '\0';
         argNo = 0;
         index ++;
      }
   cmdProc[index]->numArgs = argNo;
   cmdProc[index]->args[argNo] = '\0';
   }
   launch(cmdProc,index,quantum);       // launch the processes
   free(cmdProc);
   return 0;
}
int findP(int argc,char **argv)
{
   int i = 0;
   int count = 0;
   for(i =2;i<argc;i++)
   {
      if(strcmp(argv[i],":") == 0)
      {
         count ++;
      }
   }
   return count + 1;
}
void launch(Process **cmdProc,int numCmds,int quantum)
{
   int i = 0;
   pid_t pid;
   int status;
   for(i=0; i<=numCmds; i++)
   {
      pid = fork();
      if(pid < 0)
      {
         printf("forkin gone wrong\n");
         perror(NULL);
      }
      if(pid != 0)
      {
         if(kill(pid,SIGSTOP) < 0) //stoping all processes
         {
            printf("kill gone wrong\n");
            perror(NULL);
         }
         cmdProc[i]->pid = pid;
      }
      if(pid ==0)
      {
         if (execvp(cmdProc[i]->pName,cmdProc[i]->args) < 0)
         {
            printf("exec gone wrong\n");
            perror(NULL);
         }
      }
 } 
   scheduler(cmdProc,numCmds,quantum);
}
void scheduler(Process** cmdProc,int numCmds,int quantum)
{
   int count = 0;
   int status;
   struct itimerval timer;
   while(checker(cmdProc,numCmds))
   {
      if(cmdProc[count] != NULL)              //checks if any process is running
      {
         timer.it_value.tv_sec = quantum/1000;             //setting up timer
         timer.it_value.tv_usec = (quantum*1000)%1000000;
         timer.it_interval.tv_sec = 0;
         timer.it_interval.tv_usec = 0;
         if(setitimer(ITIMER_REAL,&timer,NULL)<0)
         { 
            printf("timer gone wrong\n");
            perror(NULL);
         }
         if(kill(cmdProc[count]->pid,SIGCONT) < 0)
         {
            printf("kill gone wrong\n");
            perror(NULL);
         }
         pause();
         if(termin)                                    // termin flag checks if sigchld recieved
         {
            if(waitpid(cmdProc[count]->pid,&status, WUNTRACED) == -1)
            {
               printf("waitin gone wrong\n");
               perror(NULL);
            }
            termin = 0;
            free(cmdProc[count]);                    // free the processes resources and set it to NULL to imply the process isn't running
            cmdProc[count] = NULL;
         }
         else if(shift)                               // shift flag checks if sigalrm is recieved
         {
            if(kill(cmdProc[count]->pid,SIGSTOP) < 0)
            {
               printf("kill gone wrong\n");
               perror(NULL);
            }
            shift = 0;
         }  
      }
      count ++;
      if(count > numCmds)     // check to see if it is the last process and moves it to the first
      {
         count = 0;
      }
   } 
}
void terminHandler(int sig)
{
   termin = 1;
}
void timerHandler(int sig)
{
   shift = 1;
}
int checker(Process **processes,int numCmds) //helper function to check if any one of the processes is still not terminated
{
   int i;
   for(i=0;i<=numCmds;i++)
   {
      if(processes[i] != NULL)
      {
        return 1;
      } 
   }
   return 0;       
}
