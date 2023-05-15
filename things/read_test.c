#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    char buf[128];
    int fd = open("./file/f2", O_RDONLY);
    int n = read(STDIN_FILENO, buf, 6);
    return 0;
}

