#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char const *argv[])
{
	srand(time(NULL));
	int random = rand() % 8;
	printf("%d\n", random);
	return 0;
}