#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "data-structs.h"
#include "file-operation.h"

#define getClock() ((double)clock() / CLOCKS_PER_SEC)

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
    if (argc < 3)
    {
        printf("Usage: ./job-shop <input-file> <output-file>\n");
        return 1;
    }

    int numberOfJobs = 0;
    int numberOfMachines = 0;
    int numberOfOperations = 0;

    readFile(argv[1], &numberOfMachines, &numberOfJobs, &numberOfOperations);

    /*  printf("################ EntryPoint Data ##################\n");
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
  */
    allocateMachines(numberOfMachines);
    allocateScheduler(numberOfJobs, numberOfOperations);

    // get start time
    clock_t t = clock();
    // double startTime = getClock();

    sheduleJobs(numberOfJobs, numberOfOperations, numberOfMachines);

    // get end time
    t = clock() - t;
    double endTime = ((double)t) / CLOCKS_PER_SEC;
    // clock_t end = clock() - start;
    // double endTime = getClock();

    writeToFileAndPrettyPrint(argv[2], numberOfJobs, numberOfOperations);

    // print time difference
    /*     printf("################ Scheduler Execution Time ##################\n");
        printf("\n");
        printf("\n"); */
    printf("%f\n", endTime);
}
