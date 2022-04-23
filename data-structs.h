#define MAX 1000

struct machine_
{
    int id;
    int startTime;
    int duration;
};

struct scheduler_
{
    int startTime;
    int duration;
    int assigned;
};

struct operation_
{
    int machineId;
    int duration;
};

struct job_
{
    struct operation_ operations[MAX];
};

struct jobshop_
{
    struct job_ jobs[MAX];
    struct machine_ *machines[MAX];
    struct scheduler_ *scheduler[MAX][MAX];
} jobshop;

void allocateMachines(int numberOfMachines)
{
#pragma omp parallel for
    for (int machine = 0; machine < numberOfMachines; machine++)
    {
        jobshop.machines[machine] = (struct machine_ *)malloc(sizeof(struct machine_));
        jobshop.machines[machine]->startTime = 0;
        jobshop.machines[machine]->duration = 0;
        jobshop.machines[machine]->id = machine;
    }
}

void allocateScheduler(int numberOfJobs, int numberOfOperations)
{
#pragma omp parallel for
    for (int job = 0; job < numberOfJobs; job++)
    {
        for (int operation = 0; operation < numberOfOperations; operation++)
        {
            jobshop.scheduler[job][operation] = (struct scheduler_ *)malloc(sizeof(struct scheduler_));
            jobshop.scheduler[job][operation]->startTime = 0;
            jobshop.scheduler[job][operation]->duration = 0;
            jobshop.scheduler[job][operation]->assigned = 0;
        }
    }
}
