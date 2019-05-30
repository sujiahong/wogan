#include <iostream>
#include <netinet/in.h>

class Connection{
private:
    int m_id; /////文件描述符
    char m_sendBuff[2048];
    char m_recvBuff[2048];
    struct sockaddr_in m_cliAddr;
public:
    Connection(int id){
        m_id = id;
    }

    struct sockaddr_in* getCliAddr(){
        return &m_cliAddr;
    }
};