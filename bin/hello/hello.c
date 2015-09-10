#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[], char* envp[]) {
	int fd;
  printf("Hello World!\n");
  fd = open("README.2md", O_RDWR);
  fd++;
	return 0;
}
