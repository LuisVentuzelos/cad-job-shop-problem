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
    struct scheduler_ scheduler[MAX][MAX];
} jobshop;