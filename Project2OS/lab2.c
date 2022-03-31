#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

#ifndef READ 
#define READ 0
#endif
#ifndef WRITE
#define WRITE 1
#endif



int main() {
    char *argv[20], buf[80], n, *p, *iPath, *oPath;
    int m, status, inword, continu, iFlag, oFlag, count, pipes;
    int  right_pipe[2], left_pipe[2], locationPtr[20] = { 0 };
    pid_t pid;
    
    while(1) {

        // given for first 3 commands
        inword = count = pipes = pid = iFlag = oFlag = 0;
        p = buf;
        m = 0;
        continu=0;
        
        printf("\nshhh> ");
        while ((n = getchar()) != '\n' || continu ){
            if (n == ' ' ) {
                if (inword) {
                    inword = 0;
                    *p++ = 0;
                }
            }
            else if (n == '\n')
                continu = 0;
            else if (n == '\\' && !inword)
                continu = 1;
            else {
                if (!inword){
                    inword = 1;
                    argv[m++] = p;
                    *p++ = n;
                }
                else
                    *p++ = n;
            }
        }

        *p++ = 0;
        argv[m] = 0;
        if(strcmp(argv[0],"exit") == 0)
            exit(0);
        
        // handle | > < inputs, ref from sketch
        while (argv[count] != 0) {
            if (strcmp(argv[count], "|") == 0) {
                argv[count] = 0;
                locationPtr[pipes + 1] = count + 1;
                ++pipes;
            }
            else if (strcmp(argv[count], "<") == 0) {
                iPath = strdup(argv[count + 1]);
                argv[count] = 0;
                iFlag = 1;
            }
            else if (strcmp(argv[count], ">") == 0) {
                oPath = strdup(argv[count + 1]);
                argv[count] = 0;
                oFlag = 1;
            }
            else {
                locationPtr[count] = count;
            }
            ++count;
        }

        for (int i = 0; i <= pipes; ++i) {
            if (pipes > 0 && i != pipes) {
                pipe(right_pipe);
            }

            
            switch (pid = fork()) {
                case -1: perror("didn't exec");
                    break;
                // handle pipes, given
                case 0:
                    if ((i == 0) && (iFlag == 1)) {
                        int iFile;
                        if (iFile == -1) {
                            perror("no i file\n");
                            return(EXIT_FAILURE);
                        }
                        close(READ);
                        dup(iFile);
                        close(iFile);
                    }

                    if ((i == pipes) && (oFlag == 1)) {
                        int oFile; 
                        if (oFile < 0) {
                            perror("no o file\n");
                            return(EXIT_FAILURE);
                        }
                        close(WRITE);
                        dup(oFile);
                        close(oFile);
                    }

                    if (pipes > 0) {
                        if (i == 0){
                            close(WRITE);
                            dup(right_pipe[WRITE]);
                            close(right_pipe[WRITE]);
                            close(right_pipe[READ]);
                        }
                        else if (i < pipes) {
                            close(READ);
                            dup(left_pipe[READ]);
                            close(left_pipe[READ]);
                            close(left_pipe[WRITE]);
                            close(WRITE);
                            dup(right_pipe[WRITE]);
                            close(right_pipe[READ]);
                            close(right_pipe[WRITE]);
                        }
                        else {
                            close(READ);
                            dup(left_pipe[READ]);
                            close(left_pipe[READ]);
                            close(left_pipe[WRITE]);
                        }
                    }
                    execvp(argv[locationPtr[i]], &argv[locationPtr[i]]);
                    perror("cmd failed\n");

                    break;

                default:
                    if (i > 0) {
                        close(left_pipe[READ]);
                        close(left_pipe[WRITE]);
                    }
                    left_pipe[READ] = right_pipe[READ];
                    left_pipe[WRITE] = right_pipe[WRITE];

                    wait(&status);

                    break;
            }
        }

        for (int i = 0; i < 20; ++i) {
            argv[i] = 0;
        }
    }
    // unallocate 
    for (int i = 0; i < 20; ++i) {
        locationPtr[i] = 0;
    }
}
