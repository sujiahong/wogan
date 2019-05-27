#include <iostream>
#include <cstdio>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/epoll.h>

#include "connection.cpp"

class TCPServer{
private:
    int m_listenSocketfd;
    int m_eopllfd;
    int m_connectionArray[65536]= {0};
    char m_sendBuff[4096];
    char m_recvBuff[4096];
public:
    TCPServer(int port){
        int ret, nfds, i, connSockfd;
        struct sockaddr_in servaddr, cliaddr;
        socklen_t len = sizeof(cliaddr);
        m_listenSocketfd = socket(AF_INET, SOCK_STREAM, 0);
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        servaddr.sin_port = htons(port);
        bind(m_listenSocketfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
        listen(m_listenSocketfd, 4096);
        printf("server listen success !!\n");
        m_eopllfd = epoll_create1(0);
        struct epoll_event ev, events[10];
        ev.events = EPOLLIN;
        ev.data.fd = m_listenSocketfd;
        epoll_ctl(m_epollfd, EPOLL_CTL_ADD, m_listenSocketfd, &ev);
        while(1){
            nfds = epoll_wait(m_eopllfd, events, 10, -1);
            if (nfds == -1){
                
            }
            for(i = 0; i < nfds; ++i){
                if (events[i].data.fd == m_listenSocketfd){
                    connSockfd = accept(m_listenSocketfd, (struct sockaddr*)&cliaddr, &len);
                    if (connSockfd < 0){

                    }
                    ev.events = EPOLLIN | EPOLLET;
                    ev.data.fd = connSockfd;
                    epoll_ctl(m_eopllfd, EPOLL_CTL_ADD, connSockfd, &ev);
                }else{

                }
            }
        }
        while(1){
            len = sizeof(cliaddr);
            connSockfd = accept(m_listenSocketfd, (struct sockaddr*)&cliaddr, &len);
            printf("connfd = %d\n", connSockfd);
            m_connectionArray[connSockfd] = connSockfd;
            char d[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaa";
            memcpy(m_sendBuff, d, strlen(d));
            printf("m_sendbuff = %s, len = %d \n", m_sendBuff, strlen(m_sendBuff));
            write(connSockfd, m_sendBuff, strlen(m_sendBuff));
        }
    };

    void send(int fd){
        //write(connSockfd, m_sendBuff, strlen(m_sendBuff));
    };

    void closeConnection(int fd){
        
    }
};