#include <stdio.h>
#include <stdlib.h>

void
foo(int *ptr)
{
	long int k = 0;
	k = random();
	printf("Yessir %ld %d\n", k, *ptr);
	return;
}

void
foo2(char *dingle)
{
	long int k = 0;
	
	k = random();
	printf("dingle %ld %s\n", k, dingle);
}
void
foo3(int *ptr, char *foo)
{
	long int k = 0;
	k = random();
	printf("Yessir %ld %d %s\n", k, *ptr, foo);
}

int
main(int argc, char **argv)
{
	foo(&argc);
	foo2("hi there");
	foo3(&argc, "hi there");
	foo2(NULL);
	foo((int *)NULL);
	return 0;
}
