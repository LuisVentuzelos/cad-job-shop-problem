#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "data-structs.h"
#include "file-input.h"

#define N 3

struct job_
{
    struct operation_ operations[N];
};

struct jobshop_
{
    struct job_ jobs[N];
    struct machine_ *machines[N];
    struct scheduler_ scheduler[N][N];
} jobshop;


void sheduleJobs()
{

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
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

                jobshop.machines[machineId]->currentTime = totalBeforeTime + (jobshop.machines[machineId]->duration + jobshop.machines[machineId]->currentTime);
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

    jobshop.jobs[0] = (struct job_){
        .operations = {
            {.machineId = 0,
             .duration = 3},
            {.machineId = 1,
             .duration = 2},
            {.machineId = 2,
             .duration = 2}}};

    jobshop.jobs[1] = (struct job_){
        .operations = {
            {.machineId = 0,
             .duration = 2},
            {.machineId = 2,
             .duration = 1},
            {.machineId = 1,
             .duration = 4}}};

    jobshop.jobs[2] = (struct job_){
        .operations = {
            {.machineId = 1,
             .duration = 4},
            {.machineId = 2,
             .duration = 3}}};

    char const *const fileName = argv[1];

    readFile(fileName);

    sheduleJobs();

    for (int i = 0; i < N; i++)
    {
        printf("Machine %d\n", jobshop.machines[i]->id);
        printf("\tStartTime: %d\n", jobshop.machines[i]->currentTime);
        printf("\tDuration: %d\n", jobshop.machines[i]->duration);
    }

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            printf("%d ", jobshop.scheduler[i][j].startTime);
        }
        printf("\n");
    }
}
