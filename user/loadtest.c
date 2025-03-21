//Background load test
#include "kernel/types.h"
#include "user/user.h"

int main() {
  printf("Starting background CPU load test\n");
  

  int pid = fork();
  if(pid > 0) {
    printf("Load test running in background\n");
    printf("Run 'sysinfotest' to check load average\n");
    exit(0);
  }
  
  for(int i = 0; i < 20; i++) {
    int child_pid = fork();
    if(child_pid == 0) {
      while(1) {
        volatile long sum = 0;
        for(volatile long j = 0; j < 10000000; j++) {
          sum += j;
        }
      }
      exit(0); 
    }
  }
  

  while(1) {
    sleep(1000);
  }
  
  exit(0);
}