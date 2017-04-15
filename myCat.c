#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>

#define READ_END 0
#define WRITE_END 1
#define BUFFER_SIZE 100

typedef struct Node
{
    int Key;
    char Value[100];
    struct Node *next;
}Node;

void ReadFile(char *);
void Insert(int, char *);
void MyCat(int);

struct Node * Head = NULL;
int totalLine;

int main(int argc, char *argv[])
{
    if(argc == 2)
    {
        ReadFile(argv[1]);
        MyCat(0); // myCat
    }
    else if (argc == 4)
    {
        if(strcmp(argv[2], "=") == 0 && strcmp(argv[3], "myMore") == 0)
        {
            ReadFile(argv[1]);
            MyCat(1); // myMore
        }
    }

    return 0;
}

void MyCat(int isMore)
{
    Node * writeMessage = Head;
    char * readMessage = (char *) malloc(sizeof(char) * BUFFER_SIZE);;
    int fd[2];
    pid_t pid;

    if(pipe(fd) == -1)
    {
        fprintf(stderr, "Pipe failed.");
    }

    pid = fork();

    if(pid < 0)
    {
        fprintf(stderr, "Fork failed.");
    }

    if(pid > 0) // Parent
    {
        close(fd[READ_END]);  

        while(writeMessage != NULL)
        {
            write(fd[WRITE_END], writeMessage->Value, sizeof(char) * BUFFER_SIZE);
            writeMessage = writeMessage->next;
        }
 
        close(fd[WRITE_END]);
        wait(NULL);
    }

    else // Child
    {
        if(isMore == 0) // myCat
        {
            int isFinished;
            close(fd[WRITE_END]);

            while(true)
            {
                isFinished = read(fd[READ_END], readMessage, sizeof(char) * BUFFER_SIZE);
                if(isFinished == 0) break;
                printf("%s\n", readMessage);
            }

            close(fd[READ_END]);
        }
        else if(isMore == 1) // myMore
        {
            /**Parameters that will be sent myMore**/
            char fd1[10]; sprintf(fd1, "%d", fd[0]);
            char fd2[10]; sprintf(fd2, "%d", fd[1]);
            char line[10]; sprintf(line, "%d", totalLine); // number of lines 

            fflush(stdout);
            execv("./myMore", (char *[]){"./myMore", fd1, fd2, line, NULL}); // call myMore
        } 
    }
}

void ReadFile(char * inputFile) 
{
    FILE * filePointer;
    char line[100];
    int index = 1;

    filePointer = fopen(strcat(inputFile, ".txt"), "r");
    if(filePointer == NULL) return;

    while(fscanf(filePointer,"%[^\n]\n", line)!=EOF)
    {
        Insert(index++, line);
        totalLine++;
    }

    fclose(filePointer);
}

void Insert(int key, char value[])
{
    Node * node = (Node *)malloc(sizeof(Node));
    node->Key = key;
    strcpy(node->Value, value);
    node->next = NULL;

    if(Head == NULL)
    {
        Head = node;
    }
    else
    {
        Node * temp = Head;
        
        while(true)
        {
            if(temp->next == NULL)
            {
                temp->next = node;
                break;
            }
            temp = temp->next;
        }
    }
}