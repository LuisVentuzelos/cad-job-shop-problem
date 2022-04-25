void readFile(char const *const filePath, int *machines, int *jobs, int *operations)
{
    FILE *file = fopen(filePath, "r");
    char line[100000];
    int i = 0;
    int j = 0;

    if (file == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    while (fgets(line, sizeof(line), file))
    {
        if (i == 0)
            *machines = atoi(line);
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

void writeToFileAndPrettyPrint(char const *const filePath, int jobs, int operations)
{
    FILE *fptr;

    fptr = fopen(filePath, "w");
    char result[50];

    /*    printf("\n");
       printf("################ Scheduler Atribution ##################\n");
       printf("\n"); */

    for (int i = 0; i < jobs; i++)
    {
        for (int j = 0; j < operations; j++)
        {
            int startTime = jobshop.scheduler[i][j]->startTime;

            sprintf(result, "%d", startTime);

            // printf("%s ", result);
            fprintf(fptr, "%s ", result);
        }
        // printf("\n");
        fprintf(fptr, "\n");
    }

    // printf("\n");
}