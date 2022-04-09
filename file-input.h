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
                jobshop.jobs[j].operations[k].machineId = atoi(token);
                token = strtok(NULL, " ");
                jobshop.jobs[j].operations[k].duration = atoi(token);
                token = strtok(NULL, " ");
            }
            j++;
        }

        i++;
    }

    fclose(file);
}