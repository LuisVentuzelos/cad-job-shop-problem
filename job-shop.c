#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

# define N 3

struct operation_ {
    int machineId;
    int duration;
} operation;

struct job_ {
    int id;
    struct operation_ operations [N];
} job;

struct machine_ {
    int id;
    int currentTime;
    int duration;
} machine;


struct jobshop_ {
    struct job_ jobs [N];
    struct machine_ machines [N];
} jobshop;


struct job_ jobs[N];
struct machine_ *machines[N];
int operationTime[N][N];
int operationWork[N][N];

int main(int argc, char *argv[]) {

    jobs[0] = (struct job_) {
        .id = 0,
        .operations = {
            {
                .machineId = 0,
                .duration = 3
            },
            {
                .machineId = 1,
                .duration = 2
            },
            {
                .machineId = 2,
                .duration = 2
            }
        }
    };

    jobs[1] = (struct job_) {
        .id = 1,
        .operations = {
            {
                .machineId = 0,
                .duration = 2
            },
            {
                .machineId = 2,
                .duration = 1
            },
            {
                .machineId = 1,
                .duration = 4
            }
        }
    };

    jobs[2] = (struct job_) {
        .id = 2,
        .operations = {
            {
                .machineId = 1,
                .duration = 4
            },
            {
                .machineId = 2,
                .duration = 3
            }
        }
    };


    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            
            int machineId = jobs[j].operations[i].machineId;
            int durationBefore = 0;
            int timeBefore = 0;
            int totalBeforeTime = 0;

            if(i>0)
            {   
                durationBefore  = jobs[j].operations[i-1].duration;
                timeBefore = operationTime[j][i-1];
                totalBeforeTime = timeBefore + durationBefore;

            }

            if (machines[machineId] == NULL) {

                machines[machineId] = (struct machine_ *) malloc(sizeof(struct machine_));
                machines[machineId]->id = machineId;
                machines[machineId] -> currentTime = durationBefore + operationTime[j][i-1];
                machines[machineId]->duration = jobs[j].operations[i].duration;     

            } 

            else if(machines[machineId] != NULL) {


                machines[machineId] -> currentTime = totalBeforeTime + (machines[machineId]->duration + machines[machineId]->currentTime) ;
                machines[machineId]->duration = jobs[j].operations[i].duration;


            }
            
            operationTime[j][i] = machines[machineId]->currentTime;
            operationWork[j][i] = machines[machineId]->duration;

           }
    }

    for (int i = 0; i < N; i++) {
        printf("Machine %d\n", machines[i]->id);
        printf("\tDuration: %d\n", machines[i]->currentTime);
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", operationTime[i][j]);
        }
        printf("\n");
    }
    
    
}