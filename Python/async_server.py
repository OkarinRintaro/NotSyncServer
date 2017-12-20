import socket
import asyncore
import multiprocessing
import struct
from threading import Thread, Lock


class ReadSocket(asyncore.dispatcher_with_send):

    def __init__(self, sock, ref_server):
        asyncore.dispatcher_with_send.__init__(self, sock)
        self.__server = ref_server

    def handle_read(self):
        data = self.recv(8)
        id_array, action = struct.unpack('Ii', data)
        self.__server.change_array(id_array, action)


class AsyncServer(asyncore.dispatcher):
    __mutex = Lock()

    def __init__(self, port):
        asyncore.dispatcher.__init__(self)
        self.__array = []
        self.create_socket(socket.AF_INET, socket.SOCK_STREAM)
        self.set_reuse_addr()
        self.bind(('0.0.0.0', port))
        self.listen(5)
        self.__cores = multiprocessing.cpu_count()

    def handle_accept(self):
        pair = self.accept()
        if pair is not None:
            sock, address = pair
            print('Connect New Client', address)
            if self.__cores > 0:
                Thread(target=asyncore.loop, args=())
                self.__cores -= 1
            ReadSocket(sock, self)
        else:
            print('Connect refused!')

    def change_array(self, id_array, action):
        self.__mutex.acquire()
        print('ID: ', id_array, ' Action: ', action)
        if len(self.__array) < id_array:
            self.__array += [0 for _ in range(id_array*2)]
        if action == 1:
            self.__array[id_array] += 1
        if action == -1:
            self.__array[id_array] -= 1
        print('ID: ', id_array, ' Result: ', self.__array[id_array])
        self.__mutex.release()


port_ = int(input('Enter port: '))
server = AsyncServer(port_)
asyncore.loop()
