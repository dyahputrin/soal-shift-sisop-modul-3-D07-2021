#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int pid;
int pipe1[2];
int pipe2[2];

void f1() 
{
  dup2(pipe1[1], 1);
  close(pipe1[0]);
  close(pipe1[1]);
  execlp("ps", "ps", "aux", NULL);
  perror("bad exec ps");
  _exit(1);
}

void f2() 
{
  dup2(pipe1[0], 0);
  dup2(pipe2[1], 1);
  close(pipe1[0]);
  close(pipe1[1]);
  close(pipe2[0]);
  close(pipe2[1]);
  execlp("sort", "sort", "-nrk", "3,3", NULL);
  perror("bad exec grep root");
  printf("\n");
  printf("\n");
  _exit(1);
}

void f3() 
{
  dup2(pipe2[0], 0);
  close(pipe2[0]);
  close(pipe2[1]);
  execlp("head", "head", "-5", NULL);
  perror("bad exec grep sbin");
  printf("\n");
  printf("\n");
  _exit(1);
}

void main() 
{
  if (pipe(pipe1)==-1) 
  {
    perror("bad pipe1");
    exit(1);
  }

  if ((pid=fork())==-1) 
  {
    perror("bad fork1");
    exit(1);
  } 
  else if (pid == 0) 
  {
    f1();
    printf("\n");
    printf("\n");
  }
  if (pipe(pipe2)==-1) 
  {
    perror("bad pipe2");
    exit(1);
  }
  if ((pid = fork())==-1) 
  {
    perror("bad fork2");
    exit(1);
  } 
  else if (pid==0)
  {
    f2();
    printf("\n");
    printf("\n");
  }
  close(pipe1[0]);
  close(pipe1[1]);

  if ((pid = fork())==-1) 
  {
    perror("bad fork3");
    exit(1);
  } 
  else if (pid==0) 
  {
    f3();
    printf("\n");
    printf("\n");
  }
}
