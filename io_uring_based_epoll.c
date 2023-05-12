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
    struct io_uring_sqe *sqe;
    struct io_uring_cqe *cqe;
    // This function fill the field of struct io_uring_sqe
    sqe = io_uring_get_sqe(&ring);
    /** Create the fd we expect to monitor its event**/
    int fd1 = open("./file/f1", O_RDONLY);
    struct fd_relate fd1_obj; // fd1 related object;
    
    char buf[BUF_SIZE];  // read buffer
    // This function also fill fields of struct io_uring_sqe
    io_uring_prep_read(sqe, fd1, buf, 6, 0);
    sqe->user_data = (unsigned long) (&fd1_obj);

    struct io_uring_sqe sqe_copy = *sqe;

    // Now we have prepared all sq events, submit all to sq ring
    io_uring_submit(&ring);

    /* Wait submit events returned from cq ring to struct io_uring_cqe cqe */ 
    io_uring_wait_cqe(&ring, &cqe);
    struct io_uring_cqe cqe_copy = *cqe;

    /* Handle returned events*/

    return 0;
}

