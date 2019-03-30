 #include <stdio.h>
 #include <sys/socket.h>
 #include <sys/types.h>
 #include <stdlib.h>
 #include <netinet/in.h>
 #include <errno.h>
 #include <string.h>
 #include <arpa/inet.h>
 #include <unistd.h>
 #define MAXLINE 1024

 #define PORT 8888

 int main(int argc,char **argv)
 {
     char *servInetAddr = "127.0.0.1";
     int socketfd;
     struct sockaddr_in sockaddr;
     char recvline[MAXLINE], sendline[MAXLINE];
     char *buf = "hello world\n";

     socketfd = socket(AF_INET,SOCK_STREAM,0);
     memset(&sockaddr,0,sizeof(sockaddr));
     sockaddr.sin_family = AF_INET;
     sockaddr.sin_port = htons(PORT);
     inet_pton(AF_INET,servInetAddr,&sockaddr.sin_addr);
     if((connect(socketfd,(struct sockaddr*)&sockaddr,sizeof(sockaddr))) < 0 )
     {
     printf("connect error %s errno: %d\n",strerror(errno),errno);
     exit(0);
     }

     printf("send message to server\n");

     if((send(socketfd,sendline,strlen(buf),0)) < 0)
     {
     printf("send mes error: %s errno : %d",strerror(errno),errno);
     exit(0);
     }
     printf("维持连接但不做事\n");
while(1) {

}
     close(socketfd);
     printf("exit\n");
     exit(0);
 }
