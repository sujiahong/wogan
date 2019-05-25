#include <iostream>

class Connection{
private:
    int m_id;
    char m_sendBuff[2048];
    char m_recvBuff[2048];

public:
    Connection(int id){
        m_id = id;
    }
};