void readFile(const char *filePath)
{
    FILE *fp;
    size_t len = 0;
    ssize_t read;

    FILE *file = fopen(filePath, "r");
    char line[256];

    while (fgets(line, sizeof(line), file))
    {
        if (line[0] != '#')
            printf("%s", line);
    }

    fclose(file);
}
