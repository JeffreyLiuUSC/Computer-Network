/*
 * Department.cpp
 */

#include <stdio.h>
#include "main.h"
#include "Department.h"

int main() {
  spawn_iterative(NUM_DEPTS);
  sleep(2);
  return 0;
}

void spawn_one() {
  do_work(0x41);
}

void spawn_iterative(int n) {
  for (int i = 0; i < n; i++) {
    switch(fork()) {
      case 0:
        do_work(i + 0x41);
        exit(0);
        break;
      case -1:
        std::cerr << "Fork error at iteration " << i << "\n";
        break;
      default:
        break;;
    }
  }
  wait(NULL);
}

int do_work(char d) {
  std::string dept_name = "";
  dept_name += d;
  DepartmentParser *dp = new DepartmentParser(dept_name);
  dm = new DepartmentMessenger();
  dm->set_department_name(dept_name);

  // Set up TCP connection for Phase1
  int connection_status = connect_to_admission_server(dp, d);
  // Set up UDP connection for Phase2
  wait_for_admission_response(d);
  return connection_status;
}

void wait_for_admission_response(char department_id) {
  char port_s[MAXDATASIZE] = "";
  int department_port = DEPARTMENT_BASE_UDP_PORT + 100 * (department_id - 0x41);
  sprintf(port_s, "%d", department_port);
  char buffer[MAXDATASIZE] = "";
  int numbytes = 0;
  struct sockaddr_storage their_addr;
  bool udp_ip_displayed = false;
  socklen_t addr_len;
  
  int rv;
  int sockfd = 0;
  struct addrinfo hints, *servinfo, *p;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_flags = AI_PASSIVE;
  
  // Setup UDP socket
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
  
  // Now listen to incoming UDP packets
  while(true) {
    if ((numbytes = recvfrom(sockfd, buffer, MAXDATASIZE - 1, 0, (struct sockaddr *) &their_addr, &addr_len)) == -1) {
      std::cout << "Using port " << port_s << "\n";
      std::cout << "Department " << department_id << " encountered recvfrom error\n";
      perror("recvfrom");
      break;
    }
    
    if (!udp_ip_displayed) {
      dm->display_udp_ip(Socket::get_socket_port(sockfd), Socket::get_self_ip_address());
      udp_ip_displayed = true;
    }
    
    if (numbytes > 0) {
      buffer[numbytes] = '\0';
      
      // Admission is done sending admitted students data
      if (strcmp(buffer, "ADM_END") == 0) {
        break;
      }
      
      // Admitted student data received
      else {
        char element[MAXDATASIZE];
        strncpy(element, buffer, 8);
        element[8] = '\0';
        
        dm->display_student_admitted(element);
      }
    }
  }
  
  close(sockfd);
  dm->display_phase2_completed();
}

//TCP client socket
int connect_to_admission_server(DepartmentParser *dp, char dept_name) {
  // Create TCP socket
  int sockfd = Socket::create_socket(TCP_CLIENT);
  dm->display_tcp_ip(Socket::get_socket_port(sockfd), Socket::get_self_ip_address());
  dm->display_connected();
  
  // Send data
  send_data_to_admission_server(dept_name, sockfd, dp);
  dm->display_phase1_completed();

  // Close TCP socket
  close(sockfd);
  return 0;
}

int send_data_to_admission_server(char dept_name, int sockfd, DepartmentParser *dp) {
  int len = 0;
  int bytes_sent = 0;
  char d_msg[MAXDATASIZE];
  char adm_resp[MAXDATASIZE];

  sprintf(d_msg, "I_AM_DEPARTMENT#%c", dept_name);
  len = (int) strlen(d_msg);
  send(sockfd, d_msg, len, 0);
  
  // Wait for Admission reply
  while(1) {
    recv(sockfd, adm_resp, sizeof(adm_resp), 0);
    
    if (strcmp(((std::string) adm_resp).substr(0, 10).c_str(), "ADM_I_RCGZ") == 0) {
      break;
    }
  }
  
  // Iterate through all program data
  for (std::map<std::string, float>::iterator r = dp->requirements->begin(); r != dp->requirements->end(); ++r) {
    
    // Send one program data
    sprintf(d_msg, "%s#%.1f", r->first.c_str(), r->second);
    len = (int) strlen(d_msg);
    bytes_sent = (int) send(sockfd, d_msg, len, 0);
    
    dm->display_one_program_sent(r->first);
    
    // Wait for acknowledgement
    while(1) {
      recv(sockfd, adm_resp, sizeof(adm_resp), 0);

      if (strcmp(((std::string) adm_resp).substr(0, 9).c_str(), "ADM_RX_OK") == 0) {
        break;
      }
    }
  }
  
  // Tell Admission all data has been transmitted
  sprintf(d_msg, "TX_FIN");
  bytes_sent = send(sockfd, d_msg, len, 0);
  
  dm->display_all_program_sent();
  
  return 0;
}
