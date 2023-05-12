#include <sys/epoll.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char** argv) {
    int epfd = epoll_create1(EPOLL_CLOEXEC);
    int fdList[5];

    fdList[0] = open("./file/f1", O_RDONLY);
    fdList[1] = open("./file/f2", O_RDONLY);
    fdList[2] = open("./file/f3", O_RDONLY);
    fdList[3] = open("./file/f4", O_RDONLY);
    fdList[4] = open("./file/f5", O_RDONLY);

    struct epoll_event eventList[5];
    for (int i = 0; i < 5; i++) {
        memset(&eventList[i], 0, sizeof(struct epoll_event));
        eventList[i].events = EPOLLIN | EPOLLPRI;
        eventList[i].data.fd = fdList[i];
        epoll_ctl(epfd, EPOLL_CTL_ADD, fdList[i], &eventList[i]);
    }

    struct epoll_event * eventPtr;
    int eventNum = epoll_wait(epfd, eventPtr, 5, -1);
    for (int i = 0; i < eventNum; i++) {
        char buf[128];
        read((eventPtr+i)->data.fd, buf, 6);
    }

}