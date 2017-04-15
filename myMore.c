#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>

#define READ_END 0
#define WRITE_END 1
#define BUFFER_SIZE 100

int main(int argc, char * argv[])
{
    int fd[2];
    int size, i;
    char command;

    fd[0] = atoi(argv[1]);
    fd[1] = atoi(argv[2]);
    size = atoi(argv[3]);

    char * readMessage = (char *) malloc(sizeof(char) * BUFFER_SIZE);

    close(fd[WRITE_END]);

    for(i = 0; i < size; i++) // loop until reaching the total lines
    {
        if(i != 0 && i % 23 == 0) // reading 24 lines
        {
            scanf("%c", &command);
            if(command == 'q') // quit
            {
                fflush(stdout);
                break;
            }
        }
        else // reading the remaining lines
        {
            read(fd[READ_END], readMessage, sizeof(char) * BUFFER_SIZE);
            printf("%s\n", readMessage);
        }
    }

    close(fd[READ_END]);

    exit(0);
}