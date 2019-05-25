#include <iostream>
#include <cstdio>
#include <sys/socket.h>
#include <netinet/in.h>

#include "connection.cpp"

class TCPServer{
private:
    int m_listenSocketfd;
    int m_connectionArray[65536]= {0};
    char m_sendBuff[2048];
    char m_recvBuff[2048];
public:
    TCPServer(int port){
        socklen_t len;
        struct sockaddr_in servaddr, cliaddr;
        m_listenSocketfd = socket(AF_INET, SOCK_STREAM, 0);
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        servaddr.sin_port = htons(port);
        bind(m_listenSocketfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
        listen(m_listenSocketfd, 4096);
        int connSockfd;
        while(1){
            len = sizeof(cliaddr);
            connSockfd = accept(m_listenSocketfd, (struct sockaddr*)&cliaddr, &len);
            printf("connfd = %d\n", connSockfd);
            m_connectionArray[connSockfd] = connSockfd;
        }
    };

    send(){

    };

    close(){

    }
};