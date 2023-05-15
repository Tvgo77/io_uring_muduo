#include <include/EventLoop.h>
#include <include/Channel.h>
#include <include/Ring.h>
#include <sys/types.h>
#include <netinet/in.h>

int main() {
    /* Test build settings*/
    // printf("Hello World!\n");

    /* Initialize EventLoop */
    EventLoop eventLoop;

    /* Create listen socket */
    int listenfd = ::socket(AF_INET, SOCK_STREAM, 0);
    uint16_t port = 27777;
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = ::htons(port);
    ::bind(listenfd, (struct sockaddr*) &serverAddr, sizeof(serverAddr));
    int result = ::listen(listenfd, 256);

    /* Allocate Channel use listen socket and EventLoop */
    std::shared_ptr<Channel> channel(new Channel(&eventLoop, listenfd));

    /* Enable accept for listening Channel */
    channel->enable_accept();

    /* Start EventLoop */
    eventLoop.loop();
    
}