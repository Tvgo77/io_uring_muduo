#include <vector>
#include <include/Channel.h>
#include <string.h>

#define BUF_SIZE 2048

/* All member function are implemented inline*/
class Buffer {
  private:
    std::vector<char> buf;  // A vector of char used for storing buffer data
    std::vector<char> unprocessData;  // A buffer to store the read but unprocessed data
    int readIndex;          // Index to current first unread byte
    int writeIndex;         // Index to the position after last unread byte
    Channel* ownerChannel;  // Associated Channel

  public:
    Buffer(Channel* channel)
      : buf(BUF_SIZE),
        readIndex(0),
        writeIndex(0),
        ownerChannel(channel) {}
      
    ~Buffer() {}


    int readN(int n, void* outerBuf) {
      int nbytes = 0;  // Record how many bytes are read, then return it
      if (n > writeIndex - readIndex) {
        nbytes = writeIndex - readIndex;
        ::memcpy(outerBuf, &buf[readIndex], writeIndex - readIndex);
        // When readIndex meets writeIndex, reset them all to 0;
        readIndex = 0;
        writeIndex = 0;
      }
      else {
        nbytes = n;
        ::memcpy(outerBuf, &buf[readIndex], nbytes);
        readIndex += nbytes;
      }
      return nbytes;
    }

    /* Add writeIndex */
    void add_writeIndex(int n) {
      writeIndex += n;
    }

    /* Get pointer to read position*/
    void* readPtr() { return &buf[readIndex];}

    /* Get pointer to write position*/
    void* writePtr() { return &buf[writeIndex];}

};