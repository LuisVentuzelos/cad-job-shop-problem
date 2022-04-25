#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "data-structs.h"
#include "file-operation.h"

int thread_count = 1;
pthread_mutex_t mutex;
pthread_cond_t cond_var;

int numberOfJobs;
int numberOfMachines;
int numberOfOperations;

void *sheduleJobs(void *rank)
{
    long my_rank = (long)rank;
    long long my_n = numberOfJobs / thread_count;           // even division
    long long my_n_remainder = numberOfJobs % thread_count; // remainder

    long long my_first_i = my_n * my_rank + (my_rank < my_n_remainder ? my_rank : my_n_remainder);
    long long my_last_i = my_n * (my_rank + 1) + (my_rank + 1 < my_n_remainder ? my_rank + 1 : my_n_remainder);

    // printf("Thread %ld: %lld -> %lld\n", my_rank, my_first_i, my_last_i);

    for (int job = my_first_i; job < my_last_i; job++)
    {
        for (int operation = 0; operation < numberOfOperations; operation++)
        {

            int totalTime = 0;
            int operationBeforeStartTime = 0;

            int machineId = jobshop.jobs[job].operations[operation].machineId;
            int operationBeforeDuration = jobshop.jobs[job].operations[operation - 1].duration;

            if (operation > 0)
                operationBeforeStartTime = jobshop.scheduler[job][operation - 1]->startTime;

            int operationTotalBeforeTime = operationBeforeStartTime + operationBeforeDuration;

            pthread_mutex_lock(&mutex);

            int currentMachineTime = jobshop.machines[machineId]->startTime + jobshop.machines[machineId]->duration;

            if (currentMachineTime > operationTotalBeforeTime)
                totalTime = currentMachineTime;
            else
                totalTime = operationTotalBeforeTime;

            jobshop.machines[machineId]->startTime = totalTime;
            jobshop.machines[machineId]->duration = jobshop.jobs[job].operations[operation].duration;

            pthread_mutex_unlock(&mutex);

            jobshop.scheduler[job][operation]->startTime = jobshop.machines[machineId]->startTime;
            jobshop.scheduler[job][operation]->duration = jobshop.machines[machineId]->duration;
            jobshop.scheduler[job][operation]->assigned = 1;
        }
    }

    return NULL;
}

int main(int argc, char **argv)
{

#ifdef _OPENMP
#include <omp.h>
#define getClock() omp_get_wtime()
    omp_set_num_threads(thread_count);
#else
#include <time.h>
#define getClock() ((double)clock() / CLOCKS_PER_SEC)
#endif

    pthread_t *thread_handles;
    long thread;

    if (argc < 4)
    {
        printf("Usage: ./job-shop-threads <input-file> <output-file> <thread_count>\n");
        return 1;
    }

    // thread_count = atoi(argv[3]);

    readFile(argv[1], &numberOfMachines, &numberOfJobs, &numberOfOperations);

    thread_handles = malloc(thread_count * sizeof(pthread_t));

    /*     printf("################ EntryPoint Data ##################\n");
        printf("\n");

        printf("Number of Jobs: %d\n", numberOfJobs);
        printf("Number of Machines: %d\n", numberOfMachines);
        printf("Number of Operations: %d\n", numberOfOperations);

        printf("\n");
        printf("################ EntryPoint Matrix ##################\n");
        printf("\n"); */

    /*     for (int i = 0; i < numberOfJobs; i++)
        {
            for (int j = 0; j < numberOfOperations; j++)
            {
                printf("(%d,%d)\t", jobshop.jobs[i].operations[j].machineId, jobshop.jobs[i].operations[j].duration);
            }
            printf("\n");
        }
     */
    allocateMachines(numberOfMachines);
    allocateScheduler(numberOfJobs, numberOfOperations);

    // get start time
    double startTime = getClock();

    pthread_mutex_init(&mutex, NULL);

    // prevent unecessary threads
    long long thread_limit = thread_count < numberOfOperations ? thread_count : numberOfOperations;

    for (thread = 0; thread < thread_limit; thread++)
        pthread_create(&thread_handles[thread], NULL, sheduleJobs, (void *)thread);

    for (thread = 0; thread < thread_limit; thread++)
        pthread_join(thread_handles[thread], NULL);

    free(thread_handles);
    pthread_mutex_destroy(&mutex);

    // get end time
    double endTime = getClock();

    writeToFileAndPrettyPrint(argv[2], numberOfJobs, numberOfOperations);

    // print time difference
    /*     printf("################ Scheduler Execution Time ##################\n");
        printf("\n");
        printf("Threads created: %lld\n", thread_limit);
        printf("\n");
        printf("Execution Time: %f\n", endTime - startTime);
        printf("\n"); */
    printf("%f\n", endTime - startTime);
}
