
// taken from: https://www.bogotobogo.com/cplusplus/sockets_server_client.php
/* The port number is passed as an argument */
//#define _BSD_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <random>
#include <cmath>

#include "messages.hpp"

#define NOISE 1e-3
#define EPS 1e-6

const unsigned int taskDuration = 2;

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

MessageTaskWithResult solveTaskWithNoise(const MessageTask& task, std::mt19937& gen, unsigned int duration = 2) {
	MessageTaskWithResult ret(task);
     std::normal_distribution<> d {0, NOISE};
     ret.firstOperand += d(gen);
     ret.secondOperand += d(gen);
     sleep(duration); // This is a complex task - takes a while
	switch(ret.operation) {
		case ADD: ret.result = ret.firstOperand + ret.secondOperand; break;
		case SUB: ret.result = ret.firstOperand - ret.secondOperand; break;
		case MUL: ret.result = ret.firstOperand * ret.secondOperand; break;
		case DIV: ret.result = fabs(ret.secondOperand) < EPS ? std::numeric_limits<double>::signaling_NaN() : ret.firstOperand / ret.secondOperand; break;
	}
	return ret;
}



int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno;
     socklen_t clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     // create a socket
     // socket(int domain, int type, int protocol)
     sockfd =  socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");

     // clear address structure
     bzero((char *) &serv_addr, sizeof(serv_addr));

     portno = atoi(argv[1]);

     /* setup the host_addr structure for use in bind call */
     // server byte order
     serv_addr.sin_family = AF_INET;  

     // automatically be filled with current host's IP address
     serv_addr.sin_addr.s_addr = INADDR_ANY;  

     // convert short integer value for port must be converted into network byte order
     serv_addr.sin_port = htons(portno);

     // bind(int fd, struct sockaddr *local_addr, socklen_t addr_length)
     // bind() passes file descriptor, the address structure, 
     // and the length of the address structure
     // This bind() call will bind  the socket to the current IP address on port, portno
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");

     // This listen() call tells the socket to listen to the incoming connections.
     // The listen() function places all incoming connection into a backlog queue
     // until accept() call accepts the connection.
     // Here, we set the maximum size for the backlog queue to 5.
     std::cout << "Listening ..." << std::endl;
     listen(sockfd,5);
     std::cout << "... listened" << std::endl;

     // The accept() call actually accepts an incoming connection
     clilen = sizeof(cli_addr);

     // This accept() function will write the connecting client's address info 
     // into the the address structure and the size of that structure is clilen.
     // The accept() returns a new socket file descriptor for the accepted connection.
     // So, the original socket file descriptor can continue to be used 
     // for accepting new connections while the new socker file descriptor is used for
     // communicating with the connected client.
     std::cout << "Newsocking ..." << std::endl;
     newsockfd = accept(sockfd, 
                 (struct sockaddr *) &cli_addr, &clilen);
     if (newsockfd < 0) 
          error("ERROR on accept");
     std::cout << "... newsocked" << std::endl;

     printf("server: got connection from %s port %d\n",
            inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));


     // Create random number generator
    std::random_device rd {};
    std::mt19937 gen {rd()};

    while(true) {

          MessageTask receiveTask;
          n = read(newsockfd,&receiveTask, sizeof(receiveTask));
          if (n < 0) error("ERROR reading receiveTask from socket");

          std::cout << "Here is the message: " << receiveTask << std::endl;	

          MessageTaskWithResult result = solveTaskWithNoise(receiveTask, gen, taskDuration);

          std::cout << "Here is the result with noise: " << result << std::endl;


          // This send() function sends the 13 bytes of the string to the new socket
          send(newsockfd, &result, sizeof(result), 0);

	     std::cout << "Sent it away" << std::endl;

    }

     bzero(buffer,256);

     n = read(newsockfd,buffer,255);
     if (n < 0) error("ERROR reading from socket");
     printf("Here is the message: %s\n",buffer);

     close(newsockfd);
     close(sockfd);
     return 0; 
}
     
