#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include "data-structs.h"
//#include "file-input.h"

#define MAXBUFLEN 1000000
#define NUMBER_OF_JOBS 3
#define NUMBER_OF_MACHINES 3
#define NUMBER_OF_OPERATIONS 3

#define MAX 100

struct job_
{
    struct operation_ operations[MAX];
};

struct jobshop_
{
    struct job_ jobs[MAX];
    struct machine_ *machines[MAX];
    struct scheduler_ scheduler[MAX][MAX];
} jobshop;

void readFile(const char *filePath, int *machines, int *jobs, int *operations)
{
    FILE *file = fopen(filePath, "r");
    char line[256];
    int i, j = 0;

    while (fgets(line, sizeof(line), file))
    {
        if (i == 0)
        {
            *machines = atoi(line);
        }
        else if (i == 1)
            *jobs = atoi(line);
        else if (i == 2)
            *operations = atoi(line);
        else
        {
            int k = 0;
            char *token = strtok(line, " ");
            for (k = 0; k < *operations; k++)
            {
                printf("%s\n", token);
                jobshop.jobs[j].operations[k].machineId = atoi(token);
                token = strtok(NULL, " ");
                printf("%s\n", token);
                jobshop.jobs[j].operations[k].duration = atoi(token);
                token = strtok(NULL, " ");
            }
            j++;
        }

        i++;
    }

    printf("%d %d %d\n", *machines, *jobs, *operations);

    fclose(file);
}

void sheduleJobs()
{

    for (int i = 0; i < NUMBER_OF_JOBS; i++)
    {
        for (int j = 0; j < NUMBER_OF_OPERATIONS; j++)
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
                {

                    totalTime = currentMachineTime;
                }
                else
                {
                    totalTime = totalBeforeTime + currentMachineTime;
                }

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
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    char const *const fileName = argv[1];

    int numberOfJobs = 0;
    int numberOfMachines = 0;
    int numberOfOperations = 0;

    readFile(fileName, &numberOfMachines, &numberOfJobs, &numberOfOperations);

    printf("Number of Jobs: %d\n", numberOfJobs);
    printf("Number of Machines: %d\n", numberOfMachines);
    printf("Number of Operations: %d\n", numberOfOperations);

    sheduleJobs();

    printf("################ EntryPoint ##################\n");

    for (int i = 0; i < NUMBER_OF_JOBS; i++)
    {
        for (int j = 0; j < NUMBER_OF_OPERATIONS; j++)
        {
            printf("(%d,%d)\t", jobshop.jobs[i].operations[j].machineId, jobshop.jobs[i].operations[j].duration);
        }
        printf("\n");
    }
    printf("################ TIME ##################\n");

    for (int i = 0; i < NUMBER_OF_JOBS; i++)
    {
        for (int j = 0; j < NUMBER_OF_OPERATIONS; j++)
        {
            printf("%d ", jobshop.scheduler[i][j].startTime);
        }
        printf("\n");
    }
}
