# Web Remote Control Robot
In this tutorial, we will show you how to control your robot manipulator through web browser. <br />

# 1. What is the Goal and what you need to have <br />
Let's first take a look what are we going to achieve after this tutorial.<br />

https://user-images.githubusercontent.com/25232370/156177097-bd422ee5-63b7-4501-8a54-8ba6f52f8c33.mp4



## 1.1 Hardware and Documents you need to have <br />
Hardware:
* Tinkerkit Braccio Robot
* Arduino UNO Rev3
* Raspberry Pi 3 Model B+
* Raspberry Pi Camera Module 2 (or other WebCam)
* USB 2.0 Cable Type A/B

Documents:
The following codes can be downloaded from this repository Code folder.
<p align="center">
<table>
    <thead>
        <tr>
            <th align="center">File Name</th>
            <th align="center">Language</th>
            <th align="center">Function/Support</th>
            <th align="center">Purpose</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td align="center">http_server.c</td>
            <td align="center">C</td>
            <td align="center">HTTP Web Server</td>
            <td align="left">Web server, serving the web page to client web browser</td>
        </tr>
        <tr>
            <td align="center">mjpeg_server.c</td>
            <td align="center">C</td>
            <td align="center">Streaming Server</td>
            <td align="left">Streaming server, serving MJPEG to client web browser</td>
        </tr>    
        <tr>
            <td align="center">picam.py</td>
            <td align="center">Python</td>
            <td align="center">Streaming Server</td>
            <td align="left">Communicate with Raspberry Pi Camera Module, output to mjpeg_server.c</td>
        </tr>  
        <tr>
            <td align="center">robot_server.py</td>
            <td align="center">Python</td>
            <td align="center">Robot Server</td>
            <td align="left">Listen from http_server.c. Output command to Arduino UNO through serial. Respond to http_server.c. </td>
        </tr>  
        <tr>
            <td align="center">utility.py</td>
            <td align="center">Python</td>
            <td align="center">Robot Server</td>
            <td align="left">Some function for robot_server.py</td>
        </tr>  
        <tr>
            <td align="center">arduino_braccio.ino</td>
            <td align="center">C</td>
            <td align="center">Robot Control</td>
            <td align="left">Listen from robot_server.py through serial. Output to Tinkerkit Braccio Robot. Respond motor status to robot_server.py. This programming is uploaded to Arduino </td>
        </tr>
        <tr>
            <td align="center">index.html</td>
            <td align="center">HTML</td>
            <td align="center">Web Page</td>
            <td align="left">Web pages</td>
        </tr>
        <tr>
            <td align="center">mystyle.css</td>
            <td align="center">CSS</td>
            <td align="center">Web Page</td>
            <td align="left">Web pages style</td>
        </tr>
        <tr>
            <td align="center">robot.js</td>
            <td align="center">Javascript</td>
            <td align="center">Web Page</td>
            <td align="left">Web pages functions</td>
        </tr>
    </tbody>
</table>
</p>


# 2. System Overview
Please review the following system structure of our remote control robot design <br />

## 2.1 Concept and System Structure
<p align="center">
<img src="/image/system_structure.png" height="100%" width="100%">
</p>

## 2.2 Hardware Connection
<p align="center">
<img src="/image/connection.png" height="90%" width="90%">
</p>

## 2.3 Software Compile and Execute
We put the Code folder (provided in this repository) under Raspberry pi ```/home/pi/Desktop```. The command to run Http Server, Streaming Server, Robot Server are in the following image.
<p align="center">
<img src="/Code/Command_and_result.JPG" height="90%" width="90%">
</p>
The Arduino folder is for Arduino, not for Raspberry pi

