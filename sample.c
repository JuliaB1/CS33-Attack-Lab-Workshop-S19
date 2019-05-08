#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

//void segmentation_fault_handler(int i)
//{
//  exit(1);
//}

void nightking()
{
  //signal(SIGSEGV, segmentation_fault_handler);
  printf("ATTACK SUCCESSFUL WOOHOO\n");
}

unsigned getbuf()
{
  char buf[25];
  gets(buf);
  return 1;
}

int main()
{
  //fprintf(stderr, "Enter in a string to launch an attack:\n");
  getbuf();
  printf("Attack unsuccessful. Try again, it's not too late to save Westeros!\n");
  return 0;
}
