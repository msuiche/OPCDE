/*
 * NCA002
 *
 * NaiveConstantArg 002
 *
 * The diff between this and 001 is using a pointer
 * which causes load/store to occur. They should be
 * removed by mem2reg. Let's see.
 *
 */
#include <stdio.h>
#include <stdlib.h>

void
seed_random()
{
	unsigned int *seedp;

	seedp = (unsigned int *)malloc(sizeof(unsigned int));
	if (seedp == NULL) return;
	*seedp = 0xdeadbeef;
	srandom(*seedp);
	free(seedp);
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
