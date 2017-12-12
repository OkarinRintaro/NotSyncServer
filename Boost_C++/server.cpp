#include "server.h"

AsyncServer::AsyncServer(int port):is_con(true),ep(ip::tcp::v4(),port),acc(service,ep)
{
    array.resize(1);
}


void AsyncServer::Accept()
{
    list_sockets.push_back(socket_info(service));
    socket_info& refitem = list_sockets.back();
    acc.async_accept(refitem.sock,[this,&refitem](const boost::system::error_code& ec)
    {
        if(!ec)
        {
            cout<<"Connect New Client"<<endl;
            Read_data(refitem);
            Accept();
        }
        else
            cout<<"Error code:"<<ec<<endl;
    });
}

void AsyncServer::handle_clients_thread()
{
    service.run();
}


void AsyncServer::start()
{
    Accept();
    auto cores = boost::thread::hardware_concurrency();
    for(unsigned int i=0; i<cores; ++i)
        boost::thread(&AsyncServer::handle_clients_thread,this);
}


void AsyncServer::Read_data(socket_info& item)
{
    async_read(item.sock,buffer((char*)&item.rinf,sizeof(read_info)),[this,&item](const boost::system::error_code& ec,size_t read_bytes)
    {
        if(!ec)
        {
            boost::mutex::scoped_lock(mtx);
            cout<<"Read data size:"<<read_bytes<<"\n";
            cout<<"ID:"<<item.rinf.id<<"   Action:"<<item.rinf.action<<endl;
            if(item.rinf.id>=array.size())
                array.resize(item.rinf.id*2);
            if(item.rinf.action == 1)
                array[item.rinf.id]++;
            if(item.rinf.action == -1)
                array[item.rinf.id]--;
            cout<<"Array["<<item.rinf.id<<"] = "<<array[item.rinf.id]<<endl;
            Read_data(item);
        }
        else
        {
            cout<<"Socket Disconnect"<<"   Error code:"<<ec<<endl;
            auto it = list_sockets.begin();
            while(it != list_sockets.end())
            {
                if(it->sock.remote_endpoint().address().to_string() == item.sock.remote_endpoint().address().to_string())
                {
                    item.sock.close();
                    it = list_sockets.erase(it);
                    return;
                }
                else
                    ++it;
            }
        }
    });
}
