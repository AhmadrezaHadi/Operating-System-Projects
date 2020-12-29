#include "types.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  printf(1, "Hi the number of read syscall is %d so far!\n", getreadcount());
  exit();
}