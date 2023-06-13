#include <include/EventLoop.h>
#include <include/Channel.h>
#include <include/Ring.h>
#include <vector>
#include <string>
#include <fcntl.h>

using std::string;

/* To test fileChannel read function, 
   comment line 187 and uncomment line 188 in Channel.cc.
   Write the read message to stdout.
*/

int main() {
    /* Initialize EventLoop */
    EventLoop eventLoop;

    /* Channel pointer list */
    Channel* ChannelPtrList[5] = {nullptr};


    /* Files to read */
    string fileDir("./file/");  // Current working directory should be io_uring
    std::vector<string> fileList = {"f1", "f2", "f3", "f4", "f5"};
    std::vector<int> fdList(5, -1);  //  Initialize all file descriptor to -1;

    /* Open every file */
    for (int i = 0; i < fileList.size(); i++) {
        string filePath = fileDir + fileList[i];
        fdList[i] = ::open(filePath.c_str(), O_RDWR);
    }

    /* Allocate Channel for every Opened file */
    for (int i = 0; i< fdList.size(); i++) {
        if (fdList[i] > 0) {
            ChannelPtrList[i] = new Channel(&eventLoop, fdList[i]);
            ChannelPtrList[i]->enable_read();
        }
    }

    /* Start loop*/
    eventLoop.loop();

}
