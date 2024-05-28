#define _DEFAULT_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <stdbool.h>
#include <errno.h>
#include <error.h>
#include <time.h>

#ifdef WINDOWS
#include <windows.h>
#endif

/* Constântes */
#define CONVEYOR1_WAIT_TIME 2000000
#define CONVEYOR2_WAIT_TIME 1000000

#define CONTROLLER_WAIT_TIME 2000000

#define MiB (1024*1024)

size_t   mibs  = 0;
bool     which = false;
uint8_t* buffer;
size_t   max_iter;
size_t   cur_iter = 0;
FILE*    fi;

void data_apply_values(void)
{

	for (size_t i = 0; i < mibs; i++)
		buffer[i] = rand();

	cur_iter++;

	return;
}

void data_read_values(void)
{

	long long int acc = 0;

	for (size_t i = 0; i < mibs; i++)
		acc += buffer[i];

	// Fazendo o GCC acreditar que esse calculo serve para alguma coisa	
	fprintf(fi, "%lli\n", acc);

	return;
}

/*
 * O data_apply_values é envolto de um mutex para que somente uma thread
 * tente aplicar seu valor em um determinado momento.
 */
pthread_mutex_t mx = PTHREAD_MUTEX_INITIALIZER;
void thread_data_apply_values(void)
{

	pthread_mutex_lock  (&mx);
	data_apply_values   ();
	pthread_mutex_unlock(&mx);

	return;
}

void thread_data_read_values(void)
{

	pthread_mutex_lock  (&mx);
	data_read_values    ();
	pthread_mutex_unlock(&mx);

	return;
}

void* thread_conveyor1(void* param)
{

	(void) param;

	while (cur_iter < max_iter)
		thread_data_apply_values(),
		usleep                  (CONVEYOR1_WAIT_TIME);

	pthread_exit((void*) -1);
}

void* thread_conveyor2(void* param)
{
	(void) param;

	while (cur_iter < max_iter)
		thread_data_read_values(),
		usleep                  (CONVEYOR2_WAIT_TIME);

	pthread_exit((void*) -1);
}

int main(int argc, char* argv[])
{

	srand(time(NULL));
	fi = fopen("/dev/null", "rw");

	if (argc < 3) error_at_line(EXIT_FAILURE, EINVAL, __FILE__, __LINE__, "\n");

	mibs   = strtol(argv[1], NULL, 10) * MiB;
	buffer = malloc(sizeof(uint8_t) * mibs);
//	buffer2 = malloc(sizeof(uint8_t) * mibs);

	max_iter = strtol(argv[2], NULL, 10);

	/* Threads */
	pthread_t tid_conveyor1, tid_conveyor2;
	pthread_attr_t attr;

	pthread_attr_init(&attr);

	pthread_create(&tid_conveyor1, &attr, thread_conveyor1, NULL);
	pthread_create(&tid_conveyor2, &attr, thread_conveyor2, NULL);

	pthread_join(tid_conveyor1, NULL);
	pthread_join(tid_conveyor2, NULL);

#ifdef WINDOWS
#else
	system("ps -o min_flt,maj_flt,cmd");
#endif

	return -1;
}
