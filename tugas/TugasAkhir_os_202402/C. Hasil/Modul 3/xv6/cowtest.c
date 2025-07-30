#include "types.h"
#include "stat.h"
#include "user.h"

int
main(void)
{
  char *mem = sbrk(1);
  *mem = 'X';

  int pid = fork();
  if(pid < 0){
    printf(1, "fork failed\n");
    exit();
  }

  if(pid == 0){
    *mem = 'Y';
    printf(1, "Child sees: %c\n", *mem);
    exit();
  } else {
    wait();
    printf(1, "Parent sees: %c\n", *mem);
  }

  exit();
}
