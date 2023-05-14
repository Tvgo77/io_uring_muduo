#pragma once

#include <include/Ring.h>
#include <include/Channel.h>
#include <liburing.h>
#include <vector>
#include <memory>

typedef struct io_uring_cqe** EventList;
typedef struct io_uring_cqe* Event;

class EventLoop {
  const int waitTimeSec = 10;

  private:  // Private member variables.
    std::shared_ptr<Ring> uring;  // Work as epoll to monitor and return events
    EventList activateEvents;  // A pointer to vector of events which have occurred
    int numEvents;  // The number of events of vector that pointer points to
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

    /* Get pointer to ring */
    std::shared_ptr<Ring> get_ring_ptr() { return uring; }

};