# -*- coding: utf-8 -*-

import socket
import time
PORT = 8000
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(("0.0.0.0", PORT))

server_free = [
    ("127.0.0.1", 7788),
    ]

server_work = {}

while True:
    receive_data, client = sock.recvfrom(32)
    receive_data = receive_data.decode()
    print(receive_data, client)
    pidx = None
    for idx, server in server_work.items():
        if server[0] == client[0] and receive_data == "y"  : # 如果 server 和 client 不跑在同一个IP地址下可以把 receive_data == "y" 去掉
            pidx = idx
            server_free.append(server)
            break

    if pidx:
        server_work.pop(pidx)
        continue

    if receive_data not in server_work:
        if len(server_free) == 0:
            sock.sendto(b'n', client)
            continue
        else:
            server_work[receive_data] = server_free[0]
            server_free = server_free[1:]
    sock.sendto(str(server_work[receive_data]).encode(), client)
    print(server_work)

