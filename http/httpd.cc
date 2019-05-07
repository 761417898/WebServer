//like TinyHttpd
#include "../net/GaoServerHeaders.h"

using namespace GaoServer;

#define ISspace(x) isspace((int)(x))

#define SERVER_STRING "Server: jdbhttpd/0.1.0\r\n"
#define STDIN   0
#define STDOUT  1
#define STDERR  2

void onConnectionServer(const TcpConnectionPtr& conn) {
}

//void bad_request(int);
void cat(int, FILE *);
//void cannot_execute(int);
//void error_die(const char *);
//void execute_cgi(int, const char *, const char *, const char *);
//int get_line(int, char *, int);
void headers(int, const char *);
//void not_found(int);
void serve_file(int, const char *);
//int startup(u_short *);
void unimplemented(int);

void onMessageServer(const TcpConnectionPtr& conn, const char* data, ssize_t len) {
    //printf("on message, received: %ld bytes, %s\n", buffer->readableBytes(), (buffer->readAsString(12)).c_str());

    write(conn->socket_->fd(), "hello world too\n", 16);

   /* size_t len = buffer->readableBytes();
    //GET / HTTP/1.0\r\nUser-Agent: WebBench 1.5\r\nHost: 127.0.0.1\r\n\r\n
    auto str = buffer->readAsString(len);
    std::string message = conn->name() + " hello world too\n";
    conn->send(message);
    //write(conn->socket_->fd(), "hello world too\n", 16);
    return;
    if (len == 0) return;
    auto buf = str.c_str();
    int client = conn->socket_->fd();

    char method[255];
    char url[255];
    char path[512];
    size_t i, j;
    struct stat st;

    i = 0; j = 0;
    while (!ISspace(buf[i]) && (i < sizeof(method) - 1))
    {
        method[i] = buf[i];
        i++;
    }
    j=i;
    method[i] = '\0';

    if (strcasecmp(method, "GET") && strcasecmp(method, "POST"))
    {
        unimplemented(client);
        return;
    }

    i = 0;
    while (ISspace(buf[j]) && buf[j] != '\n')
        j++;
    while (!ISspace(buf[j]) && (i < sizeof(url) - 1) && buf[j] != '\n')
    {
        url[i] = buf[j];
        i++; j++;
    }
    url[i] = '\0';

    sprintf(path, "htdocs%s", url);
    if (path[strlen(path) - 1] == '/')
        strcat(path, "index.html");

    serve_file(client, path);


    //std::string message = "hello world too\n";
    //conn->send(message);
*/
}

void headers(int client, const char *filename)
{
    char buf[1024];
    (void)filename;  /* could use filename to determine file type */

    strcpy(buf, "HTTP/1.0 200 OK\r\n");
    send(client, buf, strlen(buf), 0);
    strcpy(buf, SERVER_STRING);
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "Content-Type: text/html\r\n");
    send(client, buf, strlen(buf), 0);
    strcpy(buf, "\r\n");
    send(client, buf, strlen(buf), 0);
}

void serve_file(int client, const char *filename)
{
    FILE *resource = NULL;
   /* int numchars = 1;
    char buf[1024];

    buf[0] = 'A'; buf[1] = '\0';
    while ((numchars > 0) && strcmp("\n", buf))  // read & discard headers
        numchars = get_line(client, buf, sizeof(buf));*/

    resource = fopen(filename, "r");
    if (resource == NULL)
        printf("resource open error\n");
    else
    {
        headers(client, filename);
        cat(client, resource);
    }
    fclose(resource);
}

void cat(int client, FILE *resource)
{
    char buf[1024];

    fgets(buf, sizeof(buf), resource);
    while (!feof(resource))
    {
        send(client, buf, strlen(buf), 0);
        fgets(buf, sizeof(buf), resource);
    }
}

void unimplemented(int client)
{
    char buf[1024];

    sprintf(buf, "HTTP/1.0 501 Method Not Implemented\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, SERVER_STRING);
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "Content-Type: text/html\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "<HTML><HEAD><TITLE>Method Not Implemented\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "</TITLE></HEAD>\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "<BODY><P>HTTP request method not supported.\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "</BODY></HTML>\r\n");
    send(client, buf, strlen(buf), 0);
}

void *closeMainLoopServer(void *arg) {
    EventLoop *mainLoop = static_cast<EventLoop*>(arg);
    ::sleep(12000);
    printf("mainLoop quited\n");
    mainLoop->quit();
}

int main()
{
    uint16_t testPort = 8888;
    InetAddress localAddr(testPort);
    std::string testName = "NEW TcpConnection Test";
    EventLoop loop;

    TcpServer server(&loop, localAddr, testName);
    server.setConnectionCallBack(onConnectionServer);
    server.setMessageCallBack(onMessageServer);

    server.start();

    pthread_t tid;
    pthread_create(&tid, NULL, closeMainLoopServer, (void*)&loop);
    loop.loop();
    pthread_join(tid, NULL);
}
