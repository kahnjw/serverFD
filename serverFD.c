/*
 * Skeleton code of a server using TCP protocol.
 */

#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>
#include <netdb.h>




#define MAX_CONNECTS 50
#define PORT "9034"    // port we're listening on
#define BACKLOG 10     // how many pending connections queue will hold
#define NUM_CLIENTS 4  // max number of clients

/* Keep track of connections and connection state with this array, may need
 * to be an array of structs, careful so there is no stack smashing!
 */
int connections[NUM_CLIENTS + 1];


void segFaultHelp();

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

/*
 * You should use a globally declared linked list or an array to
 * keep track of your connections.  Be sure to manage either properly
 */

//thread function declaration
int connection(int);

//global variables
char logFileName[64];

int main(int argc, char *argv[]) {

    struct timeval currTime;
    int r;
    FILE *logFile;
    int selret = 0;

    int i;
    int newfd;
    int listener; // listener socket descriptor
    int numConns = 0;
    struct addrinfo hints, *res;
    struct sockaddr_storage remoteaddr; // client address
    socklen_t addrlen;
    char s[1000];

    // shit for select()
    struct timeval tv;
    fd_set read_fds;

    // Setting shit for select, timeout vals and FDset
    tv.tv_sec = 2;
    tv.tv_usec = 500000;
    FD_ZERO(&read_fds);    // clear the master and temp sets

    memset(&hints, 0, sizeof hints); // make sure the struct is empty
    hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
    hints.ai_flags = AI_PASSIVE;     // fill in my IP for me
    hints.ai_socktype = SOCK_STREAM; // TCP stream sockets

    getaddrinfo(NULL, argv[1], &hints, &res);

    //check arguments here
    if (argc != 3)  {
		printf("usage is: ./pserver <port#> <logFile>\n");
		return 0;
    }

    // Init all connections to negative one
    for(i = 0; i < NUM_CLIENTS + 1; i++) { connections[i] = -1; }
    
    gettimeofday(&currTime,NULL);
    logFile = fopen(argv[2],"w");
    fprintf(logFile,"Server started at ...");

    fclose(logFile);

    /*
     * Open a TCP socket (an Internet stream socket).
     */
    listener = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    if(listener < 0){
        perror("socket()");
        exit(1);
    }

    connections[NUM_CLIENTS] = listener;

    /*
     * Bind our local address so that the client can send to us.
     */
    
    if( bind(listener, res->ai_addr, res->ai_addrlen) < 0 ){
        printf("bind() error\n");
        exit(1);
    }

    /*
     * here you should listen() on your TCP socket
     */
    if(listen(listener, BACKLOG)) {
        perror("linsten()");
        exit(1);
    }

    FD_SET(listener, &read_fds);

    while(1) {
        printf("Waiting for a connection\n\n");
        if (selret = select(NUM_CLIENTS + 1, &read_fds, NULL, NULL, NULL) == -1) {
            perror("select");
            exit(4);
        }
        printf("Got connection\n\n");
        for(i = 0; i < NUM_CLIENTS + 1; i++) {
            if( FD_ISSET(connections[i], &read_fds) && numConns < NUM_CLIENTS){
                if (connections[i] == listener) {
                    numConns++;
                    // handle new connections
                    addrlen = sizeof remoteaddr;
                    newfd = accept(listener,
                        (struct sockaddr *)&remoteaddr,
                        &addrlen);
                    printf("Stored new connection\n");
                    FD_SET(newfd, &read_fds);
                    connections[numConns] = newfd;
                } else {
                    if(recv(connections[i], s, sizeof(s), NULL) < 0) {
                        perror("recv()");
                        exit(1);
                    }
                    printf("Recieved some shit: %s\n", s);
                }
            }

            if(selret > 0){
                
                        //if someone is trying to connect, you'll have to accept()
                //the connection
                    //newsockfd = accept(...)

                //if you've accepted the connection, you'll probably want to
                //check "select()" to see if they're trying to send data,
                    //like their name, and if so
                //recv() whatever they're trying to send

                //since you're talking nicely now.. probably a good idea send them
                //a message to welcome them to the new client.

                //if there are others connected to the server, probably good to notify them
                //that someone else has joined.


                //now add your new user to your global list of users

                //now you need to start a thread to take care of the
                //rest of the messages for that client
                r = connection(listener);
                if (r != 0) { fprintf(stderr, "thread create failed\n"); }

                //A requirement for 5273 students:
                //at this point...
                //whether or not someone connected, you should probably
                //look for clients that should be timed out
                //and kick them out
                //oh, and notify everyone that they're gone.
            } else if(selret < 0) {
                    printf("Error: 0 > select()\n");
                    exit(1);
            }
        }
    }
}





//-----------------------------------------------------------------------------
int connection(int sockfd) {
    return 0;
}

void segFaultHelp(){
    printf("Got to seg fault helper function\n");
    exit(0);
}