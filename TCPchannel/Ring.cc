#include <include/Ring.h>
#include <stdio.h>
#include <unistd.h>

Ring::Ring(EventLoop* loop) 
  : ring(),
    eventList(MAX_EVENT),
    ownerLoop(loop),
    channelDict() {
    /* Initialize io_uring instance*/
    ::io_uring_queue_init(MAX_EVENT, &ring, 0);
}

Ring::~Ring() {
    ::close(ring.ring_fd);
}

void Ring::addChannel(std::shared_ptr<Channel> channel) {
    int fd = channel->get_fd();
    channelDict[fd] = channel;
}


void Ring::removeChannel(Channel* channel) {
    int fd = channel->get_fd();
    channelDict.erase(fd);
}

void Ring::monitor(int timeoutSec, EventList* activeEvents, int* numEventsPtr) {
    /* Submit all channels' events expected for monitoring */
    for (auto pair: channelDict) {
        std::shared_ptr<Channel> channel = pair.second;
        channel->submit_events();
    }
    int totalSubmit = ::io_uring_submit(&ring);

    /* Log number of events submitted */
    ::printf("Log: Total %d events submitted in one loop\n", totalSubmit);

    /* Initialize struct io_uring_cqe * for receiving events*/
    struct io_uring_cqe *cqe;

    /* Initialize a timespec struct*/
    struct __kernel_timespec timeout = {timeoutSec, 0};

    /* Wait for occured events */
    eventList.clear();
    // This function will block until at least one event returned or Timeout
    ::io_uring_wait_cqe_timeout(&ring, &cqe, &timeout); 
    // This function return the number of occurred events and make ptr point to first one
    int numEvents = ::io_uring_peek_batch_cqe(&ring, &eventList[0], MAX_EVENT);

    /* Copy pointer and numEvents to EventLoop's member*/
    *activeEvents = &eventList[0];
    *numEventsPtr = numEvents;

    /* Log events occur*/
    ::printf("Log: %d events occurred\n", numEvents);
}