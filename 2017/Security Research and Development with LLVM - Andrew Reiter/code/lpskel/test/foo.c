#include <stdio.h>

int
main(int argc, char **argv)
{
	unsigned z = 0;
	if (argc <= 0) argc = 30;
	for (unsigned k = 0; k < argc; ++k) {
		z += k * 2;
	}	
	printf("zeta: %u\n", z);
	return 0;
}
