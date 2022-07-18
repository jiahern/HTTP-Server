#ifndef STRUC
#define STRUC

#define BUFFER_SIZE_S 50
#define BUFFER_SIZE_L 1000
#define FILE_BUFFER_FULL 256
#define FILE_BUFFER_GAP 255
#define ONE_KB 1024
#define TWO_KB 2048

typedef struct
{
    int newsockfd;
    char rootPath[BUFFER_SIZE_L];
} data_t;

// Function Prototype

// Get the relative path from client
char *filtRelPath(char *str);

// The main processing of the web server
void *multiProcess(void *input);

#endif
