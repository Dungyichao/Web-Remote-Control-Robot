import serial
from serial import SerialException
import socket

    
def ini_socket(HOST, portNum):
    try:
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.bind((HOST, portNum))
        sock.setblocking(1)
        sock.listen()
        return True, sock
    except:
        return False, None

def portIsUsable(portName):
    try:
        ser = serial.Serial(portName, 9600, timeout=1)
        return True, ser
    except:
        return False, None
