
/*
 This program will fork two child processes running the two programs generated in programming zero in parallel.
 Hint: You mant need to use fork(), exec() family, wait(), exit(), getpid() and etc   ...
 
 Requirements:
 

 1) Exactly two child processes are created, one to run testspecial program and the other is to run testalphabet program;
 2) When a program starts to run, print a message to the output screen showing which process (with PID) is running which program, for example:
    "CHILD <16741> process is executing testalphabet program!"
    
 3) When a program is done, the process exits and at the same time, a message should be print to the output screen showing which  process (with PID) is done with the program, for example:
    "CHILD <16741> process has done with testalphabet program !"
    
 4) The messages should match the real execution orders, i.e. when the testspecial program starts/ends, the right message should be print out. So you need to figure out how to get the starting/ending time of each 
 process.
   
   
 The expected screen print out should be similar as follows:
 
 CHILD <16741> process is executing testalphabet program!
 CHILD <16742> process is executing testspecial program!
, -> 745668
. -> 798072
... ...

CHILD <16742> process has done with testspecial program !
a -> 2973036
b -> 556908
... ...

CHILD <16741> process has done with testalphabet program !
 */

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <ctype.h>
#include <libgen.h>   // for dirname
#include <string.h> 
#include <limits.h>

int main(void) {
    pid_t childpid1, childpid2;
    char path[PATH_MAX];
    char testalphabetPath[PATH_MAX];
    char testspecialPath[PATH_MAX];

    // Get the path of the current directory
    readlink("/proc/self/exe", path, PATH_MAX);
    char *dir = dirname(path);

    // Build the paths to testalphabet and testspecial
    strcpy(testalphabetPath, dir);
    strcat(testalphabetPath, "/testalphabet");
    strcpy(testspecialPath, dir);
    strcat(testspecialPath, "/testspecial");

    // Fork the first child process
    childpid1 = fork();
    if (childpid1 < 0) {
        perror("fork failed");
        exit(1);
    }
    else if (childpid1 == 0) {
        chdir(dir); //allows for program to read to the file path of the executable
        printf("CHILD <%d> process is executing testalphabet program!\n", getpid());
        execl(testalphabetPath, "testalphabet", (char*) NULL);
        perror("execl failed");  // execl only returns on error
        exit(1);
    }

    // Fork the second child process
    childpid2 = fork();
    if (childpid2 < 0) {
        perror("fork failed");
        exit(1);
    }
    else if (childpid2 == 0) {
        chdir(dir);
        printf("CHILD <%d> process is executing testspecial program!\n", getpid());
        execl(testspecialPath, "testspecial", (char*) NULL);
        perror("execl failed");  // execl only returns on error
        exit(1);
    }

    // Wait for the first child to finish
    waitpid(childpid1, NULL, 0);
    printf("CHILD <%d> process is done with testalphabet program!\n", childpid1);

    // Wait for the second child to finish
    waitpid(childpid2, NULL, 0);
    printf("CHILD <%d> process is done with testspecial program!\n", childpid2);

    return 0;
}
