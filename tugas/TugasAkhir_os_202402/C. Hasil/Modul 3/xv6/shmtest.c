#include "types.h"
#include "stat.h"
#include "user.h"

int
main(void)
{
  char *shm = (char*)shmget(42);
  shm[0] = 'A';

  int pid = fork();
  if(pid < 0){
    printf(1, "fork failed\n");
    exit();
  }

  if(pid == 0){
    printf(1, "Child reads: %c\n", shm[0]);
    exit();
  } else {
    shm[0] = 'B';
    wait();
    printf(1, "Parent reads: %c\n", shm[0]);
  }

  shmrelease(42);
  exit();
}
