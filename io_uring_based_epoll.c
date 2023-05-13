#include <liburing.h>
#include <fcntl.h>

#define BUF_SIZE 1024
#define MAX_ENTRY 256

#define LIBURING_INTERNAL 1

struct fd_relate {
    int i;
    int j;
};

int main() {
    /* Initialize io_uring instance*/
    struct io_uring ring;
    // Easy way to handle io_uring_setup() manually set. 
    // Check io_uring_setup() flags for advance settings.
    // Info like sq, cq ring and ring_fd are returned to struct io_uring by kernel 
    io_uring_queue_init(MAX_ENTRY, &ring, 0); 

    /* Submit request events like read and write to the SQ ring*/
    struct io_uring_sqe *sqe1;
    struct io_uring_sqe *sqe2;
    struct io_uring_cqe *cqe;
    struct io_uring_cqe **cqeList;
    // This function fill the field of struct io_uring_sqe
    sqe1 = io_uring_get_sqe(&ring);
    sqe2 = io_uring_get_sqe(&ring);
    /** Create the fd we expect to monitor its event**/
    int fd1 = open("./file/f1", O_RDONLY);
    int fd2 = open("./file/f2", O_RDONLY);
    struct fd_relate fd1_obj = {1, 1}; // fd1 related object;
    struct fd_relate fd2_obj = {2, 2};
    
    char buf[BUF_SIZE];  // read buffer
    // This function also fill fields of struct io_uring_sqe
    io_uring_prep_read(sqe1, fd1, buf, 6, 0);
    io_uring_prep_read(sqe2, fd2, buf, 6, 0);
    sqe1->user_data = (unsigned long) (&fd1_obj);
    sqe2->user_data = (unsigned long) (&fd2_obj);

    //struct io_uring_sqe sqe_copy = *sqe;

    // Now we have prepared all sq events, submit all to sq ring
    io_uring_submit(&ring);

    /* Wait submit events returned from cq ring to struct io_uring_cqe cqe */ 
    io_uring_wait_cqe(&ring, &cqe);  // This function will block until at least one event returned
    // This function return the number of occurred events and make ptr point to first one
    // And it never block
    int count1 = io_uring_peek_batch_cqe(&ring, cqeList, 5);
    io_uring_cqe_seen(&ring, &cqeList[0]);
    io_uring_cqe_seen(&ring, &cqeList[1]);
    int count2 = io_uring_peek_batch_cqe(&ring, cqeList, 5);
    struct io_uring_cqe cqe_copy = *cqe;
    struct io_uring_cqe cqe1 = *(cqeList[0]);
    struct io_uring_cqe cqe2 = *(cqeList[1]);

    /* Handle returned events*/

    return 0;
}

