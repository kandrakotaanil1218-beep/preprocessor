#ifndef FILE_INCLUSION_H
#define FILE_INCLUSION_H

#include <stdio.h>
#include <string.h>

void process_includes(FILE *input, FILE *output)
{
    char line[1000];
    char filename[150];
    char path[200];
    char *p;
    char *start;
    char *end;
    FILE *header;
    int c;

    while (fgets(line, sizeof(line), input) != NULL)
    {
        p = line;

        while (*p == ' ' || *p == '\t' ||
               *p == '\n' || *p == '\r')
            p++;

        if (strncmp(p, "#include", 8) == 0)
        {
            p += 8;

            while (*p == ' ' || *p == '\t')
                p++;

            if (*p == '<')
            {
                start = p + 1;
                end = strchr(start, '>');

                if (end != NULL)
                {
                    size_t len = (size_t)(end - start);

                    if (len < sizeof(filename))
                    {
                        memcpy(filename, start, len);
                        filename[len] = '\0';

                        if (strchr(filename, '/') == NULL)
                        {
                            strcpy(path, "/usr/include/");
                            strcat(path, filename);

                            header = fopen(path, "r");

                            if (header != NULL)
                            {
                                while ((c = fgetc(header)) != EOF)
                                    fputc(c, output);

                                fputc('\n', output);
                                fclose(header);
                                continue;
                            }
                        }
                    }
                }
            }
        }

        fputs(line, output);
    }
}

#endif
