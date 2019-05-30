#include <iostream>
#include <cstdio>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <aio.h>
#include <signal.h>
#include <errno.h>

#include "connection.cpp"

void aioHandler(union sigval);

class TCPServer{
private:
    int m_listenSocketfd;
    int m_epollfd;
    struct epoll_event m_epevt;
    struct aiocb m_acb;
    int m_connectionArray[65536]= {0};
    char m_sendBuff[2048];
    char m_recvBuff[2048];
public:
    TCPServer(int port){
        int ret, nfds, i, connSockfd;
        struct sockaddr_in servaddr, cliaddr;
        m_listenSocketfd = socket(AF_INET, SOCK_STREAM, 0);
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        servaddr.sin_port = htons(port);
        bind(m_listenSocketfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
        listen(m_listenSocketfd, 4096);
        printf("server listen success !!\n");
        struct sigevent sev;
        memset(&sev, 0, sizeof(sev));
        sev.sigev_notify = SIGEV_THREAD;
        sev.sigev_notify_function = &aioHandler;
        sev.sigev_value.sival_ptr = this;
        sev.sigev_notify_attributes = NULL;
        sev.sigev_signo = -1;
        memset(&m_acb, 0, sizeof(m_acb));
        m_acb.aio_fildes = m_listenSocketfd;
        m_acb.aio_sigevent = sev;
        ret = aio_read(&m_acb);
        printf("aio_read = %d\n", ret);
        // struct sigaction act;
        // memset(&act, 0, sizeof(act));
        // act.sa_flags = SA_SIGINFO;
        // act.sa_sigaction = signalHandler;
        // sigemptyset(&act.sa_mask);
        // sigaction(SIGIO, &act, NULL);
        // sigset_t sigmask;
        // sigemptyset(&sigmask);
        // sigaddset(&sigmask, SIGIO);
        m_epollfd = epoll_create1(0);
        struct epoll_event epevts[10];
        while(1){
            nfds = epoll_wait(m_epollfd, epevts, 10, -1);
            printf("epoll wait fds = %d\n", nfds);
            if (nfds > 0){
                for(i = 0; i < nfds; ++i){
                    printf("read fd = %d , events = %u\n", epevts[i].data.fd, epevts[i].events);
                    read(epevts[i].data.fd, m_recvBuff, 4096);
                }
            }
            if (nfds == -1){
                printf("epoll errno = %d\n", errno);
            }
        }
        // while(1){
        //     connSockfd = accept(m_listenSocketfd, (struct sockaddr*)&cliaddr, &len);
        //     printf("connfd = %d\n", connSockfd);
        //     m_connectionArray[connSockfd] = connSockfd;
        //     char d[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaa";
        //     memcpy(m_sendBuff, d, strlen(d));
        //     printf("m_sendbuff = %s, len = %d \n", m_sendBuff, strlen(m_sendBuff));
        //     write(connSockfd, m_sendBuff, strlen(m_sendBuff));
        // }
    }

    int getListenSockfd(){
        return m_listenSocketfd;
    }

    int getEpollfd(){
        return m_epollfd;
    }

    struct epoll_event* getEpollEventPointer(){
        return &m_epevt;
    }

    struct aiocb* getAiocbPointer(){
        return &m_acb;
    }
    

    int addConnection(int fd){
        m_connectionArray[fd] = fd;
        return 0;
    }

    int rmvConnection(int fd){
        m_connectionArray[fd] = -1;
        return 0;
    }

    void send(int fd){
        //write(connSockfd, m_sendBuff, strlen(m_sendBuff));
    }

    void closeConnection(int fd){
        
    }
};

void aioHandler(union sigval sv){
    printf("singalHandler \n");
    TCPServer* ts = (TCPServer*)sv.sival_ptr;
    int ret = aio_error(ts->getAiocbPointer());
    printf("aio_error ret = %d\n", ret);
    ret = aio_return(ts->getAiocbPointer());
    printf("aio_return ret = %d\n", ret);
    struct sockaddr_in cliaddr;
    socklen_t len = sizeof(cliaddr);
    int connSockfd = accept(ts->getListenSockfd(), (struct sockaddr*)&cliaddr, &len);
    ts->addConnection(connSockfd);
    printf("connfd = %d\n", connSockfd);
    if (connSockfd < 0){
        return;
    }
    struct epoll_event* evPtr = ts->getEpollEventPointer();
    evPtr->events = EPOLLIN | EPOLLET;
    evPtr->data.fd = connSockfd;
    epoll_ctl(ts->getEpollfd(), EPOLL_CTL_ADD, connSockfd, evPtr);
    ret = aio_read(ts->getAiocbPointer());
    printf("aio_read = %d\n", ret);
}