import socket
import asyncore
import multiprocessing
import struct
from threading import Thread, Lock


class ReadSocket(asyncore.dispatcher_with_send):

    def __init__(self, sock, ref_serv):
        asyncore.dispatcher_with_send.__init__(self, sock)
        self.server = ref_serv

    def handle_read(self):
        data = self.recv(8)
        id_array, action = struct.unpack('Ii', data)
        self.serv.change_array(id_array, action)


class AsyncServer(asyncore.dispatcher):
    array = []
    mutex = Lock()

    def __init__(self, port):
        asyncore.dispatcher.__init__(self)
        self.create_socket(socket.AF_INET, socket.SOCK_STREAM)
        self.set_reuse_addr()
        self.bind(('0.0.0.0', port))
        self.listen(5)
        self.cores = multiprocessing.cpu_count()

    def handle_accept(self):
        pair = self.accept()
        if pair is not None:
            sock, addr = pair
            print('Connect New Client', addr)
            if self.cores > 0:
                Thread(target=asyncore.loop, args=())
                self.cores -= 1
            ReadSocket(sock, self)
        else:
            print('Connect refused!')

    def change_array(self, id_array, action):
        self.mutex.acquire()
        print('ID: ', id_array, ' Action: ', action)
        if len(self.array) < id_array:
            self.array += [0 for _ in range(id_array*2)]
        if action == 1:
            self.array[id_array] += 1
        if action == -1:
            self.array[id_array] -= 1
        print('ID: ', id_array, ' Result: ', self.array[id_array])
        self.mutex.release()


port_ = int(input('Enter port: '))
server = AsyncServer(port_)
asyncore.loop()
