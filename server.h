#ifndef AsyncServer_H
#define AsyncServer_H

#include "struct.h"
#include "list"
#include "iostream"
#include "ostream"

using namespace std;

class BaseServer
{
public:
    virtual void Accept() = 0;
    virtual void Read_data(socket_info&) = 0;
    virtual void start() = 0;
    virtual void handle_clients_thread() = 0;
    virtual ~BaseServer(){}
};

class AsyncServer: public BaseServer
{
    bool is_con;
    vector<int> array;
    io_service service;
    ip::tcp::endpoint ep;
    boost::mutex mtx;
    ip::tcp::acceptor acc;
    list<socket_info> list_sockets;
public:
    explicit AsyncServer(int port1);
    void Accept();
    void Read_data(socket_info& item);
    void handle_clients_thread();
    void start();
};

#endif // AsyncServer_H
