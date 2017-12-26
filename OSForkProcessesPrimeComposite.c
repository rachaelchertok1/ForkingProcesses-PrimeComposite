#include <unistd.h>
#include <stdio.h>
#include <time.h> //for time function
#include <stdlib.h> //for rand and srand function

int main(){
  int fd[2];
  int N,pN; int i;
  int isPrime=1; //isPrime=true
  int cisPrime;
  pipe(fd);

  if(fork()==0){
    srand(time(0));
    N=rand() % 101; //child process generating random N from 0 to 100

    close (fd[0]); //closing fd that you do not need
    printf("CHILD_PID %d: Sending %d to parent \n", getpid(), N);
    write(fd[1], &N, sizeof(N)); //Writing number to parent through pipe
    close(fd[1]); //close fd you just used
    sleep(1); //waiting for parent process to write
    open(fd[0]);
    read(fd[0], &cisPrime, sizeof(cisPrime)); //reading isPrime from parent through pipe
    close(fd[0]);

    if(cisPrime==1){
      printf("CHILD_PID %d: Parent said I am PRIME \n", getpid());
    }

    else{
      printf("CHILD_PID %d: Parent said I am COMPOSITE \n", getpid());
    }
  }

  else{
    close(fd[1]); //closing fd that you do not need
    read(fd[0], &pN, sizeof(pN)); //Reading in number from child through pipe
    printf("PARENT_PID %d: I received %d from the child \n", getpid(), pN);
    close(fd[0]);

    //Calculating if a number is prime
    for(i=2; i<pN; i++){
      if(pN%i==0){
        isPrime=0; //isPrime=false
      }
    }

    //open(fd[1]);
    if(isPrime==1){//if pN is prime
      printf("PARENT_PID %d: My child is PRIME \n", getpid());
    }

    else{
      printf("PARENT_PID %d: My child is COMPOSITE \n", getpid());
    }
    open(fd[1]);
    write(fd[1], &isPrime, sizeof(isPrime));
    close(fd[1]);
  }

  return 0;
}
