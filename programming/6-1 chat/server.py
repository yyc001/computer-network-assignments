import socket

import threading
import time

COORDINATOR_PORT = 8000
COORDINATOR_IP = "127.0.0.1"
coordinator_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

class SocketThread(threading.Thread):

    def broadcast(self, msg):
        global_sockets_lock.acquire()
        for client_socket, client_addr in global_sockets:
            # if client_addr != self.client:
            #     client_socket.send(msg)
            client_socket.send(msg)
        global_sockets_lock.release()

    def __init__(self, sock, client):
        super().__init__()
        self.sock = sock
        self.client = client
        global_sockets_lock.acquire()
        global_sockets.append((sock, client))
        global_sockets_lock.release()
        self.broadcast((self.client[0] + " joined in.\n").encode())
    
    def run(self):
        while True:
            try:
                recv_data = self.sock.recv(1024)
                print(self.sock, self.client, recv_data)
                self.broadcast((self.client[0] + ": " + recv_data.decode() + "\n").encode())
            except ConnectionResetError:
                break

        global_sockets_lock.acquire()
        global_sockets.remove((self.sock, self.client))
        if len(global_sockets) == 0:
            coordinator_socket.sendto(b'y', (COORDINATOR_IP, COORDINATOR_PORT))
        global_sockets_lock.release()

        self.broadcast((self.client[0] + " left.\n").encode())
        self.sock.close()


PORT = 7788
global_sockets = []
global_sockets_lock = threading.Lock()

if __name__ == '__main__':
    coordinator_socket.sendto(b'y', (COORDINATOR_IP, COORDINATOR_PORT))
    listen_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    listen_socket.bind(('0.0.0.0', PORT))
    listen_socket.listen()
    print("Listen port", PORT)
    while True:
        client_socket, client_addr = listen_socket.accept()
        SocketThread(client_socket, client_addr).start()
    listen_socket.close()
