#include <cstdio>

#include "../utils/net/tcp_client.hpp"


int main(){
    TCPClient cli = TCPClient("127.0.0.1", 7701);
}