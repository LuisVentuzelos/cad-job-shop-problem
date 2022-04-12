#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include "data-structs.h"
#include "file-operation.h"

void sheduleJobs(int numberOfJobs, int numberOfOperations, int numberOfMachines)
{

    /*     for (int machineId = 0; machineId < numberOfMachines; machineId++)
        {
            jobshop.machines[machineId] = (struct machine_ *)malloc(sizeof(struct machine_));
            jobshop.machines[machineId]->id = machineId;
        } */

    for (int i = 0; i < numberOfJobs; i++)
    {
        for (int j = 0; j < numberOfOperations; j++)
        {

            int machineId = jobshop.jobs[j].operations[i].machineId;
            int durationBefore = 0;
            int timeBefore = 0;
            int totalBeforeTime = 0;
            int currentOperationDuration = jobshop.jobs[j].operations[i].duration;

            if (i > 0)
            {
                durationBefore = jobshop.scheduler[j][i - 1].duration;
                timeBefore = jobshop.scheduler[j][i - 1].startTime;
                totalBeforeTime = timeBefore + durationBefore;
            }

            if (jobshop.machines[machineId] == NULL)
            {

                jobshop.machines[machineId] = (struct machine_ *)malloc(sizeof(struct machine_));
                jobshop.machines[machineId]->id = machineId;
                jobshop.machines[machineId]->currentTime = totalBeforeTime;
                jobshop.machines[machineId]->duration = currentOperationDuration;
            }

            else if (jobshop.machines[machineId] != NULL)
            {
                int currentMachineTime = jobshop.machines[machineId]->duration + jobshop.machines[machineId]->currentTime;

                int totalTime = 0;

                if (currentMachineTime > totalBeforeTime)
                    totalTime = currentMachineTime;
                else
                    totalTime = totalBeforeTime;

                jobshop.machines[machineId]->currentTime = totalTime;
                jobshop.machines[machineId]->duration = currentOperationDuration;
            }

            jobshop.scheduler[j][i] = (struct scheduler_){
                .startTime = jobshop.machines[machineId]->currentTime,
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

    int numberOfJobs = 0;
    int numberOfMachines = 0;
    int numberOfOperations = 0;

    readFile(fileName, &numberOfMachines, &numberOfJobs, &numberOfOperations);

    printf("################ EntryPoint Data ##################\n");
    printf("\n");

    printf("Number of Jobs: %d\n", numberOfJobs);
    printf("Number of Machines: %d\n", numberOfMachines);
    printf("Number of Operations: %d\n", numberOfOperations);

    sheduleJobs(numberOfJobs, numberOfOperations, numberOfMachines);

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

    printf("\n");
    printf("################ Scheduler Atribution ##################\n");
    printf("\n");

    FILE *fptr;

    fptr = fopen(outputFileName, "w");
    char result[50];

    for (int i = 0; i < numberOfJobs; i++)
    {
        for (int j = 0; j < numberOfOperations; j++)
        {
            int startTime = jobshop.scheduler[i][j].startTime;

            sprintf(result, "%d", startTime);

            printf("%s ", result);
            fprintf(fptr, "%s ", result);
        }
        printf("\n");
        fprintf(fptr, "\n");
    }

    printf("\n");
}
