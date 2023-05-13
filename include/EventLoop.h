#pragma once

#include <include/Ring.h>
#include <include/Channel.h>
#include <liburing.h>
#include <vector>
#include <memory>

typedef std::vector<struct io_uring_cqe*> EventList;
typedef struct io_uring_cqe* Event;

class EventLoop {
  const int waitTimeMs = 10000;

  private:  // Private member variables.
    std::unique_ptr<Ring> uring;  // Work as epoll to monitor and return events
    EventList activateEvents;  // A vector of events which have occurred and been monitored
    Event currentEvent;      // Current handling event
    Channel* currentChannel;  // Current activate Channel

    bool quitFlag = false;  // Flags to control loop
    int iteration = 0;  // Number of total loop times

  public:
    EventLoop();
    ~EventLoop();

    /* Start loop*/
    void loop();

    /* Quit loop*/
    void quit();

};