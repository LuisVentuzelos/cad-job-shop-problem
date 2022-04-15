#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include "data-structs.h"
#include "file-operation.h"

#define MAX_THREADS 8

void allocateMachines(int numberOfMachines)
{
#pragma omp parallel for
    for (int machine = 0; machine < numberOfMachines; machine++)
    {
        jobshop.machines[machine] = (struct machine_ *)malloc(sizeof(struct machine_));
        jobshop.machines[machine]->id = machine;
    }
}

void sheduleJobs(int numberOfJobs, int numberOfOperations, int numberOfMachines)
{

    for (int i = 0; i < numberOfJobs; i++)
    {
        for (int j = 0; j < numberOfOperations; j++)
        {
            int totalTime = 0;

            int machineId = jobshop.jobs[j].operations[i].machineId;

            int operationBeforeDuration = jobshop.scheduler[j][i - 1].duration;
            int operationBeforeStartTime = jobshop.scheduler[j][i - 1].startTime;
            int operationTotalBeforeTime = operationBeforeStartTime + operationBeforeDuration;

            int currentMachineTime = jobshop.machines[machineId]->startTime + jobshop.machines[machineId]->duration;

            if (currentMachineTime > operationTotalBeforeTime)
                totalTime = currentMachineTime;
            else
                totalTime = operationTotalBeforeTime;

            jobshop.machines[machineId]->startTime = totalTime;
            jobshop.machines[machineId]->duration = jobshop.jobs[j].operations[i].duration;

            jobshop.scheduler[j][i] = (struct scheduler_){
                .startTime = jobshop.machines[machineId]->startTime,
                .duration = jobshop.machines[machineId]->duration};
        }
    }
}

int main(int argc, char *argv[])
{

    if (argc != 3)
    {
        printf("Usage: ./job-shop <input-file> <output-file>\n");
        return 1;
    }

    char const *const fileName = argv[1];
    char const *const outputFileName = argv[2];

#ifdef _OPENMP
#include <omp.h>
#define getClock() omp_get_wtime()
    omp_set_num_threads(MAX_THREADS);
#else
#include <time.h>
#define getClock() ((double)clock() / CLOCKS_PER_SEC)
#endif

    int numberOfJobs = 0;
    int numberOfMachines = 0;
    int numberOfOperations = 0;

    readFile(fileName, &numberOfMachines, &numberOfJobs, &numberOfOperations);

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

    sheduleJobs(numberOfJobs, numberOfOperations, numberOfMachines);

    printf("\n");
    printf("################ Scheduler Atribution ##################\n");
    printf("\n");

    writeToFileAndPrettyPrint(outputFileName, numberOfJobs, numberOfOperations);
}
