#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void primes(int) __attribute__((noreturn));

void primes(int left_p) {
    int num;
    int prime;
    int right_p[2];

    //Read first number (must be a prime) of the process
    if (read(left_p, &prime, sizeof(int)) != sizeof(int)) {
        //This process does not receive any number, exit
        close(left_p);
        exit(0);
    }
    printf("prime %d\n", prime);

    pipe(right_p);

    int pid = fork();

    if (pid == 0) {
        close(left_p);
        close(right_p[1]);
        primes(right_p[0]);
    } else if (pid > 0) {
        close(right_p[0]);
        while (read(left_p, &num, sizeof(int)) != 0) {
            if (num % prime != 0) write(right_p[1], &num, sizeof(int));
        }
        close(left_p);
        close(right_p[1]);
        wait(0);
        exit(0);
    } else {
        printf("Fork error\n");
        exit(1);
    }

}

int main(int argc, char *argv[]) {
    
    int main_p[2];
    pipe(main_p);

    int pid = fork();

    if (pid == 0) {
        close(main_p[1]);
        primes(main_p[0]);
    } else if (pid > 0) {
        close(main_p[0]);
        for (int i = 2; i <= 280; ++i) {
            write(main_p[1], &i, sizeof(int));
        }
        close(main_p[1]);
        wait(0);
    } else {
        printf("Fork error\n");
        exit(1);
    }

    exit(0);
}