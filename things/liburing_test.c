#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <liburing.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[]) {
    int fd, ret, i;
    char buf[BUF_SIZE];
    struct io_uring ring;
    struct io_uring_sqe *sqe;
    struct io_uring_cqe *cqe;
    struct stat st;
    off_t off = 0;

    if (argc < 2) {
        printf("Usage: %s <file>\n", argv[0]);
        exit(1);
    }

    fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        perror("open");
        exit(1);
    }

    if (fstat(fd, &st) < 0) {
        perror("fstat");
        exit(1);
    }

    /* Initialize the io_uring instance */
    ret = io_uring_queue_init(2, &ring, 0);
    if (ret < 0) {
        perror("io_uring_queue_init");
        exit(1);
    }

    /* Prepare the read operation */
    sqe = io_uring_get_sqe(&ring);
    io_uring_prep_read(sqe, fd, buf, BUF_SIZE, off);
    io_uring_sqe_set_data(sqe, buf);

    /* Submit the operation */
    ret = io_uring_submit(&ring);
    if (ret < 0) {
        perror("io_uring_submit");
        exit(1);
    }

    /* Wait for the operation to complete */
    ret = io_uring_wait_cqe(&ring, &cqe);
    if (ret < 0) {
        perror("io_uring_wait_cqe");
        exit(1);
    }

    /* Verify the result */
    if (cqe->res < 0) {
        perror("read");
        exit(1);
    }

    /* Print the data read */
    printf("%.*s", cqe->res, (char *) io_uring_cqe_get_data(cqe));

    /* Clean up */
    io_uring_cqe_seen(&ring, cqe);
    io_uring_queue_exit(&ring);

    return 0;
}
