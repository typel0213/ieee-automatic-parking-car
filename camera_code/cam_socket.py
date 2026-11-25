import camera_vision
import socket
import threading

# Global Variables
HEADER = 64 # how long the first message sent must be
PORT = 5050 # generic port
SERVER = "insert server ip address here"
ADDR = (SERVER, PORT)
FORMAT = 'utf-8'
DISCONNECT_MSG = "[CAM_DISCONNECT]"
INITIALIZE_MSG = "[CAM_INITIALIZED]"

#note: this file contains only the functions that would send the socket msgs
"""
TODO:
    make this into a class and import into camera main code ?
    figure out how the server is going to interpret this
    create proper init and disconnect functions (not just send strings, close)

"""

# creates a socket that opens camera up to internet connection
client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect(ADDR)

def send(msg):
    message = msg.encode(FORMAT)
    msg_length = len(message)

    # message needs to be a specific length to avoid errors, so pad with ' '
    send_length = str(msg_length).encode(FORMAT)
    send_length += b' ' * (HEADER - len(send_length))

    # temp format for now
    client.send(send_length)
    client.send(message)

    # if feedback is needed, this should store the feedback msg from server
    # feedback = client.recv(2048).decode(FORMAT)
    # return feedback


def send_initialized():
    send(INITIALIZE_MSG)

def send_disconnect():
    send(DISCONNECT_MSG)