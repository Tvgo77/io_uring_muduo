#pragma once

#include <include/EventLoop.h>
#include <liburing.h>
#include <vector>
#include <map>
#include <memory>

#define MAX_EVENT 256

class Channel;
class EventLoop;

typedef struct io_uring_cqe** EventList;
typedef struct io_uring_cqe* Event;

/* EventOwner is used for cqe to determine its owner and event type*/
struct EventOwner {
  Channel* channel;
  int eventType;
};

class Ring {
  typedef std::vector<struct io_uring_cqe *> CqeList;
  typedef std::map<int, std::shared_ptr<Channel>> ChannelMap;  // int Key is the fd number of Channel

  private:  // private member variable
    struct io_uring ring;
    CqeList eventList;
    EventLoop* ownerLoop;
    ChannelMap channelDict;

  public:
    Ring(EventLoop* loop);
    ~Ring();

    /* Add a channel to io_uring for monitoring events*/
    void addChannel(std::shared_ptr<Channel> channel);
    
    /* Modify a channel in io_uring to change monitoring settings*/
    void updateChannel(const Channel* channel) { ; }  // Not implemented now

    /* Remove a channle in io_uring*/
    void removeChannel(Channel* channel);

    /* Monitor events occurred in channel. 
    Fill Channels which get a event into active channelList of EventLoop */
    void monitor(int timeoutMs, EventList* activeEvents, int* numEventsPtr);

    /* Get pointer to io_uring instance */
    struct io_uring* get_io_uring_ptr() { return &ring; }

    /* Debug and Test use*/
    int submitNum = 0;

    private:  // private member function

};