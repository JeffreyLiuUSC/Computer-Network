/*
** Client side of DHCP
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
#define MAXBUFFSIZE 100 // maximum receiving chars

using namespace std;

//get socket address, depending on IPv4 or IPv6
void *get_in_addr (struct sockaddr *sa)
{
  if (sa->sa_family == AF_INET) {
    return &(((struct sockaddr_in*)sa)->sin_addr);
  }
  return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

string to_string(int num)
{
  stringstream ss;
  ss << num;
  return ss.str();
}

int main (int argc, char *argv[])
{
  int sockfd;
  char buffer[MAXBUFFSIZE];
  int num_bytes;
  char s[INET6_ADDRSTRLEN];
  int rv;
  struct addrinfo hints, *servinfo, *p;
  
  if(argc != 2) {
    fprintf(stderr, "usage: client hostname\n");
    exit(1);
  }

  memset(&hints, 0, sizeof hints); //make sure the struct is empty
  hints.ai_family = AF_UNSPEC; //no matter IPv4 or IPv6
  hints.ai_socktype = SOCK_STREAM; //using TCP

  if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    return 1;
  }

  //loop for all results, connect the first available one
  for (p = servinfo; p != NULL; p = p->ai_next) {
    //perror when encounter error with socket() return -1
    if((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1){
      perror("client: socket");
      continue;
    }
    //connect(), close socket when encounter error
    if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
      close(sockfd);
      perror("client: connect");
      continue;
    }
    //when excaping from all the errors, socket() and connect() finished
    break;
  }

  //if the p is not valid
  if (p == NULL) {
    fprintf(stderr, "client: failed to connect\n");
    return 2;
  }
  //convert IPv4 or IPv6 addresses from binary to text form to s
  inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), s, sizeof s);
  printf("client: connecting to %s\n", s);

  freeaddrinfo(servinfo); //all rely on this structure

  string id;
  string recv_addr;
  string recv_id;
  string msg;
  //Discoverying phase
  id = to_string(rand() % 256);
  strcpy(buffer, id.c_str());
  num_bytes = send(sockfd, buffer, MAXBUFFSIZE-1, 0);
  if (num_bytes > 0) {
    cout << "Exploring an IP address in Discovery phase (transaction ID: " << buffer << ")" << endl;
  }

  //receiving an offer from DHCP server
  num_bytes = recv(sockfd, buffer, MAXBUFFSIZE-1, 0);
  if(num_bytes > 0) {
    msg = string(buffer);
    int idx = msg.find('#');
    recv_addr = msg.substr(0, idx);
    recv_id = msg.substr(idx+1);
    cout << "Offer received: IP: " << recv_addr << " (Transaction ID: " << recv_id << ")" <<endl;
    sleep(3);
  }
  
  //Request phase
  id = to_string(rand() % 256);
  strcpy(buffer, id.c_str());
  num_bytes = send(sockfd, buffer, MAXBUFFSIZE-1, 0);
  if (num_bytes > 0) {
    cout << "Confirming taking IP: " << recv_addr << " (Transaction ID: "<< buffer << ")" << endl;
  }
  num_bytes = recv(sockfd, buffer, MAXBUFFSIZE-1, 0);
  if(num_bytes > 0) {
    cout << "DHCP completed, now using IP: " << recv_addr << endl;
  }
  
  if (num_bytes == -1) {
    perror("recv");
    exit(1);
  }
  buffer[MAXBUFFSIZE-1] = '\0';
  close(sockfd);

  return 0;

}
