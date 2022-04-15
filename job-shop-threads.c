#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "data-structs.h"
#include "file-operation.h"

long long thread_count = 3; // set to number threads
pthread_mutex_t mutex;
pthread_cond_t cond_var;

int numberOfJobs;
int numberOfMachines;
int numberOfOperations;

void allocateMachines(int numberOfMachines)
{
#pragma omp parallel for
    for (int machine = 0; machine < numberOfMachines; machine++)
    {
        jobshop.machines[machine] = (struct machine_ *)malloc(sizeof(struct machine_));
        jobshop.machines[machine]->id = machine;
    }
}

void allocateScheduler(int numberOfJobs, int numberOfOperations)
{
    for (int job = 0; job < numberOfJobs; job++)
    {
        for (int operation = 0; operation < numberOfOperations; operation++)
        {
            jobshop.scheduler[job][operation] = (struct scheduler_ *)malloc(sizeof(struct scheduler_));
            jobshop.scheduler[job][operation]->assigned = 0;
        }
    }
}

void *sheduleJobs(void *rank)
{
    long my_rank = (long)rank;
    long long my_n = numberOfOperations / thread_count; // even division
    long long my_first_i = my_n * my_rank;
    long long my_last_i = my_first_i + my_n;

    printf("Thread %ld: %lld -> %lld\n", my_rank, my_first_i, my_last_i);

    for (int operation = my_first_i; operation < my_last_i; operation++)
    {
        for (int job = 0; job < numberOfJobs; job++)
        {

            int totalTime = 0;
            int machineId = jobshop.jobs[job].operations[operation].machineId;
            int operationBeforeDuration = jobshop.jobs[job].operations[operation - 1].duration;

            while (operation > 0 && jobshop.scheduler[job][operation - 1]->assigned == 0)
            {
                pthread_cond_wait(&cond_var, &mutex);
            }

            pthread_mutex_lock(&mutex);

            int currentMachineTime = jobshop.machines[machineId]->startTime + jobshop.machines[machineId]->duration;
            int operationBeforeStartTime = 0;

            if (operation > 0)
            {
                operationBeforeStartTime = jobshop.scheduler[job][operation - 1]->startTime;
            }
            int operationTotalBeforeTime = operationBeforeStartTime + operationBeforeDuration;

            if (currentMachineTime > operationTotalBeforeTime)
                totalTime = currentMachineTime;
            else
                totalTime = operationTotalBeforeTime;

            jobshop.machines[machineId]->startTime = totalTime;
            jobshop.machines[machineId]->duration = jobshop.jobs[job].operations[operation].duration;

            jobshop.scheduler[job][operation]->startTime = jobshop.machines[machineId]->startTime;
            jobshop.scheduler[job][operation]->duration = jobshop.machines[machineId]->duration;
            jobshop.scheduler[job][operation]->assigned = 1;

            pthread_cond_broadcast(&cond_var);
            pthread_mutex_unlock(&mutex);
        }
    }

    return NULL;
}

int main(int argc, char *argv[])
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

    if (argc != 3)
    {
        printf("Usage: ./job-shop <input-file> <output-file>\n");
        return 1;
    }

    char const *const fileName = argv[1];
    char const *const outputFileName = argv[2];

    readFile(fileName, &numberOfMachines, &numberOfJobs, &numberOfOperations);

    thread_handles = malloc(thread_count * sizeof(pthread_t));

    printf("################ EntryPoint Data ##################\n");
    printf("\n");

    printf("Number of Jobs: %d\n", numberOfJobs);
    printf("Number of Machines: %d\n", numberOfMachines);
    printf("Number of Operations: %d\n", numberOfOperations);

    printf("\n");
    printf("################ EntryPoint Matrix ##################\n");
    printf("\n");

    for (int i = 0; i < numberOfJobs; i++)
    {
        for (int j = 0; j < numberOfOperations; j++)
        {
            printf("(%d,%d)\t", jobshop.jobs[i].operations[j].machineId, jobshop.jobs[i].operations[j].duration);
        }
        printf("\n");
    }

    allocateMachines(numberOfMachines);
    allocateScheduler(numberOfJobs, numberOfOperations);

    pthread_mutex_init(&mutex, NULL);

    for (thread = 0; thread < thread_count; thread++)
        pthread_create(&thread_handles[thread], NULL, sheduleJobs, (void *)thread);

    // sheduleJobs(numberOfJobs, numberOfOperations, numberOfMachines);

    for (thread = 0; thread < thread_count; thread++)
        pthread_join(thread_handles[thread], NULL);
    //...

    free(thread_handles);
    pthread_mutex_destroy(&mutex);

    printf("\n");
    printf("################ Scheduler Atribution ##################\n");
    printf("\n");

    writeToFileAndPrettyPrint(outputFileName, numberOfJobs, numberOfOperations);
}
