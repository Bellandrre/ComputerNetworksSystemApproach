/**
 * Created by karthik Kolathumani
**/

#include <stdio.h>
#include <winsock2.h>
#include <stdlib.h>
#include <io.h>
#define SERVER_PORT 5432
#define MAX_LINE 256
#pragma comment(lib, "ws2_32")
int main (int argc, char * argv[]){
    //FILE *fp;
    struct hostent *hp;
    struct sockaddr_in sin;
    char *host;
    char buf[MAX_LINE];
    int s;
    int len;

    if(argc == 2){
        host = argv[1];
    }else{
        fprintf(stderr, "usage: simplex-talk host \n");
        exit(1);
    }

    /* translate host name into peer's IP address */
    hp = gethostbyname(host); // returns a structure of type hostent for given host name
    if( !hp ){
        fprintf(stderr, "simplex-talk: unknown host: %s\n", host);
        exit(1);
    }

    /* win32 equivalent for bzero*/
    memset((char *)&sin, 0,  sizeof(sin)); // sets everything as 0 at the address sin.
    sin.sin_family = AF_INET;
    memcpy(hp->h_addr, (char *)&sin.sin_addr, hp->h_length);
    sin.sin_port = htons(SERVER_PORT); // converts the integer from host byte order to network byte order.

    if((s = socket(PF_INET, SOCK_STREAM,0)) < 0){ // socket will return an identifier to use it as a reference
        perror("simplex-talk: socket"); // Displays the Error message
        exit(1);
    }

    if(connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0){
        perror("simplex-talk: connect");
        close(s); // closes and flushes the stream
        exit(1);
    }

    while(fgets(buf, sizeof(buf), stdin)){ // reads a line from the standard input file descriptor and stores it in buf
        buf[MAX_LINE - 1] = '\0';
        len = strlen(buf) + 1;
        send(s, buf, len, 0);
    }
}
