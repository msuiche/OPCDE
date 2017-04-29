#include <stdio.h>
#include <sys/types.h>
#include <pthread.h>

#define	MAXITER	(4 * 4096)

char
bf8(char in)
{
	return in;
}

static void *
thd_bf8(void *a)
{
	for (unsigned i = 0; i < MAXITER; i++)
		printf("thd_bf8: %d\n", bf8(0));
	return a;
}

short
bf16(short in)
{
	return in;
}

static void *
thd_bf16(void *a)
{
	for (unsigned i = 0; i < MAXITER; i++)
		printf("thd_bf16: %d\n", bf16(0));
	return a;
}

int
bf32(int in)
{
	return in;
}

static void *
thd_bf32(void *a)
{
	for (unsigned i = 0; i < MAXITER; i++)
		printf("thd_bf32: %d\n", bf32(0));
	return a;
}

int64_t
bf64(int64_t in)
{
	return in;
}

static void *
thd_bf64(void *a)
{
	for (unsigned i = 0; i < MAXITER; i++)
		printf("thd_bf64: %ld\n", bf64(0));
	return a;
}

char
rr8(char in)
{
	return in;
}

static void *
thd_rr8(void *a)
{
	for (unsigned i = 0; i < MAXITER; i++)
		printf("thd_rr8: %d\n", rr8(0));
	return a;
}

short
rr16(short in)
{
	return in;
}

static void *
thd_rr16(void *a)
{
	for (unsigned i = 0; i < MAXITER; i++)
		printf("thd_rr16: %d\n", rr16(0));
	return a;
}

int
rr32(int in)
{
	return in;
}

static void *
thd_rr32(void *a)
{
	for (unsigned i = 0; i < MAXITER; i++)
		printf("thd_rr32: %d\n", rr32(0));
	return a;
}

int64_t
rr64(int64_t in)
{
	return in;
}

static void *
thd_rr64(void *a)
{
	for (unsigned i = 0; i < MAXITER; i++)
		printf("thd_rr64: %ld\n", rr64(0));
	return a;
}

int
main(int argc, char **argv)
{
	pthread_t a,b,c,d,e,f,g,h;
	printf("Creating.\n");
	pthread_create(&a, NULL, &thd_bf8, NULL);
	pthread_create(&b, NULL, &thd_bf16, NULL);
	pthread_create(&c, NULL, &thd_bf32, NULL);
	pthread_create(&d, NULL, &thd_bf64, NULL);
	pthread_create(&e, NULL, &thd_rr8, NULL);
	pthread_create(&f, NULL, &thd_rr16, NULL);
	pthread_create(&g, NULL, &thd_rr32, NULL);
	pthread_create(&h, NULL, &thd_rr64, NULL);
	printf("Created.\n");
	pthread_join(a, NULL); 
	pthread_join(b, NULL); 
	pthread_join(c, NULL); 
	pthread_join(d, NULL); 
	pthread_join(e, NULL); 
	pthread_join(f, NULL); 
	pthread_join(g, NULL); 
	pthread_join(h, NULL); 
	return 0;	
}
