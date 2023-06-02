#include <include/Channel.h>
#include <memory>
#include <string>


Channel::Channel(EventLoop* loop, int fdNum) 
  : fd(fdNum),
    interestEvents(),
    sqeList(MAX_EVENT, nullptr),
    eventOwners(MAX_EVENT, {this, 0}),
    receivedEvent(0),
    returnVal(0),
    registerFlag(0),
    buffer(this),
    ownerLoop(loop) {;}

Channel::~Channel() {;}

void Channel::enable_read() {
    interestEvents[EVENT_READ] = false;

    /* Have not added to a ring*/
    if (registerFlag == NEW) {
        auto ring = ownerLoop->get_ring_ptr();
        // Create a shared_ptr to channel and copy it to ChannelDict of Ring
        // Then the shared_ptr in Ring's Channel Map will be the only one
        // So we can destory the dynamically allocated Channel by remove it from Ring
        std::shared_ptr<Channel> channelPtr(this);
        ring->addChannel(channelPtr);
        registerFlag = ADDED;
    }

    /* Already in the ring*/
    else if (registerFlag == ADDED) {
        // Do nothing
    }
}

void Channel::disable_read() {
    interestEvents.erase(EVENT_READ);

    /* If no interestEvents, remove it from Ring*/
    if (interestEvents.empty()) {
        auto ring = ownerLoop->get_ring_ptr();
        ring->removeChannel(this);
        registerFlag = NEW;
    }
}

void Channel::enable_accept() {
    interestEvents[EVENT_ACCEPT] = false;

    /* Have not added to a ring*/
    if (registerFlag == NEW) {
        auto ring = ownerLoop->get_ring_ptr();
        std::shared_ptr<Channel> channelPtr(this);
        ring->addChannel(channelPtr);
        registerFlag = ADDED;
    }
}

void Channel::disable_accept() {
    interestEvents.erase(EVENT_ACCEPT);

    /* If no interestEvents, remove it from Ring */
    if (interestEvents.empty()) {
        auto ring = ownerLoop->get_ring_ptr();
        ring->removeChannel(this);
        registerFlag = NEW;
    }
}

void Channel::submit_events() {
    /* Submit all interest event if it's not being monitored*/
    int i = 0;  // for loop count
    auto ring = ownerLoop->get_ring_ptr();
    for (auto pair: interestEvents) {
        if (pair.second == false) {
            sqeList[i] = ::io_uring_get_sqe(ring->get_io_uring_ptr());
            switch (pair.first) {  // May change to use map if too many event types
                case EVENT_READ: {
                    /* Read data to inner buffer */
                    ::io_uring_prep_read(sqeList[i], fd, buffer.writePtr(), BUF_SIZE/2, 0);

                    /* Set user_data points to eventOwner */
                    eventOwners[i].eventType = EVENT_READ;
                    io_uring_sqe_set_data(sqeList[i], &eventOwners[i]);

                    /* Change state to isMonitoring == true */
                    interestEvents[pair.first] = true;

                    /* Log preparing complete */  
                    printf("Log: fd %d read event prepared\n", fd);
                    break;
                }
                case EVENT_WRITE:
                    break;
                case EVENT_ACCEPT: {
                    /* Accept TCP connection on listening fd */
                    // The submit of accept event is not in this scope.
                    // So make sure the sockaddr pointer is valid before the final submit
                    struct sockaddr_storage* unused_addr = &addr;
                    socklen_t* unused_addrlen = &addrlen;
                    ::io_uring_prep_accept(sqeList[i], 
                                           fd, 
                                           (struct sockaddr*) unused_addr, 
                                           unused_addrlen,
                                           0);
                    
                    /* Set user_data points to eventOwner */
                    eventOwners[i].eventType = EVENT_ACCEPT;
                    io_uring_sqe_set_data(sqeList[i], &eventOwners[i]);

                    /* Change state to isMonitoring == true */
                    interestEvents[pair.first] = true;

                    /* Log preparing complete */  
                    printf("Log: fd %d accept event prepared\n", fd);
                    break;
                }
                default:
                    break;    
            }
        }
    }
}

void Channel::handle_event() {
    switch (receivedEvent) {  // Change switch to map if there are too many event types
        case EVENT_READ:
            handle_read();
            break;
        case EVENT_WRITE:
            handle_write();
            break;
        case EVENT_ACCEPT:
            handle_accept();
            break;
        default:
            break;
    }
}

void Channel::handle_read() {
    /* returnVal means the number of bytes read*/
    /* Error handling*/
    if (returnVal < 0) {
        printf("Error: read fd %d read error occured\n", fd);

        /* Close related fd*/
        ::close(fd);

        /* Remove channel from ring*/
        auto ring = ownerLoop->get_ring_ptr();
        ring->removeChannel(this);

        /* Check if "this" pointer is the last one pointed to Channel*/
    }

    else if (returnVal == 0) {
        printf("Log: read fd %d EOF detect. Close socket and remove Channel\n", fd);

        ::close(fd);

        auto ring = ownerLoop->get_ring_ptr();
        ring->removeChannel(this);

    }

    else if (returnVal > 0) {
        /* Add buffer's writeIndex */
        buffer.add_writeIndex(returnVal);

        /* Read buffer */
        char buf[BUF_SIZE];
        int nbytes_1 = buffer.read_unprocessed(buf);
        int nbytes_2 = buffer.readN(buf+nbytes_1, BUF_SIZE/2);

        /* Do echo for every line*/
        int stringHeadIndex = 0;
        int i = 0;
        for (i = 0; i < nbytes_1 + nbytes_2; i++) {
            if (buf[i] != '\n')
                continue;
            else {
                std::string message(&buf[stringHeadIndex], i - stringHeadIndex + 1);
                //::write(fd, &message[0], message.size());  // Send echo message back;
                ::write(STDOUT_FILENO, &message[0], message.size());  // For file read send to stdout;
                stringHeadIndex = i + 1;
            }
        }

        /* Save unprocessed data into buffer */
        buffer.write_unprocessed(&buf[stringHeadIndex], i - stringHeadIndex);
    }
}

void Channel::handle_accept() {
    /* returnVal means the fd number of connection socket*/
    /* Error handling*/
    if (returnVal < 0) {
        printf("Error: listening fd %d accept error occured\n", fd);

        /* Close related fd*/
        ::close(fd);

        /* Remove channel from ring*/
        auto ring = ownerLoop->get_ring_ptr();
        ring->removeChannel(this);


        /* Check if "this" pointer is the last one pointed to Channel*/
    }
    else {
        /* Dynamicall allocate a new channel */
        Channel* tcpChannel = new Channel(ownerLoop, returnVal);

        /* Enable read for this channel */
        tcpChannel->enable_read();
    }
}



