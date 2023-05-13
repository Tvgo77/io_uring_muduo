#pragma once

#include <include/Ring.h>
#include <vector>
#include <memory>

class Channel;

typedef std::vector<Channel> ChannelList;

class EventLoop {
  private:  // Private member variables.
    std::unique_ptr<Ring> uring;  // Work as epoll to monitor and return events
    ChannelList activateChannel;  // A vector of channels which have a event to handle
    Channel* currentChannel;      // Current activate channel which is now handling events

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