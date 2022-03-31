#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MSGSIZE 512

int main(void)
{
    //char inbuf[MSGSIZE];
    int p[2];

    if (pipe(p) == -1)
    {
        perror("pipe call"); // from example code pipe2.c
        exit(1);
    }

    switch (fork())
    {
    case -1:
        perror("fork call");     // from example code pipe2.c
        exit(2);
    case 0:     // child writes to pipe
        dup2(p[1], STDOUT_FILENO);     // redirect standard output and error output
        close(p[0]);     // close link
        close(p[1]);
        execl("./preC", "preC", (char *)0);    // call execl() to execute pre.c
        perror("execl() failed!");
        return(EXIT_FAILURE);
    default:     // parent reads from pipe
        close(p[1]);
        dup2(p[0], STDIN_FILENO);
        close(p[0]);
        // read(p[0], inbuf, MSGSIZE); // read message in pipe (sent from child)
        // printf("%s\n", inbuf); // print message
        execl("./sortC", "sortC", (char *)0);    // call execl() to execute sort.c
        return(EXIT_FAILURE);
    }
    /*NOTREACHED*/

    return(EXIT_FAILURE);
}
// The primary changes are:

// Closing the second end of the pipe after the call to dup2().
// Revising the code in the parent branch to rework the I/O redirection.
// This compiles without warnings under my preferred stringent compilation options (on a Mac running macOS Sierra 10.12.6 still, with GCC 7.2.0):

// gcc -O3 -g -std=c11 -Wall -Wextra -Werror -Wmissing-prototypes -Wstrict-prototypes \
//     fork79.c -o fork79
// I created a pair of minimal shell scripts to work as preC and sortC:

// $ cat preC
// #!/bin/sh
// exec ls -t *.c
// $ cat sortC
// #!/bin/sh
// exec sort
// $
// The basic output from preC is:

// fork79.c        pth47.c         cpp13.c         th89.c          mnmx67.c
// pl53.c          pth23.c         fd19.c          cp53.c          mm19.c
// da73.c          bs79.c          fd53.c          th59.c          mm53.c
// example.c       pm73.c          fd23.c          th19.c          mm23.c
// rf13.c          nr13.c          fd97.c          cp41.c          mm59.c
// pipe97.c        cola83.c        fd47.c          mnmx71.c        bst29.c
// pipe83.c        cola61.c        cp79.c          mergesort47.c
// (When the output is piped, the file names are listed one per line.)

// The output from running fork97 (created by compiling fork79.c as shown) is:

// bs79.c
// bst29.c
// cola61.c
// cola83.c
// cp41.c
// cp53.c
// cp79.c
// cpp13.c
// da73.c
// example.c
// fd19.c
// fd23.c
// fd47.c
// fd53.c
// fd97.c
// fork79.c
// mergesort47.c
// mm19.c
// mm23.c
// mm53.c
// mm59.c
// mnmx67.c
// mnmx71.c
// nr13.c
// pipe83.c
// pipe97.c
// pl53.c
// pm73.c
// pth23.c
// pth47.c
// rf13.c
// th19.c
// th59.c
// th89.c
// This shows that the output is sorted by name from sortC.

// Round 1

// Here's a mildly modified version of your updated code — tagged 'first revision' in the question.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MSGSIZE 512

int main(void)
{
    //char inbuf[MSGSIZE];
    int p[2];

    if (pipe(p) == -1)
    {
        perror("pipe call"); // from example code pipe2.c
        exit(1);
    }

    switch (fork())
    {
    case -1:
        perror("fork call");     // from example code pipe2.c
        exit(2);
    case 0:     // child writes to pipe
        dup2(p[1], STDOUT_FILENO);     // redirect standard output and error output
        close(p[0]);     // close link
        close(p[1]);
        execl("./preC", "preC", (char *)0);    // call execl() to execute pre.c
        perror("execl() failed!");
        return(EXIT_FAILURE);
    default:     // parent reads from pipe
        close(p[1]);
        dup2(p[0], STDIN_FILENO);
        close(p[0]);
        // read(p[0], inbuf, MSGSIZE); // read message in pipe (sent from child)
        // printf("%s\n", inbuf); // print message
        execl("./sortC", "sortC", (char *)0);    // call execl() to execute sort.c
        return(EXIT_FAILURE);
    }
    /*NOTREACHED*/

    return(EXIT_FAILURE);
}

// The primary changes are:

// Closing the second end of the pipe after the call to dup2().
// Revising the code in the parent branch to rework the I/O redirection.
// This compiles without warnings under my preferred stringent compilation options (on a Mac running macOS Sierra 10.12.6 still, with GCC 7.2.0):

// gcc -O3 -g -std=c11 -Wall -Wextra -Werror -Wmissing-prototypes -Wstrict-prototypes \
//     fork79.c -o fork79
// I created a pair of minimal shell scripts to work as preC and sortC:

// $ cat preC
// #!/bin/sh
// exec ls -t *.c
// $ cat sortC
// #!/bin/sh
// exec sort
// $
// The basic output from preC is:

// fork79.c        pth47.c         cpp13.c         th89.c          mnmx67.c
// pl53.c          pth23.c         fd19.c          cp53.c          mm19.c
// da73.c          bs79.c          fd53.c          th59.c          mm53.c
// example.c       pm73.c          fd23.c          th19.c          mm23.c
// rf13.c          nr13.c          fd97.c          cp41.c          mm59.c
// pipe97.c        cola83.c        fd47.c          mnmx71.c        bst29.c
// pipe83.c        cola61.c        cp79.c          mergesort47.c
// (When the output is piped, the file names are listed one per line.)

// The output from running fork97 (created by compiling fork79.c as shown) is:

// bs79.c
// bst29.c
// cola61.c
// cola83.c
// cp41.c
// cp53.c
// cp79.c
// cpp13.c
// da73.c
// example.c
// fd19.c
// fd23.c
// fd47.c
// fd53.c
// fd97.c
// fork79.c
// mergesort47.c
// mm19.c
// mm23.c
// mm53.c
// mm59.c
// mnmx67.c
// mnmx71.c
// nr13.c
// pipe83.c
// pipe97.c
// pl53.c
// pm73.c
// pth23.c
// pth47.c
// rf13.c
// th19.c
// th59.c
// th89.c
// This shows that the output is sorted by name from sortC.

// Round 2

// After discussion, it seems that it will be best if the original process forks and executes the preC process with a pipe from the standard output of the preC read from the original process. Then the original process should execute the sortC process.

// To support this exercise, I created a data file data containing:

// abyssinian
// yosemite
// sierra
// high
// ocelot
// jaguar
// tiger
// lion
// leopard
// snow leopard
// puma
// panther
// cougar
// I created a revised program from the source fork31.c:

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MSGSIZE 512

int main(void)
{
    int p[2];

    if (pipe(p) == -1)
    {
        perror("pipe call"); // from example code pipe2.c
        exit(1);
    }

    switch (fork())
    {
    case -1:
        perror("fork call");     // from example code pipe2.c
        exit(2);
    case 0:     // child writes to pipe
        dup2(p[1], STDOUT_FILENO);     // redirect standard output and error output
        close(p[0]);     // close link
        close(p[1]);
        execl("./preC", "preC", (char *)0);    // call execl() to execute pre.c
        perror("execl() failed!");
        return(EXIT_FAILURE);
    default:     // parent reads from pipe
        {
        close(p[1]);
        int nbytes;
        char inbuf[MSGSIZE];
        while ((nbytes = read(p[0], inbuf, MSGSIZE)) > 0)
            write(STDOUT_FILENO, inbuf, nbytes);
        close(p[0]);
        execl("./sortC", "sortC", (char *)0);    // call execl() to execute sort.c
        return(EXIT_FAILURE);
        }
    }
    /*NOTREACHED*/

    return(EXIT_FAILURE);
}
// This is close to the revised code in 'next revision' in the question. 
// The primary difference is the use of > instead of >= to recognize EOF. With the low-level (file descriptor) I/O function read(), 
//EOF is indicated by zero bytes read; -1 indicates an error, and EOF is not an error. I also use write() to write the data to the standard output.

// I changed preC shell script to use ls -Ct *.c to reduce the amount of output.

// The result of invoking was:

// $ fork31 < data
// fork31.c        pipe83.c        cola61.c        cp79.c          mergesort47.c
// fork79.c        pth47.c         cpp13.c         th89.c          mnmx67.c
// pl53.c          pth23.c         fd19.c          cp53.c          mm19.c
// da73.c          bs79.c          fd53.c          th59.c          mm53.c
// example.c       pm73.c          fd23.c          th19.c          mm23.c
// rf13.c          nr13.c          fd97.c          cp41.c          mm59.c
// pipe97.c        cola83.c        fd47.c          mnmx71.c        bst29.c
// abyssinian
// cougar
// high
// jaguar
// leopard
// lion
// ocelot
// panther
// puma
// sierra
// snow leopard
// tiger
// yosemite
// $
// The output from the ls is no longer sorted by a secondary sort, of course. The list of names fed to sortC from data was sorted as expected. Note that if ls read from its standard input, this would not have worked
// — the sort process would have gotten no data to sort. Fortunately, ls is a data generator — it doesn't read its standard input.