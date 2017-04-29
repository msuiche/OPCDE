#include <stdio.h>
#include <stdlib.h>

struct shame {
	int one;
	int two;
	int three;
};

int
foo(struct shame *s)
{
	long int z;

	z = random();
	return s->one;
}

int
main(int argc, char **argv)
{
	struct shame *s = (struct shame *)NULL;
	foo(s);
	return 0;
}
