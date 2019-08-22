/*
** Server side of DHCP
** Reference from Beej's Guide to C Programming and Network Programming
*/
#include <sstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#define PORT "3597" //uscID: 6044109297
#define BACKLOG 10 //pending connectinons queue
#define MAXBUFFSIZE 1024 //max buffer size for handling upcoming data

using namespace std;

static void sigchld_hdl (int sig)
{
  /* Wait for all dead processes*/
  while(waitpid(-1, NULL, WNOHANG) > 0);
}

//get socket address, depending on IPv4 or IPv6
void *get_in_addr (struct sockaddr *sa)
{
  if (sa->sa_family == AF_INET) {
    return &(((struct sockaddr_in*)sa)->sin_addr);
  }
  return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

string to_string (int num) {
  stringstream ss;
  ss << num;
  return ss.str();
}

string random_addr(string trans_id)
{
  srand(time(NULL));
  string head_addr = "";
  for (int i = 0; i < 3; i++) {
    head_addr += to_string(rand() % 256) + ".";
  }
  return head_addr + trans_id;
}

int main (void)
{
  int sockfd, new_fd; //Do listening in sock_fd, new_fd is the new connection
  struct addrinfo hints, *servinfo, *p;
  struct sockaddr_storage their_addr;
  socklen_t sin_size;
  struct sigaction sa;
  int yes = 1;
  char s[INET6_ADDRSTRLEN];
  int rv;

  memset(&hints, 0, sizeof hints); //make sure the struct is empty
  hints.ai_family = AF_UNSPEC; //no matter IPv4 or IPv6
  hints.ai_socktype = SOCK_STREAM; //using TCP
  hints.ai_flags = AI_PASSIVE; //filling my IP

  if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    return 1;
  }

  //loop for all results, bind with the first available
  for (p = servinfo; p != NULL; p = p->ai_next) {
    //perror when encounter error with socket() return -1;
    if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
      perror("server: socket");
      continue;
    }
    //perror and exit  when setsockopt is incorrect
    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
      perror("setsockopt");
      exit(1);
    }
    //perror and close current socket when binding error occured
    if(bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
      close(sockfd);
      perror("server:bind");
      continue;
    }
    //when escaping from all the errors, then p will pointing to the result
    break;
  }

  //if the p is not valid
  if(p == NULL) {
    fprintf(stderr, "server: failed to bind\n");
    return 2;
  }
  freeaddrinfo(servinfo); //release the memory for this struct
 
  //start listening, exit if error occured
  if(listen(sockfd, BACKLOG) == -1) {
    perror("listen");
    exit(1);
  }

  //clean up all the dead processes
  sa.sa_handler = sigchld_hdl;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  if(sigaction(SIGCHLD, &sa, NULL) == -1) {
    perror("sigaction");
    exit(1);
  }

  printf("server: waiting for connections...\n");
  
  int num_bytes;
  char buffer[MAXBUFFSIZE];
  string addr;
  string id;
  string trans_id;
  string recv_id;
  string msg;

  while(1) {;
    sin_size = sizeof their_addr;
    //Accept phase
    new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
    if(new_fd == -1) {
      perror("accept");
      continue;
    }

    //convert IPv4 or IPv6 addresses from binary to text form to s
    inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s);
    printf("server: got conncetion from %s\n", s);

    //Receive phase: receive from client about discovery
    num_bytes = recv(new_fd, buffer, MAXBUFFSIZE - 1, 0);
    if(num_bytes > 0) {
      cout << "Client's request received! (Transaction ID: " << buffer <<")" <<endl;
      sleep(3);
    }
    
    //Offer phase: random generated network part of IP and a id
    recv_id = string(buffer);
    addr = random_addr(recv_id);
    id = to_string(rand() % 256);
    msg = addr + "#" + id;
    strcpy(buffer, msg.c_str());
    num_bytes = send(new_fd, buffer, MAXBUFFSIZE-1, 0);
    if (num_bytes > 0) {
      cout << "Offering IP address: " << addr << " to client! (Transaction ID: " << id << ")" << endl;
    }

    //Receive phase 2: receive the request from client
    num_bytes = recv(new_fd, buffer, MAXBUFFSIZE-1, 0);
    if(num_bytes > 0) {
      cout << "Request for taking IP received (Transaction ID: " << buffer << ")" << endl;
    }
    //Ack phase
    id = to_string(rand() % 256);
    strcpy(buffer, id.c_str());
    if(num_bytes > 0) {
      cout << "Grant client to use IP: " << addr << " (Transaction ID: " << buffer << ")"<< endl;
    }
  }
  return 0;
}
