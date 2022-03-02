// Server side C program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <errno.h>

#include <netdb.h> 


//#include <fstream>
//#include <sstream>
//#include <arpa/inet.h>
//#include <string>
//#include <netdb.h>
//#include <unistd.h>
//using namespace std;

#define PORT 8086
#define PORT_ROBOT 8082

char* parse(char line[], const char symbol[]);
char* parse_method(char line[], const char symbol[]);
char* find_token(char line[], const char symbol[], const char match[]);
int send_message(int fd, char image_path[], char head[]);
int send_robot(char *argv[], char message[], char* robot_response[]);

int sendfile_bin(int fd, FILE *f);
int sendlong(int fd, long value);
int senddata(int fd, void *buf, int buflen);
//void setHttpHeader_other(char httpHeader[], char *path);
//void setHttpHeader(char httpHeader[]);
//void report(struct sockaddr_in *serverAddress);
/*std::string read_image(const std::string& image_path);
int send_image(int & fd, std::string& image);*/

//https://developer.mozilla.org/en-US/docs/Web/HTTP/Basics_of_HTTP/MIME_types/Common_types
//https://stackoverflow.com/questions/45670369/c-web-server-image-not-showing-up-on-browser

char http_header[25] = "HTTP/1.1 200 Ok\r\n";

void error(const char *msg)
{
    perror(msg);
    //exit(1);
}

int main(int argc, char const *argv[])
{
    int server_fd, new_socket, pid; 
    long valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("In sockets");
        exit(EXIT_FAILURE);
    }
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
    
    memset(address.sin_zero, '\0', sizeof address.sin_zero);
    
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }
    
    //report(&address);
    //setHttpHeader(httpHeader);
    
    while(1)
    {
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }
        
        //Create child process to handle request from different client
        pid = fork();
        if(pid < 0){
            perror("Error on fork");
            exit(EXIT_FAILURE);
        }
        if(pid == 0){
            //close(server_fd);
            char buffer[30000] = {0};
            valread = read( new_socket , buffer, 30000);

            //printf("\n buffer message: %s \n ", buffer);
            char *parse_string_method = parse_method(buffer, " ");  //Try to get the path which the client ask for
            //printf("Client method: %s\n", parse_string_method);
            
            //char httpHeader1[800021] = "HTTP/1.1 200 OK\r\n\n";

            char *parse_string = parse(buffer, " ");  //Try to get the path which the client ask for
            printf("Client ask for path: %s\n", parse_string);

            //prevent strtok from changing the string
            //https://wiki.sei.cmu.edu/confluence/display/c/STR06-C.+Do+not+assume+that+strtok%28%29+leaves+the+parse+string+unchanged
            //https://stackoverflow.com/questions/5099669/invalid-conversion-from-void-to-char-when-using-malloc/5099675 
            char *copy = (char *)malloc(strlen(parse_string) + 1);
            strcpy(copy, parse_string);
            char *parse_ext = parse(copy, ".");  // get the file extension such as JPG, jpg
            char *copy_head = (char *)malloc(strlen(http_header) +200);
            char *copy_head1 = (char *)malloc(strlen(http_header) +200);
            strcpy(copy_head, http_header);
            
            if(parse_string_method[0] == 'G' && parse_string_method[1] == 'E' && parse_string_method[2] == 'T'){
                //https://developer.mozilla.org/en-US/docs/Web/HTTP/Basics_of_HTTP/MIME_types/Common_types
                if(strlen(parse_string) <= 1){
                    //case that the parse_string = "/"  --> Send index.html file
                    //write(new_socket , httpHeader , strlen(httpHeader));
                    char path_head[500] = ".";
                    strcat(path_head, "/index.html");
                    strcat(copy_head, "Content-Type: text/html\r\n\r\n");
                    send_message(new_socket, path_head, copy_head);
                }
                else if ((parse_ext[0] == 'j' && parse_ext[1] == 'p' && parse_ext[2] == 'g') || (parse_ext[0] == 'J' && parse_ext[1] == 'P' && parse_ext[2] == 'G'))
                {
                    //send image to client
                    char path_head[500] = ".";
                    strcat(path_head, parse_string);
                    strcat(copy_head, "Content-Type: image/jpeg\r\n\r\n");
                    send_message(new_socket, path_head, copy_head);
                }
                else if ((parse_ext[0] == 'm' && parse_ext[1] == 'j' && parse_ext[2] == 'p' && parse_ext[3] == 'e' && parse_ext[4] == 'g') || (parse_ext[0] == 'M' &&  parse_ext[1] == 'J' && parse_ext[2] == 'P' && parse_ext[3] == 'E' && parse_ext[4] == 'G'))
                {
                    //send image to client
                    char path_head[500] = ".";
                    strcat(path_head, parse_string);
                    //strcat(copy_head, "Content-Type: multipart/x-mixed-replace; boundary=FRAME\r\n\r\n");
                    
                    strcat(copy_head, "Content-Type: multipart/x-mixed-replace; boundary=FRAME \r\n");
                    strcat(copy_head, "Cache-Control: no-cache, private \r\n");
                    strcat(copy_head, "Pragma: no-cache \r\n\r\n");
                    write(new_socket, copy_head, strlen(copy_head));
                    //send_message(new_socket, "", copy_head);
                    
                    //strcat(copy_head1, "--FRAME\r\n");
                    //write(new_socket, copy_head1, strlen(copy_head1));
                    //senddata(new_socket, copy_head1, strlen(copy_head1));
                    
                    strcat(path_head, "/img/p1.JPG");
                    strcat(copy_head1, "Content-Type: image/jpeg\r\n");
                    send_message(new_socket, path_head, copy_head1);                    
                }
                else if (parse_ext[0] == 'i' && parse_ext[1] == 'c' && parse_ext[2] == 'o')
                {
                    //https://www.cisco.com/c/en/us/support/docs/security/web-security-appliance/117995-qna-wsa-00.html
                    char path_head[500] = ".";
                    strcat(path_head, "/img/favicon.png");
                    strcat(copy_head, "Content-Type: image/vnd.microsoft.icon\r\n\r\n");
                    send_message(new_socket, path_head, copy_head);
                }
                else if (parse_ext[0] == 't' && parse_ext[1] == 't' && parse_ext[2] == 'f')
                {
                    //font type, to display icon from FontAwesome
                    char path_head[500] = ".";
                    strcat(path_head, parse_string);
                    strcat(copy_head, "Content-Type: font/ttf\r\n\r\n");
                    send_message(new_socket, path_head, copy_head);
                }
                else if (parse_ext[strlen(parse_ext)-2] == 'j' && parse_ext[strlen(parse_ext)-1] == 's')
                {
                    //javascript
                    char path_head[500] = ".";
                    strcat(path_head, parse_string);
                    strcat(copy_head, "Content-Type: text/javascript\r\n\r\n");
                    send_message(new_socket, path_head, copy_head);
                }
                else if (parse_ext[strlen(parse_ext)-3] == 'c' && parse_ext[strlen(parse_ext)-2] == 's' && parse_ext[strlen(parse_ext)-1] == 's')
                {
                    //css
                    char path_head[500] = ".";
                    strcat(path_head, parse_string);
                    strcat(copy_head, "Content-Type: text/css\r\n\r\n");
                    send_message(new_socket, path_head, copy_head);
                }
                else if (parse_ext[0] == 'w' && parse_ext[1] == 'o' && parse_ext[2] == 'f')
                {
                    //Web Open Font Format woff and woff2
                    char path_head[500] = ".";
                    strcat(path_head, parse_string);
                    strcat(copy_head, "Content-Type: font/woff\r\n\r\n");
                    send_message(new_socket, path_head, copy_head);
                }
                else if (parse_ext[0] == 'm' && parse_ext[1] == '3' && parse_ext[2] == 'u' && parse_ext[3] == '8')
                {
                    //Web Open m3u8
                    char path_head[500] = ".";
                    strcat(path_head, parse_string);
                    strcat(copy_head, "Content-Type: application/vnd.apple.mpegurl\r\n\r\n");
                    send_message(new_socket, path_head, copy_head);
                }
                else if (parse_ext[0] == 't' && parse_ext[1] == 's')
                {
                    //Web Open ts
                    char path_head[500] = ".";
                    strcat(path_head, parse_string);
                    strcat(copy_head, "Content-Type: video/mp2t\r\n\r\n");
                    send_message(new_socket, path_head, copy_head);
                }
                else{
                    //send other file 
                    char *find_string = malloc(200);
                    find_string = find_token(buffer, "\r\n GET", "/action?go");
                    if(find_string != NULL){
                        int send_robot_result = -1;
                        char *parse_string = parse(buffer, "="); 
                        char robot_response[50];
                        printf("parse string for action: %s \n", parse_string);
                        
                        if (strcmp(parse_string, "motor1+") == 0){
                            send_robot_result = send_robot("127.0.0.1", "101", &robot_response);  
                        }
                        else if (strcmp(parse_string, "motor2+") == 0){
                            send_robot_result = send_robot("127.0.0.1", "102", &robot_response);  
                        }
                        else if (strcmp(parse_string, "motor3+") == 0){
                            send_robot_result = send_robot("127.0.0.1", "103", &robot_response);  
                        }
                        else if (strcmp(parse_string, "motor4+") == 0){
                            send_robot_result = send_robot("127.0.0.1", "104", &robot_response);  
                        }
                        else if (strcmp(parse_string, "motor5+") == 0){
                            send_robot_result = send_robot("127.0.0.1", "105", &robot_response);  
                        }
                        else if (strcmp(parse_string, "motor6+") == 0){
                            send_robot_result = send_robot("127.0.0.1", "106", &robot_response);  
                        }
                        else if (strcmp(parse_string, "motor1-") == 0){
                            send_robot_result = send_robot("127.0.0.1", "201", &robot_response);  
                        }
                        else if (strcmp(parse_string, "motor2-") == 0){
                            send_robot_result = send_robot("127.0.0.1", "202", &robot_response);  
                        }
                        else if (strcmp(parse_string, "motor3-") == 0){
                            send_robot_result = send_robot("127.0.0.1", "203", &robot_response);  
                        }
                        else if (strcmp(parse_string, "motor4-") == 0){
                            send_robot_result = send_robot("127.0.0.1", "204", &robot_response);  
                        }
                        else if (strcmp(parse_string, "motor5-") == 0){
                            send_robot_result = send_robot("127.0.0.1", "205", &robot_response);  
                        }
                        else if (strcmp(parse_string, "motor6-") == 0){
                            send_robot_result = send_robot("127.0.0.1", "206", &robot_response);  
                        }
                        else if (strcmp(parse_string, "reset_robot") == 0){
                            send_robot_result = send_robot("127.0.0.1", "300", &robot_response);  
                        }
                        else if (strcmp(parse_string, "refresh_robot") == 0){
                            send_robot_result = send_robot("127.0.0.1", "301", &robot_response);  
                        }
                        else if (strcmp(parse_string, "step_size+") == 0){
                            send_robot_result = send_robot("127.0.0.1", "302", &robot_response);  
                        }
                        else if (strcmp(parse_string, "step_size-") == 0){
                            send_robot_result = send_robot("127.0.0.1", "303", &robot_response);  
                        }
                        else{
                            send_robot_result = send_robot("127.0.0.1", "100", &robot_response);  
                        }
                        
                        if(send_robot_result == 0){
                            printf("Send command to robot successful \n");
                        }
                        else{
                            printf("Send command to robot failed \n");
                        }              
                            
                        //printf("Going to send browser robot response: %s \n", robot_response);  
                        
                        strcat(copy_head, "Content-Type: text/plain \r\n\r\n"); //\r\n\r\n   
                        strcat(copy_head, robot_response);
                        write(new_socket, copy_head, strlen(copy_head));
                        
                    }
                }
                printf("\n------------------Server sent----------------------------------------------------\n");
            }
            else if (parse_string_method[0] == 'P' && parse_string_method[1] == 'O' && parse_string_method[2] == 'S' && parse_string_method[3] == 'T'){
                char *find_string = malloc(200);
                find_string = find_token(buffer, "\r\n", "action");
                strcat(copy_head, "Content-Type: text/plain \r\n\r\n"); //\r\n\r\n
                //strcat(copy_head, "Content-Length: 12 \n");
                strcat(copy_head, "User Action: ");
                printf("find string: %s \n", find_string);
                strcat(copy_head, find_string);
                write(new_socket, copy_head, strlen(copy_head));
                
            }
            //close(server_fd);
            close(new_socket);
            free(copy);
            free(copy_head);  
            free(copy_head1); 
        }
        else{
            printf(">>>>>>>>>>Parent create child with pid: %d <<<<<<<<<", pid);
            close(new_socket);
        }
    }
    close(server_fd);
    return 0;
}

char* parse(char line[], const char symbol[])
{
    char *copy = (char *)malloc(strlen(line) + 1);
    strcpy(copy, line);
    
    char *message;
    char * token = strtok(copy, symbol);
    int current = 0;
    while( token != NULL ) {
      token = strtok(NULL, " ");
      if(current == 0){
          message = token;
          if(message == NULL){
              message = "";
          }
          return message;
      }
      current = current + 1;
   }
   free(token);
   free(copy);
   message="";
   return message;
}

char* parse_method(char line[], const char symbol[])
{
    char *copy = (char *)malloc(strlen(line) + 1);
    strcpy(copy, line);
        
    char *message;
    char * token = strtok(copy, symbol);
    int current = 0;

    while( token != NULL ) {
      
      //token = strtok(NULL, " ");
      if(current == 0){
          message = token;
          if(message == NULL){
              message = "";
          }
          return message;
      }
      current = current + 1;
   }
   free(copy);
   free(token);
   return message;
}

char* find_token(char line[], const char symbol[], const char match[])
{
    char *copy = (char *)malloc(strlen(line) + 1);
    strcpy(copy, line);
        
    char *message;
    char * token = strtok(copy, symbol);

    while( token != NULL ) {
      
      printf("--Token: %s \n", token);
      
      if(strlen(match) <= strlen(token))
      {
          int match_char = 0;
          for(int i = 0; i < strlen(match); i++)
          {
              if(token[i] == match[i])
              {
                  match_char++;
              }
          }
          if(match_char == strlen(match)){
            message = token;
            return message;
          }
      }      
      token = strtok(NULL, symbol);
   }
   free(copy);
   free(token);
   message = "";
   return message;
}



//https://stackoverflow.com/questions/45670369/c-web-server-image-not-showing-up-on-browser
//http://www.tldp.org/LDP/LG/issue91/tranter.html
//https://linux.die.net/man/2/fstat
//http://man7.org/linux/man-pages/man2/stat.2.html
//http://man7.org/linux/man-pages/man2/sendfile.2.html
int send_message(int fd, char image_path[], char head[]){
    /*
    char imageheader[] = 
    "HTTP/1.1 200 Ok\r\n"
    "Content-Type: image/jpeg\r\n\r\n";
    */
    struct stat stat_buf;  /* hold information about input file */

    write(fd, head, strlen(head));
    printf("response header: %s", head);

    int fdimg = open(image_path, O_RDONLY);
    
    if(fdimg < 0){
        printf("Cannot Open file path : %s with error %d\n", image_path, fdimg); 
        return 0;
    }
     
    fstat(fdimg, &stat_buf);
    int img_total_size = stat_buf.st_size;
    int block_size = stat_buf.st_blksize;
    //printf("image block size: %d\n", stat_buf.st_blksize);  
    //printf("image total byte st_size: %d\n", stat_buf.st_size);
    if(fdimg >= 0){
        ssize_t sent_size;

        while(img_total_size > 0){
            //if(img_total_size < block_size){
             //   sent_size = sendfile(fd, fdimg, NULL, img_total_size);
            //}
            //else{
            //    sent_size = sendfile(fd, fdimg, NULL, block_size);
            //}          
            //img_total_size = img_total_size - sent_size;
        
            //if(sent_size < 0){
             //   printf("send file error --> file: %d, send size: %d , error: %s\n", fdimg, sent_size, strerror(errno));
             //   img_total_size = -1;
              int send_bytes = ((img_total_size < block_size) ? img_total_size : block_size);
              int done_bytes = sendfile(fd, fdimg, NULL, block_size);
              img_total_size = img_total_size - done_bytes;
            //}
        }
        if(sent_size >= 0){
            printf("send file: %s \n" , image_path);
        }
        close(fdimg);
    }
}

int send_robot(char *argv[], char message[], char* robot_response[]){
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    struct timeval tv;
    
    tv.tv_sec = 2;  //timeout 10 sec for socket connect
    tv.tv_usec = 0;

    char buffer[256];
    //portno = atoi(argv[2]);
    portno = PORT_ROBOT;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0){ 
        error("ERROR opening socket");
        return 1;
    }
    //we set timeout for socket connect to prevent python robot server not responding
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(struct timeval));
    server = gethostbyname(argv);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        return 1;
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){ 
        error("ERROR connecting");
        return 1;
    }
    n = write(sockfd,message,strlen(message));
    if (n < 0){ 
         error("ERROR writing to socket");
         return 1;
     }
    bzero(buffer,256);
    n = read(sockfd,buffer,255);
    if (n < 0){ 
         error("ERROR reading from socket");
         return 1;
     }
    printf("Read from robot: %s\n",buffer);
    //*robot_response = buffer;
    strcpy(robot_response, buffer);
    //write(sockfd, http_header, strlen(http_header));
    //char messages[15] = "16987";
    //write(sockfd, messages, strlen(messages));
    close(sockfd);
    return 0;
}

int senddata(int fd, void *buf, int buflen)
{
    unsigned char *pbuf = (unsigned char *) buf;

    while (buflen > 0)
    {
        int num = send(fd, pbuf, buflen, 0);
        printf("senddata num: %d", num);
        if (num < 0)
        {
            
            return 0;
        }

        pbuf += num;
        buflen -= num;
    }

    return 1;
}

int sendlong(int fd, long value)
{
    value = htonl(value);
    return senddata(fd, &value, sizeof(value));
}

int sendfile_bin(int fd, FILE *f)
{
    fseek(f, 0, SEEK_END);
    long filesize = ftell(f);
    rewind(f);
    if (filesize == EOF)
        return 0;
    if (!sendlong(fd, filesize))
        return 0;
    if (filesize > 0)
    {
        char buffer[1024];
        do
        {
            size_t num = filesize > sizeof(buffer) ? sizeof(buffer) : filesize;
            num = fread(buffer, 1, num, f);
            if (num < 1)
                return 0;
            if (senddata(fd, buffer, num) == 0)
                return 0;
            filesize -= num;
        }
        while (filesize > 0);
    }
    return 1;
}

/*
void setHttpHeader(char httpHeader[])
{   
    // File object to return
    
    FILE *htmlData = fopen("index.html", "r");

    char line[100];
    char responseData[1000000];
    if(htmlData){
        while (fgets(line, 100, htmlData) != 0 ) {
            strcat(responseData, line);
        }
        // char httpHeader[8000] = "HTTP/1.1 200 OK\r\n\n";
        strcat(httpHeader, responseData);
        fclose(htmlData);
    }
    else
    {
        printf("\n Read index.html file problem");
    }
}
*/


/*
void setHttpHeader_other(char httpHeader[], char *path)
{  
    // File object to return
    
    char path_head[500] = ".";
    strcat(path_head, path);
    printf("\n path head : %s", path_head);
    //printf("\n Length of httpheader: %d", strlen(httpHeader));
    
    FILE *htmlData1 = fopen(path_head, "r");

    ////char httpHeader1[8000] = "HTTP/1.1 200 OK\r\n\n";
    int size_data = 800000;
    char line[100];

    char *responseData;
    responseData = (char*)malloc(size_data * sizeof(char));  
    //https://stackoverflow.com/questions/5099669/invalid-conversion-from-void-to-char-when-using-malloc/5099675 

    if(htmlData1){
        
        while (fgets(line, 100, htmlData1) != 0 ) {
            strcat(responseData, line);
        }
        strcat(httpHeader, responseData);
        fclose(htmlData1);
        //free(responseData);      
    }
    else
    {
        printf("\n Read other file problem");
    }
    //delete[] responseData;
}
*/

/*void report(struct sockaddr_in *serverAddress)
{
    char hostBuffer[INET6_ADDRSTRLEN];
    char serviceBuffer[NI_MAXSERV]; // defined in `<netdb.h>`
    socklen_t addr_len = sizeof(*serverAddress);
    int err = getnameinfo(
        (struct sockaddr *) serverAddress,
        addr_len,
        hostBuffer,
        sizeof(hostBuffer),
        serviceBuffer,
        sizeof(serviceBuffer),
        NI_NUMERICHOST
    );
    if (err != 0) {
        printf("It's not working!!\n");
    }
    printf("\n\n\tServer listening on http://%s:%s\n", hostBuffer, serviceBuffer);
}
*/

/*
std::string read_image(const std::string& image_path){
    std::ifstream is(image_path.c_str(), std::ifstream::in);
    is.seekg(0, is.end);
    int flength = is.tellg();
    is.seekg(0, is.beg);
    char * buffer = new char[flength];
    is.read(buffer, flength);
    std::string image(buffer, flength);
    return image;
}

int send_image(int & fd, std::string& image){

    int body_length = image.size();
    const char * body = image.data();
    int response_length = body_length;
    char * buffer = new char[response_length];
    memcpy(buffer, body, body_length);

    int ret = write(fd, buffer, response_length);

    delete [] buffer;
    return ret;
}
*/






