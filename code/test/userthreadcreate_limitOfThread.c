/**
 * userthreadcreate_limitOfThread.c
 * 
 * Unit test to check whether we create too many threads
 * 
 * Be careful to settle the value of NB_MAX_THREADS to the one 
 * in system.h. 
 * 
 * Sadly there a problem with dependencies that didn't allow us to include
 * system.h in which the macro is located.
 */

#include "syscall.h"
#include "minunit.h"

#define NB_THREADS 100
#define NB_MAX_THREADS 10

int nbThreads = 0;
int nbThreadsError = 0;

void print(void *phrase)
{
    PutString((char *)phrase);
}

void create_threads()
{
    char *phrase = "Un thread\n";

    for (int i = 0; i < NB_THREADS; i++)
    {
        if (UserThreadCreate(print, phrase) != -1)
            nbThreads++;
        else
            nbThreadsError++;
    }
}

static char *test_number_of_threads_limit()
{
    create_threads();
    mu_assert("Error, nbThreads > MAX_NB_THREADS\n", nbThreads <= NB_MAX_THREADS);
    return 0;
}

int main()
{
    char *result = test_number_of_threads_limit();

    if (result != 0)
        PutString(result);
    else
        PutString("ALL TESTS PASSED\n");

    return result != 0;
}