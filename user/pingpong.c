#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {

    int p_parent[2];
    int p_child[2];
    char buffer[100];

    pipe(p_parent); //Parent writes and child reads
    pipe(p_child); //Child writes and parent reads

    int pid = fork();

    if (pid == 0) {
        //Child process
        close(p_parent[1]);
        close(p_child[0]);

        if (read(p_parent[0], buffer, 1) != 1) {
            printf("%d: child read error\n", getpid());
            exit(1);
        }

        printf("%d: received ping\n", getpid());
        
        if (write(p_child[1], "c", 1) != 1) {
            printf("%d: child write error\n", getpid());
        }

        close(p_parent[0]);
        close(p_child[1]);
        exit(0);

    } else if (pid > 0) {
        //Parent process
        close(p_parent[0]);
        close(p_child[1]);

        if (write(p_parent[1], "c", 1) != 1) {
            printf("%d: parent write error\n", getpid());
            exit(1);
        }

        wait((int*)0);

        if (read(p_child[0], buffer, 1) != 1) {
            printf("%d: parent read error\n", getpid());
            exit(1);
        }
        
        printf("%d: received pong\n", getpid());

        close(p_parent[1]);
        close(p_child[0]);
        
        exit(0);
        
    } else {
        printf("Fork error!\n");
        exit(1);
    }

    exit(0);
}