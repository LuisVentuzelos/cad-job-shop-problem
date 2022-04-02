#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>


typedef unsigned char bool;
static const bool False = 0;
static const bool True = 1;

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
    bool busy;
    int id;
    int currentTime;
    int duration;
} machine;

struct job_ jobs[N];
struct machine_ *machines[N];
int machineTime[N][N];

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


int currentTime = 0;
    for (int i = 0; i < N; i++) {
        //printf("Job %d\n", jobs[i].id);
        for (int j = 0; j < N; j++) {
            int machineId = jobs[j].operations[i].machineId;
            int beforeMachineValue = jobs[j-1].operations[i].machineId;

            if (machines[machineId] == NULL && i==0) {
                printf("Machine %d is free\n", machineId);

                machines[machineId] = (struct machine_ *) malloc(sizeof(struct machine_));

                machines[machineId]->busy = True;
                machines[machineId]->id = machineId;
                machines[machineId]->currentTime = 0;
                machines[machineId]->duration = jobs[j].operations[i].duration;


                machineTime[j][i] = machines[machineId]->currentTime;
                

            } 
             else if (machineTime[j][i] == NULL && machines[machineId] == NULL) {

                printf("\tMachine %d is going to be added\n", machineId);
                printf("\t Duration: %d,%d,%d\n", machineTime[j][i-1] ,jobs[j].operations[i].duration,+ machineTime[j][i-1]);
                printf("\tIteration I %d\n", i);
                printf("\tIteration J: %d\n", j);


                machines[machineId] = (struct machine_ *) malloc(sizeof(struct machine_));


                machines[machineId]->busy = True;
                machines[machineId]->id = machineId;
                machines[machineId] -> currentTime = (machines[beforeMachineValue]->duration + machineTime[j][i-1]);
                machines[machineId]->duration = jobs[j].operations[i].duration;

                machineTime[j][i] = machines[machineId]->currentTime;


            }
            else if(machineTime[j][i] == NULL && machines[machineId] != NULL) {


                //if(i>0) {
                    printf("\t\tMachine %d is going to be udpated\n", machineId);
                    printf("\t\tIteration I %d\n", i);
                    printf("\t\tIteration J: %d\n", j);
                    printf("\t\t Duratin: %d,%d,%d\n", machines[machineId]->duration ,machineTime[j][i-1] ,jobs[j].operations[i].duration);
                    printf("\t\t Machine Before: %d\n",beforeMachineValue);

                    if (machines[beforeMachineValue]->duration != NULL) {

                        machines[machineId] -> currentTime = (machines[machineId]->duration + machineTime[j][i-1]) ;

                    }
                    else {
                        machines[machineId] -> currentTime = machines[machineId] -> duration  + machineTime[j][i-1];

                    }
                    machines[machineId]->duration = jobs[j].operations[i].duration;

                //}
                //else {
                //    machines[machineId] -> duration = machines[machineId]->duration + jobs[j].operations[i].duration;
               // }
                
                machineTime[j][i] = machines[machineId]->currentTime;

                //machines[machineId] -> duration = machines[machineId]->duration + machineTime[i][j-1] + jobs[i].operations[j].duration;

            }
           

            //machineTime[i][j] = machines[machineId]->duration;


            /*machines[machineId] = (struct machine_) {
                .id = machineId,
                .duration = jobs[i].operations[j].duration
            };*/


            //printf("\tOperation %d on machine %d with duration %d\n", j, jobs[i].operations[j].machineId, jobs[i].operations[j].duration);
        }
    }

    for (int i = 0; i < N; i++) {
        printf("Machine %d\n", machines[i]->id);
        printf("\tDuration: %d\n", machines[i]->currentTime);
        printf("\tBusy: %d\n", machines[i]->busy);
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", machineTime[i][j]);
        }
        printf("\n");
    }
    
    
}