/*
 * NCA001
 *
 * NaiveConstantArg 001
 *
 */
#include <stdio.h>
#include <stdlib.h>

void
seed_random()
{

	srandom(0xdeadbeef);
}

int
main(int argc, char **argv)
{
	long int rv;

	seed_random();
	rv = random();
	(void)printf("random value = %ld\n", rv);	
	return 0;
}
