# Web Remote Control Robot
In this tutorial, we will show you how to control your robot arm through web browser. <br />

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

Documents

<p align="center">
<table>
    <thead>
        <tr>
            <th align="center">File Name</th>
            <th align="center">Language</th>
            <th align="center">Purpose</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td align="center">http_server.c</td>
            <td align="center">C</td>
            <td align="left">Web server, serving the web page to client web browser</td>
        </tr>
        <tr>
            <td align="center">mjpeg_server.c</td>
            <td align="center">C</td>
            <td align="left">Streaming server, serving MJPEG to client web browser</td>
        </tr>    
        <tr>
            <td align="center">mjpeg_server.py</td>
            <td align="center">Python</td>
            <td align="left">Communicate with Raspberry Pi Camera Module, output to mjpeg_server.c</td>
        </tr>  
        <tr>
            <td align="center">robot_server.py</td>
            <td align="center">Python</td>
            <td align="left">Listen from http_server.c. Output command to Arduino UNO through serial. Respond to http_server.c. </td>
        </tr>  
        <tr>
            <td align="center">utility.py</td>
            <td align="center">Python</td>
            <td align="left">Some function for robot_server.py</td>
        </tr>  
        <tr>
            <td align="center">arduino_braccio.ino</td>
            <td align="center">C</td>
            <td align="left">Listen from robot_server.py through serial. Output to Tinkerkit Braccio Robot. Respond to robot_server.py. </td>
        </tr>
        <tr>
            <td align="center">Web files</td>
            <td align="center">html, css, js</td>
            <td align="left">Web pages </td>
        </tr>
    </tbody>
</table>
</p>


# 2. System Design
Please review the following system structure of our remote control robot design <br />
<p align="center">
<img src="/image/system_structure.png" height="100%" width="100%">
</p>

