struct machine_
{
    int id;
    int currentTime;
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
