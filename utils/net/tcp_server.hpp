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
    int m_connectionArray[65536]= {0};
    char m_sendBuff[4096];
    char m_recvBuff[4096];
public:
    TCPServer(int port){
        int ret;
        socklen_t len;
        struct sockaddr_in servaddr, cliaddr;
        m_listenSocketfd = socket(AF_INET, SOCK_STREAM, 0);
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        servaddr.sin_port = htons(port);
        bind(m_listenSocketfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
        listen(m_listenSocketfd, 4096);
        printf("server listen success !!\n");
        int connSockfd;
        epollfd = epoll_create1(0);
        struct epoll_event ev, events[10];
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