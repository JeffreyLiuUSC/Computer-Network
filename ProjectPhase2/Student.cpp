/*
 * Student.cpp
 */

#include <stdio.h>
#include "main.h"
#include "Student.h"
#include "StudentParser.hpp"

int main() {
  spawn_iterative(NUM_STUDENTS);
  sleep(2);
  return 0;
}

void spawn_one() {
  do_work(1);
}

void spawn_iterative(int n) {
  for (int i = 0; i < n; i++) {
    switch(fork()) {
      case 0:
        do_work(i + 1);
        exit(0);
        break;
      default:
        break;;
    }
  }
  wait(NULL);
}

void do_work(uint32_t id) {
  StudentParser *sp = new StudentParser(id);
  sm = new StudentMessenger();
  sm->set_student_name(id);
  
  // Set up TCP socket and send data of students applications
  int wait_for_admission = connect_to_admission_server(sp, id);
  if (wait_for_admission == 1) {
    // Set up UDP listener socket and wait for Admission's result
    wait_for_admission_response(id);
  }
  sm->display_p2_end();
  exit(0);
}

void wait_for_admission_response(uint32_t student_id) {
  char port_s[MAXDATASIZE] = "";
  int student_port = STUDENT_BASE_UDP_PORT + 100 * (student_id - 1);
  sprintf(port_s, "%d", student_port);
  char buffer[MAXDATASIZE] = "";
  int numbytes = 0;
  struct sockaddr_storage their_addr;
  socklen_t addr_len;

  int rv;
  int sockfd = 0;
  struct addrinfo hints, *servinfo, *p;
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_flags = AI_PASSIVE;

  // Set up UDP socket
  if ((rv = getaddrinfo(LOCAL, port_s, &hints, &servinfo)) != 0) {
    std::cerr << "getaddrinfo: " << gai_strerror(rv);
  }
  
  for (p = servinfo; p != NULL; p = p->ai_next) {
    if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
      perror("listener: socket");
      continue;
    }
    
    if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
      close(sockfd);
      continue;
    }
    
    break;
  }
  
  if (p == NULL) {
    perror("listener");
  }
  
  freeaddrinfo(servinfo);
  
  memset(&hints, 0, sizeof(hints));

  // Now listen to incoming UDP packets
  while(true) {
    if ((numbytes = recvfrom(sockfd, buffer, MAXDATASIZE - 1, 0, (struct sockaddr *) &their_addr, &addr_len)) == -1) {
      std::cout << "Student " << student_id << " encountered recvfrom error\n";
      perror("recvfrom");
      break;
    }
    
    if (numbytes > 0) {
      buffer[numbytes] = '\0';
      // Admission has finished sending results
      if (strcmp(buffer, "ADM_END") == 0) {
        sm->display_udp_ip(Socket::get_socket_port(sockfd), Socket::get_self_ip_address());
        sm->display_received_application_result();
        break;
      }
    }
  }
  
  close(sockfd);
}

// TCP client socket
int connect_to_admission_server(StudentParser *sp, uint32_t student_id) {
  // Create TCP socket
  int sockfd = Socket::create_socket(TCP_CLIENT);
  sm->display_tcp_ip(Socket::get_socket_port(sockfd), Socket::get_self_ip_address());
  
  // Send data
  int proceed_to_wait_decision = send_data_to_admission_server(student_id, sockfd, sp);
  sm->display_all_applications_sent();
  
  // Close TCP socket and wait for UDP results
  close(sockfd);
  return proceed_to_wait_decision;
}

int send_data_to_admission_server(int student_id, int sockfd, StudentParser *sp) {
  char s_msg[MAXDATASIZE];
  char adm_resp[MAXDATASIZE];
  int numbytes = 0;
  int proceed_to_wait_decision = 0;
  
  // Send packet to introduce self
  sprintf(s_msg, "I_AM_STUDENT#%d", student_id);
  send(sockfd, s_msg, strlen(s_msg), 0);
  
  // Wait for reply from Admission
  while(1) {
    recv(sockfd, adm_resp, sizeof(adm_resp), 0);
    
    if (strcmp(((std::string) adm_resp).substr(0, 10).c_str(), "ADM_I_RCGZ") == 0) {
      break;
    }
  }
  
  // Send GPA and wait for acknowledgement
  sprintf(s_msg, "%d#GPA#%.1f", student_id, sp->gpa);
  send(sockfd, s_msg, strlen(s_msg), 0);
  while(1) {
    recv(sockfd, adm_resp, sizeof(adm_resp), 0);
    
    if (strcmp(((std::string) adm_resp).substr(0, 9).c_str(), "ADM_RX_OK") == 0) {
      break;
    }
  }
  
  // Iterate through all student
  for (std::vector<std::string>::iterator s = sp->interests->begin(); s != sp->interests->end(); ++s) {
    sprintf(s_msg, "%d#%s", student_id, s->c_str());
    send(sockfd, s_msg, strlen(s_msg), 0);
    
    // Wait for acknowledgement
    while(1) {
      recv(sockfd, adm_resp, sizeof(adm_resp), 0);
      
      if (strcmp(((std::string) adm_resp).substr(0, 9).c_str(), "ADM_RX_OK") == 0) {
        break;
      }
    }
  }

  // Tell Admission all data has been transmitted
  sprintf(s_msg, "TX_FIN");
  send(sockfd, s_msg, strlen(s_msg), 0);
  
  // Waiting for admission's respond whether there are matching interest or not
  while(1) {
    numbytes = recv(sockfd, adm_resp, sizeof(adm_resp), 0);
    adm_resp[numbytes] = '\0';
    
    if (strcmp(adm_resp, "valid") == 0) {
      proceed_to_wait_decision = 1;
    }
    break;
  }
  return proceed_to_wait_decision;
}
