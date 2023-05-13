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
    int registerFlag;    // Mainly used to indicate if channel has been added to a Ring's ChannelMap
    EventLoop* ownerLoop;  // Associated EventLoop

  public:
    Channel(EventLoop* loop, int fd);
    ~Channel();

    /* Enable or disable monitoring read Event*/
    void enableRead();
    void disableRead();

    /* Enable or disable monitoring write Event*/
    void enableWrite();
    void disableWrite();

    /* Handle received event*/
    void handleEvent();

    /* Handle read and write*/
    void handleRead() const;
    void handleWrite() const;

    /* Set receivedEvent number*/
    void set_receivedEvent(int eventType) { receivedEvent = eventType;}
};