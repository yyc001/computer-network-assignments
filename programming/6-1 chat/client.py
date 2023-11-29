# -*- coding: utf-8 -*-

import socket
import time
import tkinter as tk
import tkinter.scrolledtext as tst
import time
import tkinter.messagebox
import threading

class Application(tk.Frame):
    def __init__(self, master, server_socket):
        tk.Frame.__init__(self,master)
        self.grid()
        self.server_socket = server_socket
        self.createWidgets()

    def createWidgets(self):
        self.textEdit=tst.ScrolledText(self,width=50,height=15)
        self.textEdit.grid(row=0,column=0,rowspan=1,columnspan=4)
        self.textEdit.config(state='disabled')

        self.inputText = tk.Text(self,width=40,height=5)
        self.inputText.grid(row=1,column=0,columnspan=1)

        self.btnSend = tk.Button(self,text='send',command=self.textSend)
        self.btnSend.grid(row=1,column=3)
        t=threading.Thread(target=self.getInfo)
        t.start()


    def textSend(self):
        strstr=self.inputText.get('1.0','end-1c')
        if strstr:
            self.inputText.delete(0.0,tk.END)
            sendMessage=bytes(strstr,encoding='utf8')
            self.server_socket.send(sendMessage)

    def getInfo(self):
        while True:
            recMessage = self.server_socket.recv(1024).decode()+'\n'
            self.textEdit.config(state='normal')
            self.textEdit.insert(tk.END,recMessage)
            self.textEdit.see(tk.END)
            self.textEdit.config(state='disabled')

COORDINATOR_PORT = 8000
COORDINATOR_IP = "127.0.0.1"

coordinator_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
room = input("Room name:")
coordinator_socket.sendto(room.encode(), (COORDINATOR_IP, COORDINATOR_PORT))
server_data, _ = coordinator_socket.recvfrom(1024)
server_data = server_data.decode()
if server_data == 'n':
    print("Server busy.")
    exit(0)
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_addr = eval(server_data)
print("Server:", server_addr)
server_socket.connect(server_addr)

root=tk.Tk()
root.title('client')
app = Application(master=root, server_socket=server_socket)
app.mainloop()
# while True:
#     send_data = input(">>>")
#     if send_data == "exit":
#         break
#     server_socket.send(send_data.encode())
#     recv_data = server_socket.recv(1024)
#     print(recv_data.decode())
# server_socket.close()


