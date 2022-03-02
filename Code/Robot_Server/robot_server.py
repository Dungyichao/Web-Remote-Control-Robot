#import serial
#from serial import SerialException
#from utility import *
import utility 
import time


PORT = 8082
HOST = "127.0.0.1"  #listen on local host
TimeOutUSBResponse = 3
TimeOutSocketGET = 10

CanOpenUSBPort, USBPort_Obj = utility.portIsUsable('/dev/ttyACM0')
CanOpenSocket, Socket_Obj = utility.ini_socket(HOST, PORT)

if CanOpenUSBPort and CanOpenSocket:
    while True:
        if CanOpenSocket:
            print('Socket start listening -------------------- \n')
            conn, addr = Socket_Obj.accept()
            with conn:
                print(f"Connected by {addr}")
                timeout = time.time() + TimeOutSocketGET
                while time.time() < timeout:
                    data = conn.recv(1024)
                    if not data:
                        break
                    else:
                        print('web data: ', data)
                        print('write start >> \n')
                        USBPort_Obj.write(data + "\n".encode('ascii'))
                        #conn.sendall(b'987')
                        break                  
                    #conn.sendall(data)
                if USBPort_Obj.in_waiting > 0 or True:
                    timeout = time.time() + TimeOutUSBResponse
                    while time.time() < timeout:
                        print('read start >> \n')
                        line = USBPort_Obj.readline()#.decode('utf-8').rstrip()
                        print(line)
                        if line.decode('utf-8')[0:7] == 'Arduino':
                            print('Received Message End --> Break')
                            conn.sendall(line)
                            break;
                        time.sleep(1)
                    #print('Time out -- \n')
                
else:
    print ("No Arduino being detected")
