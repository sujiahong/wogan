#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <aio.h>
#include <unistd.h>
#include <cstring>

void AIOHandler(union sigval sv);

class TCPClient{
private:
    int m_connSockfd;
    char m_readbuf[4096];
    struct aiocb m_acb;
public:
    TCPClient(const char* ip, const int port){
        int ret;
        m_connSockfd = socket(AF_INET, SOCK_STREAM, 0);
        struct sockaddr_in servaddr;
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(port);
        inet_pton(AF_INET, ip, &servaddr.sin_addr);
        if ((ret = connect(m_connSockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))) == 0){
            printf("TCPClient connect success!!!\n");
            ///异步IO
            struct sigevent sev;
            sev.sigev_notify = SIGEV_THREAD;
            sev.sigev_notify_function = &AIOHandler;
            sev.sigev_value.sival_ptr = this;
            sev.sigev_notify_attributes = NULL;
            sev.sigev_signo = -1;
            memset(&m_acb, 0, sizeof(m_acb));
            m_acb.aio_fildes = m_connSockfd;
            m_acb.aio_offset = 0;
            m_acb.aio_nbytes = 4096;
            m_acb.aio_buf = m_readbuf;
            m_acb.aio_sigevent = sev;

            ret = aio_read(&m_acb);
            printf("aio_read ret = %d\n", ret);
            sleep(5);
        }else{
            printf("TCPClient errcode: %d\n", ret);
            disconnect();
        }
    }

    char* get(){
        return m_readbuf;
    }
    struct aiocb* getAIOCB(){
        return &m_acb;
    }
    

    void send(){
        
    }

    void disconnect(){
        close(m_connSockfd);
        m_connSockfd = -1;
    }
};

void AIOHandler(union sigval sv){
    TCPClient* tc = (TCPClient*)sv.sival_ptr;
    int ret = aio_error(tc->getAIOCB());
    printf("aio_error ret = %d\n", ret);
    ret = aio_return(tc->getAIOCB());
    printf("aio_return ret = %d\n", ret);
    printf("read buff = %s\n", tc->get());
}