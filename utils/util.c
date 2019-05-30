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

ssize_t readn(int fd, void* buf, size_t n){////从内核读
    size_t nleft;
    ssize_t nread;
    nleft = n;
    while(nleft > 0){
        if ((nread = read(fd, buf, nleft)) < 0){
            if (nleft == n) ///一个字节没写成功
                return -1;
            else
                break;
        }else if (nread == 0)
            break;
        nleft -= nread;
        buf += nread;
    }
    return (n - nleft);
}

ssize_t writen(int fd, void* buf, size_t n){
    size_t nleft;
    ssize_t nwrite;
    nleft = n;
    while(nleft > 0){
        if ((nwrite = write(fd, buf, nleft)) < 0){
            if (nleft == n)
                return -1;
            else
                break;
        }else if(nwrite == 0)
            break;
        nleft -= nwrite;
        buf += nwrite;
    }
    return (n - nleft);
}