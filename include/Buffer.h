#include <vector>
#include <include/Channel.h>
#include <string.h>

#define BUF_SIZE 2048

class Buffer {
  private:
    std::vector<char> buf;
    int readIndex;
    int writeIndex;
    Channel* ownerChannel;

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
};