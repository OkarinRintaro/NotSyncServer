#include "server.h"
#include <memory>

using namespace std;

int main()
{
    int port = 44011;
    cout<<"Enter port of server:"<<endl;
    cin>>port;
    shared_ptr<BaseServer> ptr_server(new AsyncServer(port));
    ptr_server->start();
    ptr_server->handle_clients_thread();
    return 0;
}
