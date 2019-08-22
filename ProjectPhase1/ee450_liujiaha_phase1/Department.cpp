/*
** Department.cpp
*/

#include <stdio.h>
#include "main.h"
#include "Department.h"

int main() {
  spawn_iterative(NUM_OF_DEPTS);
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
      do_work(i + 0x41); // ASCII: int(0) to char(A)
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
  std::string dept_name = ""; dept_name += d;
  DepartmentParser *dp = new DepartmentParser(dept_name);
  dm = new DepartmentMessenger();
  dm->set_department_name(dept_name);
  
  if (PROJ_DEBUG) {
    std::cout << "Process " << ::getpid() << " created for department " << d << "\n";
  }
  
  // Set up TCP connection and process data
  int connection_status = connect_to_admission_server(dp, d);
  
  if (PROJ_DEBUG) {
    std::cout << "Process " << ::getpid() << " returned\n";
  }
  
  return connection_status;
}

//TCP client socket
int connect_to_admission_server(DepartmentParser *dp, char dept_name) {
  // Create TCP socket
  int sockfd = Socket::create_socket(TCP_CLIENT);
  dm->display_tcp_ip(Socket::get_socket_port(sockfd), Socket::get_self_ip_address());
  dm->display_connected();
  
  // Sent data
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
    
    if (PROJ_DEBUG) {
      std::cout << "Received " << adm_resp << "\n";
    }
    
    if (strcmp(((std::string) adm_resp).substr(0, 10).c_str(), "ADM_I_RCGZ") == 0) {
      if (PROJ_DEBUG) {
	std::cout << "ADM_I_RCGZ received \n";
      }
      break;
    }
  }
  
  if (PROJ_DEBUG) {
    std::cout << "Admission has recognized department. Now sending data...\n";
  }
  
  // Iterate through all program data
  for (std::map<std::string, float>::iterator r = dp->requirements->begin(); r != dp->requirements->end(); ++r) {
    
    // Send one program data
    sprintf(d_msg, "%s#%.1f", r->first.c_str(), r->second);
    len = (int) strlen(d_msg);
    bytes_sent = (int) send(sockfd, d_msg, len, 0);
    
    dm->display_one_program_sent(r->first);
    
    if (PROJ_DEBUG) {
      std::cout << bytes_sent << " bytes sent: " << d_msg << "\n";
      std::cout << "Waiting for acknowledgement from Admission Server...\n";
    }
    
    // Wait for acknowledgement
    while(1) {
      recv(sockfd, adm_resp, sizeof(adm_resp), 0);
      
      if (PROJ_DEBUG) {
        std::cout << "Received " << adm_resp << "\n";
      }
      if (strcmp(((std::string) adm_resp).substr(0, 9).c_str(), "ADM_RX_OK") == 0) {
        if (PROJ_DEBUG) {
          std::cout << "ADM_RX_OK received\n";
        }
        break;
      }
    }
  }
  
  // Tell Admission all data has been transmitted
  sprintf(d_msg, "TX_FIN");
  bytes_sent = send(sockfd, d_msg, len, 0);
  
  if (PROJ_DEBUG) {
    std::cout << bytes_sent << " bytes sent: " << d_msg << "\n";
    std::cout << "Done transmitting!\n";
  }
  
  dm->display_all_program_sent();
  
  return 0;
}
