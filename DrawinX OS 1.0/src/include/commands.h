#include "FDrawin.h"

uint_32 runCommand(char command[256], uint_32 linea)
{
    uint_32 linee;
    if (strncmp("cd ", command, 3) == 0)
    {
        printf("Opening directory: ", linea, 0);
        setRowColumnForDebugPrintf(linea, 21);
        debugPrintf("%s", command + 3);
        fs_opendir(command + 3);
        linee = linea;
    }
    else if (strncmp("mkdir ", command, 6) == 0)
    {
        printf("Creating directory: ", linea, 0);
        setRowColumnForDebugPrintf(linea, 22);
        debugPrintf("%s", command + 6);
        fs_create(command + 6, 1);
        linee = linea;
    }
    else if (strncmp("touch ", command, 6) == 0)
    {
        printf("Creating file: ", linea, 0);
        setRowColumnForDebugPrintf(linea, 18);
        debugPrintf("%s", command + 6);
        fs_create(command + 6, 0);
        linee = linea;
    }
    else if (strncmp("ls", command, 2) == 0)
    {
        printf("Listing directory...", linea, 0);
        linea++;
        char *names = fs_list();
        setRowColumnForDebugPrintf(linea, 0);
        debugPrintf("%s", names);
        linee = linea;
    }
    else if (strncmp("write ", command, 6) == 0)
    {
        char *args = command + 6;
        char *filename = strtok(args, " ");
        char *text = strtok(NULL, "");

        if (!filename || !text)
        {
            printf("Usage: write <filename> <text>", linea, 0);
        }
        else
        {
            int file = fs_open(filename);
            if (file == -1)
            {
                fs_create(filename, 0);
                file = fs_open(filename);
            }
            fs_write(file, (uint_8 *)text, strlen(text));
            fs_close(file);
            printf("Written to file successfully", linea, 0);
        }
        linee = linea;
    }
    else if (strncmp("read ", command, 5) == 0)
    {
        char *filename = command + 5;
        int file = fs_open(filename);
        if (file == -1)
        {
            printf("File not found", linea, 0);
        }
        else
        {
            uint_8 buf[256];
            memset(buf, 0, 256);
            fs_read(file, buf, 256);
            printf("File contents: ", linea, 0);
            setRowColumnForDebugPrintf(linea, 16);
            debugPrintf("%s", buf);
            fs_close(file);
        }
        linee = linea;
    }

    else
    {
        printf("Command not found", linea, 0);
        linee = linea;
    }
    return linee;
}