#include <liburing.h>
#include <vector>
#include <map>

class EventLoop;
class Channel;

typedef std::vector<Channel> ChannelList;


class Ring {
  typedef std::vector<struct io_uring_cqe> cqeList;
  typedef std::map<int, Channel> ChannelMap;  // int Key is the fd number of Channel

  private:  // private member variable
    int ringfd;
    cqeList eventList;
    EventLoop* ownerLoop;
    ChannelMap channelDict;

  public:
    Ring(EventLoop* loop);
    ~Ring();

    /* Add a channel to io_uring for monitoring events*/
    void addChannel(const Channel* channel);
    
    /* Modify a channel in io_uring to change monitoring settings*/
    void updateChannel(const Channel* channel) { ; }  // Not implemented now

    /* Remove a channle in io_uring*/
    void removeChannel(const Channel* channel);

    /* Monitor events occurred in channel. 
    Fill Channels which get a event into active channelList of EventLoop */
    void monitor(int timeoutMs, ChannelList* activeChannels);

    private:  // private member function

};