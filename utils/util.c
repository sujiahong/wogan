#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <math.h>

#include <unistd.h>

int generateUniqueId(char* id){
    struct timespec tn;
    clock_gettime(CLOCK_REALTIME, &tn);
    unsigned long sec = (unsigned long)tn.tv_sec;
    // unsigned long sec2 = sec*1000000000;
    // unsigned long sec3 = sec ^ sec2;
    // printf("sec3 = %lu\n", sec3);
    unsigned long nsec = sec*1000000000+tn.tv_nsec;
    unsigned int insec = nsec & 0x00000000FFFFFFFF;
    srand(insec);
    int r = rand();
    printf("rand= %d, ulsize = %d\n", r, sizeof(unsigned long));
    printf("s= %lx, cs = %lu, ccs = %u, cccs = %lu, ns= %ld\n",sec, nsec, (unsigned int)nsec, insec, tn.tv_nsec);
    __pid_t pid = getpid();
    printf("pid= %d, idlen = %d\n", pid, sizeof(id));
    snprintf(id, 20, "%d%d", pid, r);
    return 0;
}