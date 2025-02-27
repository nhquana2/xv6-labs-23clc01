#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "kernel/param.h"

#define MAXLINE 512

int main(int argc, char *argv[]) {
    char buf[MAXLINE];
    int pos, r;
    char c;

    if (argc < 2) {
        printf("xargs: Missing arguments\n");
        exit(1);
    }

    while (1) {
        pos = 0;

        //Read a line
        while (1) {
            r = read(0, &c, 1);

            if (r < 0) {
                printf("xargs: Input error\n");
                exit(1);
            }

            if (r == 0) break; //EOF
            if (c == '\n') break;

            if (pos < MAXLINE-1) buf[pos++] = c;
        }

        if (r == 0 && pos == 0) break; //EOF and no character read

        buf[pos] = '\0'; //Line termination

        //Build new args array
        char* args[MAXARG];
        int arg_count = 0;
        //Note: argv[0] = "xargs"
        for (int i = 1; i < argc; ++i) {
            args[arg_count++] = argv[i];
        }

        //Parse line buf into args
        int i = 0;
        while (buf[i]) {
            if (buf[i] == ' ' || buf[i] == '\t') ++i;
            if (buf[i] == '\0') break;

            args[arg_count++] = &buf[i];
            if (arg_count >= MAXARG-1) {
                printf("xargs: Too many arguments\n");
                exit(1);
            }

            while (buf[i] && buf[i] != ' ' && buf[i] != '\t') ++i;
            if (buf[i]) {
                buf[i] = '\0';
                ++i;
            }
        } 

        args[arg_count] = 0; //Null-terminate the args array

        //Fork child and execute command
        int pid = fork();

        if (pid < 0) {
            printf("xargs: Fork error\n");
            exit(1);
        }

        if (pid == 0) {
            //Child process
            exec(args[0], args);
            //exec return: failed
            printf("xargs: exec failed\n");
            exit(1);
        }

        wait(0); //Parent wait child

    }

    exit(0);
}