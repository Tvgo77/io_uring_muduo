#include "lib/syscall_wrap.h"


int main(int argc, char **argv) 
{
    int clientfd;
    char *host, *port, buf[MAXLINE];
    rio_t rio;

    host = "127.0.0.1";
    port = "27777";

    clientfd = Open_clientfd(host, port);
    Rio_readinitb(&rio, clientfd);

    while (Fgets(buf, MAXLINE, stdin) != NULL) {
	      Rio_writen(clientfd, buf, strlen(buf));
	      Rio_readlineb(&rio, buf, MAXLINE);
	      Fputs(buf, stdout);
    }
    Close(clientfd); //line:netp:echoclient:close
    exit(0);
}