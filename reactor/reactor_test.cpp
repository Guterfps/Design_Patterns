/*-------------------------------------------*/
/* DEVELOPER: Itay Guter                    */
/* LAST MODIFIED: 15/6/2023	               */
/* WORKSHEET NAME: Reactor                */
/* REVIEWER: Gali 		                 */
/*--------------------------------------*/

#include <iostream>
#include <unistd.h>
#include <string>
#include <cstring> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 

#include "reactor.hpp"

static constexpr const int PORT = 8080;
static constexpr const int MAXBUFLEN = 1024;

void SimpleReactorTest();
void TestPingPongServer(int port);
int HandleTCPClient(int connfd, const char *message, int listenfd, 
                    ilrd::Reactor &reactor);
int HandleUDPClient(int udpfd, const char *message, struct sockaddr *cliaddr);

int main()
{
    SimpleReactorTest();

    TestPingPongServer(PORT);

    return 0;
}

void SimpleReactorTest()
{
    ilrd::Reactor reactor;

    reactor.Add(STDOUT_FILENO, ilrd::Reactor::WRITE,
    [&reactor]
    {
        std::cout << "Hello write" << std::endl;
        reactor.Add(STDOUT_FILENO, ilrd::Reactor::READ,
        []
        {
            std::cout << "Hello read" << std::endl;
        });
        reactor.Remove(STDOUT_FILENO, ilrd::Reactor::WRITE);
    });
    reactor.Add(STDIN_FILENO, ilrd::Reactor::READ,
    [&reactor]
    {
        std::string str;
        std::getline(std::cin, str);
        if (str == "stop")
        {
            reactor.Stop();
        }
    });

    reactor.Run();
    
}

void TestPingPongServer(int port)
{
    ilrd::Reactor reactor;
    
    int listenfd, udpfd, connfd;
    socklen_t len;
    struct sockaddr_in cliaddr, servaddr;
    const char *message = "pong";

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == listenfd)
    {
        perror("socket tcp");
        return;
    }

    std::memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);
 
    if (-1 == bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)))
    {
        perror("bind");
        return;
    }
    
    if (-1 == listen(listenfd, 10))
    {
        perror("listen");
        return;
    }
 
    udpfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (-1 == udpfd)
    {
        perror("socket udp");
        return;
    }
    if (-1 == bind(udpfd, (struct sockaddr*)&servaddr, sizeof(servaddr)))
    {
        perror("bind udp");
        return;
    }

    reactor.Add(udpfd, ilrd::Reactor::READ,
    [&udpfd, &cliaddr, &message]
    {
        std::cout << "server: got udp message" << std::endl;
        HandleUDPClient(udpfd, message, (struct sockaddr*)&cliaddr);
    });

    reactor.Add(listenfd, ilrd::Reactor::READ,
    [&listenfd, &cliaddr, &len, &message, &connfd, &reactor]
    {
        len = sizeof(cliaddr);
        connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &len);
        if (-1 == connfd)
        {
            perror("accept");
            return;
        }
        printf("server: got connection from %s\n", 
                inet_ntoa(cliaddr.sin_addr));

        HandleTCPClient(connfd, message, listenfd, reactor);
        close(connfd);    
    });

    reactor.Add(STDIN_FILENO, ilrd::Reactor::READ,
    [&reactor]
    {
        std::string str;
        std::getline(std::cin, str);
        if (str == "stop")
        {
            reactor.Stop();
        }
    });

    std::cout << "Server is running on port " << port << std::endl;
    reactor.Run();
}

int HandleTCPClient(int connfd, const char *message, int listenfd, 
                    ilrd::Reactor &reactor)
{
    pid_t childpid;

    if ((childpid = fork()) == 0) 
    {
        char buf[MAXBUFLEN] = {0};
        int numbytes;
        close(listenfd);

        while (0 != (numbytes = recv(connfd, buf, MAXBUFLEN - 1, 0))) 
        {
            if (-1 == numbytes)
            {
                perror("recv");
                return -1;
            }
            buf[numbytes] = '\0';
            printf("\nMessage from TCP client: '%s'\n", buf);
            memset(buf, 0, MAXBUFLEN);

            if (-1 == send(connfd, message, strlen(message), 0))
            {
                perror("send");
            }
        }
        printf("TCP client disconnected\n");
        close(connfd);
        reactor.Stop();
    }

    return 0;
}

int HandleUDPClient(int udpfd, const char *message, struct sockaddr *cliaddr)
{
    ssize_t n;
    char buffer[MAXBUFLEN] = {0};
    socklen_t len = sizeof(struct sockaddr_in);
    
    n = recvfrom(udpfd, buffer, sizeof(buffer), 0,
                (struct sockaddr*)cliaddr, &len);
    if (-1 == n)
    {
        perror("recvfrom");
        return -1;
    }
    printf("\ngot packet from %s\n", 
            inet_ntoa(((struct sockaddr_in*)cliaddr)->sin_addr));
    printf("Message from UDP client: '%s'\n", buffer);

    if (-1 == sendto(udpfd, message, sizeof(buffer), 0,
        (struct sockaddr*)cliaddr, sizeof(*cliaddr)))
    {
        perror("sendto");
        return -1;
    }
    
    return 0;
}