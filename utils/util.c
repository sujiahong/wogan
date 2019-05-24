#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

int generateUniqueId(char* id){
    struct timespec tn;
    clock_gettime(CLOCK_REALTIME, &tn);
    unsigned long sec = (unsigned long)tn.tv_sec;
    unsigned long nsec = sec*1000000000+tn.tv_nsec;
    __pid_t pid = getpid();
    snprintf(id, 40, "%d%ld%u", pid, nsec, id);
    return 0;
}