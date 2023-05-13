#pragma once

#include <include/EventLoop.h>
#include <set>

struct InterestEvent {
  int eventType;      // The type of event
  bool isMonitoring;  // Indicate if this event is now being monitored
};

typedef std::set<InterestEvent> InterestEventSet;

class Channel {
  private:
    const int fd;  // Associated file descriptor
    InterestEventSet interestEvents;  // A set of event types which this channel expect to monitor
    int receivedEvent;   // The type of events received from the ring
    int returnVal;  // The return value of the submitted system call. Usally an int
    int registerFlag;    // Mainly used to indicate if channel has been added to a Ring's ChannelMap
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

    /* Handle received event*/
    void handle_event();

    /* Handle read, write, accept*/
    void handle_read() const;
    void handle_write() const;
    void handle_accept() const;

    /* Set receivedEvent number*/
    void set_receivedEvent(int eventType) { receivedEvent = eventType;}
    void set_returnVal(int result) { returnVal = result;}

};