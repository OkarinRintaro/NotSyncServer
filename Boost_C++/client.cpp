#include <iostream>
#include "boost/asio.hpp"

using namespace std;
using namespace boost::asio;

#pragma pack(push, 8)
typedef struct{
    unsigned int id;
    int action;
}write_info;
#pragma pack(pop)


int main()
{
    string str;
    write_info winfo;
    int port = 44011;
    cout<<"Enter ip adress of server:"<<endl;
    cin>>str;
    cout<<"Enter port of server:"<<endl;
    cin>>port;
    io_service service;
    ip::tcp::endpoint ep(ip::address::from_string(str),port);
    ip::tcp::socket sock(service);
    boost::system::error_code ec;
    sock.connect(ep,ec);
    if(ec)
    {
        cout<<"Error_code:"<<ec<<endl;
        return 0;
    }
    bool is_con = true;
    while(is_con)
    {
        cout<<"Enter array id:"<<endl;
        cin>>winfo.id;
        winfo.action = 0;
        while(true)
        {
            cout<<"Enter action:"<<endl;
            cin>>winfo.action;
            if(winfo.action == 1 || winfo.action == -1)
                break;
            else
                cout<<"Not correct action"<<endl;
        }
        sock.write_some(buffer((char*)&winfo,sizeof(write_info)),ec);
        if(ec)
        {
            cout<<"Error_code:"<<ec<<endl;
            is_con = false;
        }
    }
    return 0;
}
