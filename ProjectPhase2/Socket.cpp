/*
 * Socket.cpp
 * Implementation of socket programming in general
 * mainly referred from http://www.beej.us/guide/bgnet
 */

#include "Socket.hpp"

void* get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*) sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*) sa)->sin6_addr);
}

std::string Socket::get_socket_port(int sockfd) {
  struct sockaddr_in sin;
  socklen_t addrlen = sizeof(sin);
  getsockname(sockfd, (struct sockaddr *) &sin, &addrlen); //port number in sin
  int local_port = ntohs(sin.sin_port); //port number transform
  char local_port_s[MAXDATASIZE];
  sprintf(local_port_s, "%d", local_port);
  return (std::string) local_port_s;
}

std::string Socket::get_self_ip_address() {
  struct addrinfo hints, *res, *p;
  char ipstr[INET6_ADDRSTRLEN];
  
  memset(&hints, 0, sizeof(hints)); //make sure the struct is empty
  hints.ai_family = AF_INET; //no matter IPv4 or IPv6
  hints.ai_socktype = SOCK_STREAM;  //using TCP stream
  hints.ai_flags = AI_PASSIVE;
  
  getaddrinfo(SERVER, NULL, &hints, &res);
  
  for (p = res; p != NULL; p = p->ai_next) {
    void *addr;
    struct sockaddr_in *ip = (struct sockaddr_in *) p->ai_addr;
    addr = &(ip->sin_addr);
    // Convert IP from binary to text
    inet_ntop(p->ai_family, addr, ipstr, sizeof(ipstr));
  }
  freeaddrinfo(res);
  return (std::string) ipstr;
}

int Socket::create_udp_socket(int type, const char *port, struct addrinfo *p) {
  int sockfd = 0;
  int rv;
  struct addrinfo hints, *servinfo;
  
  memset(&hints, 0, sizeof(hints)); // make sure the struct is empty
  hints.ai_family = AF_INET; // no matter IPv4 or IPv6
  hints.ai_socktype = SOCK_DGRAM; //Using UDP datagram
  
  if (type == UDP_LISTENER) {
    hints.ai_flags = AI_PASSIVE; // if is listener, need to be passive to wait
  }
  
  if (type == UDP_LISTENER) {
    if ((rv = getaddrinfo(SERVER, port, &hints, &servinfo)) != 0) {
      std::cerr << "getaddrinfo udp_listener: " << gai_strerror(rv) << "\n";
      return 1;
    }
  } else if (type == UDP_TALKER) {
    if ((rv = getaddrinfo(SERVER, port, &hints, &servinfo)) != 0) {
      std::cerr << "getaddrinfo udp_talker: " << gai_strerror(rv) << "\n";
      return 1;
    }
  }
    //loop for all results, connect the first available one
    for (p = servinfo; p != NULL; p = p->ai_next) {
    //perror when encounter error with socket return -1
    if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
      continue;
    }
    
    if (type == UDP_LISTENER) {
      if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
        close(sockfd);
        continue;
      }
    }
    
    break;
  }
  
  if (p == NULL) {
    std::cerr << "Failed to create UDP socket\n";
  }
  freeaddrinfo(servinfo);
  return sockfd;
}

int Socket::create_socket(int type) {
  int sockfd = 0;
  struct addrinfo hints, *servinfo, *p;
  int rv;
  
  memset(&hints, 0, sizeof(hints)); //make sure the struct is empty
  hints.ai_family = AF_INET; //no matter IPv4 or IPv6
  hints.ai_socktype = SOCK_STREAM; //using TCP stream
  
  if (type == TCP_SERVER) {
    hints.ai_flags = AI_PASSIVE;
  }
  
  if ((rv = getaddrinfo(SERVER, ADMISSION_PORT, &hints, &servinfo)) != 0) {
    std::cerr << "getaddrinfo: " << gai_strerror(rv) << "\n";
    return 1;
  }
  //loop for all results, connect the first available one
  for (p = servinfo; p != NULL; p = p->ai_next) {
    //perror when encounter error with socket return -1
    if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
      //perror("client: socket");
      continue;
    }
    //for client side, do the connection
    if (type == TCP_CLIENT) {
      if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
        close(sockfd);
        continue;
      }
    } else if (type == TCP_SERVER) {
      int yes = 1;
      if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
        exit(1);
      }
      //after successful set socket options, do the bind
      if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
        close(sockfd);
        continue;
      }
    }
    
    break;
  }
  //if the p is not valid
  if (p == NULL) {
    std::cerr << "Failed to connect\n";
    return -2;
  }
  
  freeaddrinfo(servinfo); //release the memory fro the struct
  return sockfd;
}

