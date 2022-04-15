#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include "data-structs.h"
#include "file-operation.h"

void allocateMachines(int numberOfMachines)
{
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

void sheduleJobs(int numberOfJobs, int numberOfOperations, int numberOfMachines)
{

    for (int operation = 0; operation < numberOfOperations; operation++)
    {
        for (int job = 0; job < numberOfJobs; job++)
        {
            int totalTime = 0;
            int operationBeforeStartTime = 0;

            int machineId = jobshop.jobs[job].operations[operation].machineId;

            int operationBeforeDuration = jobshop.jobs[job].operations[operation - 1].duration;

            if (operation > 0)
                operationBeforeStartTime = jobshop.scheduler[job][operation - 1]->startTime;

            int operationTotalBeforeTime = operationBeforeStartTime + operationBeforeDuration;

            int currentMachineTime = jobshop.machines[machineId]->startTime + jobshop.machines[machineId]->duration;

            if (currentMachineTime > operationTotalBeforeTime)
                totalTime = currentMachineTime;
            else
                totalTime = operationTotalBeforeTime;

            jobshop.machines[machineId]->startTime = totalTime;
            jobshop.machines[machineId]->duration = jobshop.jobs[job].operations[operation].duration;

            jobshop.scheduler[job][operation]->startTime = jobshop.machines[machineId]->startTime;
            jobshop.scheduler[job][operation]->duration = jobshop.machines[machineId]->duration;
            jobshop.scheduler[job][operation]->assigned = 1;
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
    allocateScheduler(numberOfJobs, numberOfOperations);

    sheduleJobs(numberOfJobs, numberOfOperations, numberOfMachines);

    writeToFileAndPrettyPrint(outputFileName, numberOfJobs, numberOfOperations);
}
