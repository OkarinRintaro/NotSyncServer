#ifndef STRUCT_H
#define STRUCT_H

#include "boost/asio.hpp"
#include "boost/thread.hpp"
#include <vector>

using namespace boost::asio;

#pragma pack(push, 8)
typedef struct{
    unsigned int id;
    int action;
}read_info;
#pragma pack(pop)

struct socket_info
{
    read_info rinf;
    ip::tcp::socket sock;
    socket_info(io_service& serv): sock(serv){}
};

#endif // STRUCT_H
