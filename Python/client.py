import socket
import struct

# Part of Cosmos by OpenGenus Foundation


adr = input('Enter ip address of server:')
port = int(input('Enter port of server:'))
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
is_con = 1
try:
    sock.connect((adr, port))
    print('connection accepted')
except:
    print('connection refused')
    is_con = 0
while is_con:
    id_ = int(input('Enter array id: '))
    action = 0
    while True:
        action = int(input('Enter action: '))
        if action == 1 or action == -1:
            break
        else:
            print('Not correct action')
    data = struct.pack('Ii', id_, action)
    try:
        sock.send(data)
    except:
        print('Disconnect!')
        is_con = 0
