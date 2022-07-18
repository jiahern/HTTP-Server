#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>
#include <assert.h>
#include "utils.h"

// Get the relative path from client
char *filtRelPath(char *string)
{
    // Tokenize the string with " " (Space)
    strtok(string, " ");
    char *relPath = strtok(NULL, " ");

    return relPath;
}

// The main processing of the web server
void *multiProcess(void *input)
{
    data_t *data = (data_t *)input;
    int newsockfd = data->newsockfd;
    char rootPath[BUFFER_SIZE_L];
    strcpy(rootPath, data->rootPath);

    int n;

    // Assignment Requirement stated GET Request not larger than 2KB = 2048Bytes
    char buffer[TWO_KB];
    char charac[ONE_KB];
    buffer[0] = '\0';
    bool doneRead = true;

    // Check if done reading the full request from client
    while (doneRead && ((n = read(newsockfd, charac, ONE_KB)) > 0))
    {
        strcat(buffer, charac);

        int i;
        for (i = 0; i < strlen(buffer); i++)
        {
            // When the GET Request has reached an END
            if (strstr(buffer + i, "\r\n\r\n") != NULL)
            {
                doneRead = false;
            }
            else
            {
                continue;
            }
        }
    }

    if (n < 0)
    {
        perror("read");
        exit(EXIT_FAILURE);
    }

    // Null-terminate string
    buffer[n] = '\0';

    char buffer2[strlen(buffer) + 1];

    // Get the fullPath by merging root path and relative path
    strcpy(buffer2, buffer);
    char *relPath = filtRelPath(buffer2);
    char *fullPath = malloc(strlen(relPath) + strlen(rootPath) + 1);
    strcpy(fullPath, rootPath);
    strcat(fullPath, relPath);

    // Condition if ../ exist write 404 ERROR to the server
    if (strstr(fullPath, "../") != NULL)
    {
        char *output = "HTTP/1.0 404 Not Found\r\n";
        write(newsockfd, output, strlen(output));
    }

    FILE *fp = fopen(fullPath, "r");
    char *output = "HTTP/1.0 200 OK\r\n";
    if (fp != NULL)
    {
        write(newsockfd, output, strlen(output));
        char content_type[BUFFER_SIZE_S];

        // Extract out the content type using strtok
        char *string;
        char *prevString;
        string = strtok(fullPath, ".");
        while (string != NULL)
        {
            prevString = string;
            string = strtok(NULL, ".");
        }

        // Categorize the content type based on the client request's item
        if (strstr(prevString, "html") != NULL)
        {
            strcpy(content_type, "text/html");
        }

        else if (strstr(prevString, "jpg") != NULL)
        {
            strcpy(content_type, "image/jpeg");
        }

        else if (strstr(prevString, "css") != NULL)
        {
            strcpy(content_type, "text/css");
        }

        else if (strstr(prevString, "js") != NULL)
        {
            strcpy(content_type, "text/javascript");
        }

        else
        {
            strcpy(content_type, "application/octet-stream");
        }

        // Create the output with format of "content-type: %s\r\n\r\n"
        char *subOutput = "content-type: ";
        char *subOutput2 = "\r\n\r\n";
        char *finalOutput = malloc(strlen(subOutput) + strlen(content_type) + strlen(subOutput2) + 1);
        strcpy(finalOutput, subOutput);
        strcat(finalOutput, content_type);
        strcat(finalOutput, subOutput2);

        // Output the content-type to the client
        write(newsockfd, finalOutput, strlen(finalOutput));
        free(finalOutput);
    }
    else
    // When the requested file not found
    {
        char *output = "HTTP/1.0 404 Not Found\r\n";
        write(newsockfd, output, strlen(output));
    }

    // Referenced from COMP30023 Week 9 Workshop Session
    // Send file to server
    if (fp != NULL)
    {
        char file_buffer[FILE_BUFFER_FULL];
        int charac, i = 0;
        while ((charac = fgetc(fp)) != EOF)
        {
            file_buffer[i] = charac;
            i++;

            // When reached the maximum of each file's partition buffer, write to the server.
            if (i == FILE_BUFFER_GAP)
            {
                file_buffer[FILE_BUFFER_GAP] = '\0';

                n = write(newsockfd, file_buffer, FILE_BUFFER_GAP);
                if (n < 0)
                {
                    perror("write");
                    exit(EXIT_FAILURE);
                }
                i = 0;
            }
        }
        file_buffer[i] = '\0';

        // Write the remaining file buffer
        n = write(newsockfd, file_buffer, i);
        if (n < 0)
        {
            perror("write");
            exit(EXIT_FAILURE);
        }
        fclose(fp);
    }

    // Free the malloc data
    free(data);
    free(fullPath);

    // free(input);
    close(newsockfd);

    return NULL;
}