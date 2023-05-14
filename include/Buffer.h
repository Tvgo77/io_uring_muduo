#include <vector>
#include <include/Channel.h>
#include <string.h>

#define BUF_SIZE 2048

class Buffer {
  private:
    std::vector<char> buf;  // A vector of char used for storing buffer data
    int readIndex;          // Index to current first unread byte
    int writeIndex;         // Index to the position after last unread byte
    Channel* ownerChannel;  // Associated Channel

  public:
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

    /* Get pointer to read position*/
    void* readPtr() { return &buf[readIndex];}

    /* Get pointer to write position*/
    void* writePtr() { return &buf[writeIndex];}
    
};