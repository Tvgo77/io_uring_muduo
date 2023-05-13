#pragma once

#include <include/Ring.h>
#include <include/Channel.h>
#include <liburing.h>
#include <vector>
#include <memory>

typedef std::vector<struct io_uring_cqe*> EventList;
typedef struct io_uring_cqe* Event;

class EventLoop {
  private:  // Private member variables.
    std::unique_ptr<Ring> uring;  // Work as epoll to monitor and return events
    EventList activateEvents;  // A vector of events which have occurred and been monitored
    Event currentEvent;      // Current handling event

    /* Flags to control loop */
    bool quit = false;

  public:
    EventLoop();
    ~EventLoop();

    /* Start loop*/
    void loop();

    /* Quit loop*/
    void quit();

};