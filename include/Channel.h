#pragma once

#include <include/EventLoop.h>
#include <include/Buffer.h>
#include <map>

/* Macro define for Event type*/
#define EVENT_READ IORING_OP_READ
#define EVENT_WRITE IORING_OP_WRITE
#define EVENT_ACCEPT IORING_OP_ACCEPT

/* Macro define for registerFlag*/
#define NEW 0
#define ADDED 1


typedef std::map<int, bool> InterestEventMap;
typedef std::vector<struct io_uring_sqe*> SqeList;

class Channel {
  private:
    const int fd;  // Associated file descriptor
    InterestEventMap interestEvents;  // A set of event types which this channel expect to monitor
    SqeList sqeList;
    std::vector<EventOwner> eventOwners;
    int receivedEvent;   // The type of events received from the ring
    int returnVal;  // The return value of the submitted system call. Usally an int
    int registerFlag;    // Mainly used to indicate if channel has been added to a Ring's ChannelMap
    Buffer buffer;
    EventLoop* ownerLoop;  // Associated EventLoop

  public:
    Channel(EventLoop* loop, int fd);
    ~Channel();

    /* Enable or disable monitoring read() Event*/
    void enable_read();
    void disable_read();

    /* Enable or disable monitoring write() Event*/
    void enable_write();
    void disable_write();

    /* Enable or disable monitoring accept() Event*/
    void enable_accept();
    void disable_accept();

    /* Submit interest event*/
    void submit_events();

    /* Handle received event*/
    void handle_event();

    /* Handle read, write, accept*/
    void handle_read() const;
    void handle_write() const;
    void handle_accept() const;

    /* Set receivedEvent number*/
    void set_receivedEvent(int eventType) { receivedEvent = eventType;}
    void set_returnVal(int result) { returnVal = result;}
    void set_isMonitoring(int eventType, bool isMonitoring) {
      interestEvents[eventType] = isMonitoring;
    }

    /* Get channel's fd number*/
    int get_fd() const { return fd;}
};