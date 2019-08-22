/*
** Admission socket
*/

#include <stdio.h>
#include "Admission.h"

void sigchld_handler(int s) 
{
  while(waitpid(-1, NULL, WNOHANG) > 0);
}

// Main program entry
int main() 
{
  create_empty_database_file(); //initialize a local .txt file to store the data from departments
  create_tcp_and_process(); //create socket
  return 0;
}

void create_empty_database_file() 
{
  std::ofstream fs;
  fs.open(DATABASE_FILE, std::ios::out);
  fs.close();
  db = new Database();
}

// TCP communication
void create_tcp_and_process() 
{
  srand(time(NULL));
  
  int sockfd = 0, new_fd;
  struct sockaddr_storage their_addr;
  socklen_t sin_size;
  
  char s[INET6_ADDRSTRLEN];
  int tcp_client_type = 0;
  
  struct sigaction sa;
  
  // create socket and bind and print to STDOUT
  sockfd = Socket::create_socket(TCP_SERVER); //created in Socket.cpp including creation setsocktopt and binding
  am->display_tcp_ip(Socket::get_socket_port(sockfd), Socket::get_self_ip_address()); //print
  
  // listen
  if (listen(sockfd, ADMISSION_BACKLOG) == -1) {
    perror("listen");
    exit(-1);
  }
  
  sa.sa_handler = sigchld_handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  if (sigaction(SIGCHLD, &sa, NULL) == -1) {
    perror("sigaction");
    exit(1);
  }
  
  if (PROJ_DEBUG) { //PROJ_DEBUG initialize with 0
    std::cout << "Admission Server: waiting for connections...\n";
  }
  
  // wait for incoming connections
  while(1) {
    sin_size = sizeof(their_addr);
    
    //accpeting phase
    new_fd = accept(sockfd, (struct sockaddr *) &their_addr, &sin_size);
    
    //if error happened, skip rest and keep accepting next one
    if (new_fd == -1) {
      continue;
    }
    
    if (PROJ_DEBUG) {
      std::cout << "Admission Server: got connection from " << s << "\n";
    }

    // incoming connections, now fork process
    if (!fork()) {
      // Child process don't need parent's socket, close it
      close(sockfd);
      int matching_program = 0;
      
      // wait for incoming packets
      while (1) {
	//receive phase
        int receive_length = (int) recv(new_fd, receive_buffer, MAXDATASIZE - 1, 0);
        char current_dept;
        if (receive_length > 0) {
	  std::string r_msg = debug_receive_buffer(receive_buffer, receive_length);
          
          // First packet received because tcp_client_type has not been set
          if (tcp_client_type == 0) {
            if (PROJ_DEBUG) {
	      std::cout << "Received introduction: " << r_msg << "\n";
            }

	    if (strcmp(r_msg.substr(0, 15).c_str(), "I_AM_DEPARTMENT") == 0) {
	      tcp_client_type = CLIENT_IS_DEPARTMENT;
	    } else {
	      std::cerr << "Unrecognized type: " << r_msg << "\n";
	    }

            // Now set tcp_client_type accordingly based on handshake message
            if (tcp_client_type > 0) {
              if (PROJ_DEBUG) {
		std::cout << "tcp_client_type set to " << tcp_client_type << "\n";
              }
              if (send(new_fd, "ADM_I_RCGZ", 10, 0) == -1) {
                perror("adm_recogniztion");
              }
            }
          }
          // This time child process knows who it's talking to
          else {
            int can_continue = 0;
            if (PROJ_DEBUG) {
	      std::cout << "Entering core communication phase with tcp_client_type " << tcp_client_type << "\n";
            }
            
            // Process the messages accordingly
            switch (tcp_client_type) {
	    case CLIENT_IS_DEPARTMENT:
	      can_continue = handle_department_messages(new_fd,
							r_msg.c_str(),
							&current_dept);
	      break;
	    default:
	      std::cerr << "tcp_client_type is not set: " << tcp_client_type << "\n";
            }
            // Check for communication termination
            if (can_continue == 0) {
              break;
            }
          }
        }
      }
      // Child process is complete
      close(new_fd);
      exit(0);
    } else {
      wait(NULL);
    }
    // parent process is complete
    close(new_fd);
    // Check if all students and departments have completed sending data
    if (db->check_is_complete()) {
      am->display_phase1_completed();
      break;
    }
  }
}

int handle_department_messages(int new_fd, const char *msg, char *current_dept) {
  // Signals end of transmission from Department
  if (strcmp(msg, "TX_FIN") == 0) {
    if (PROJ_DEBUG) {
      std::cout << "TX_FIN signal received. Closing socket " << new_fd << "\n";
    }
    am->display_department_completed(*current_dept);
    return NO_MORE_ITERATION;
  }
  // Data packet from Department
  else {
    int rand_wait = rand() % 500 + 500;
    usleep(rand_wait);
    int pdm_error;

    pdm_error = process_department_message(msg);
    
    if (pdm_error == 0) {
      if (PROJ_DEBUG) {
	std::cout << "No PDM error\n";
      }
      *current_dept = receive_buffer[0];
      
      if (PROJ_DEBUG) {
	std::cout << "Setting currend dept to " << *current_dept << "\n";
      }
      
      // Reply with acknowledgement
      if (send(new_fd, "ADM_RX_OK", 9, 0) == -1) {
        perror("dept ack");
      } else {
        if (PROJ_DEBUG) {
	  std::cout << "ADM_RX_OK sent out after randomly waiting for " << rand_wait << " us\n";
        }
      }
    }
    
    return PROCEED_WITH_ITERATION;
  }
}

uint32_t process_department_message(std::string _buffer) {
  char *element;
  uint32_t e_pos = 0;
  char *buffer = new char[_buffer.length() + 1];
  strcpy(buffer, _buffer.c_str());
  element = strtok((char*) buffer, "#");
  std::string dept_program = "";
  float dept_program_min_gpa = 0;
  
  while (element != NULL) {
     switch(e_pos) {
     case 0:
       dept_program = element;
       break;
     case 1:
       dept_program_min_gpa = atof(element);
       break;
    }
    element = strtok(NULL, "#");
    e_pos++;
  }
  
  std::ofstream f;
  f.open(DATABASE_FILE, std::ios::app);
  char line_buffer[MAXDATASIZE];
  
  sprintf(line_buffer, "%s#%.1f\n", dept_program.c_str(), dept_program_min_gpa);
  f << line_buffer;
  f.close();
  return 0;
}

std::string debug_receive_buffer(char *receive_buffer, int receive_length) {
  std::string result = "";
  
  if (PROJ_DEBUG) {
    std::cout << "Received message (" << receive_length << " bytes): ";
  }
  
  for (int i = 0; i < receive_length; i++) {
    if (PROJ_DEBUG) {
      printf("%c", receive_buffer[i]);
    }
    result += receive_buffer[i];
  }
  
  if (PROJ_DEBUG) {
    std::cout << "\n";
  }
  
  return result;
}

std::string get_client_ip_address(int sockfd) {
  struct sockaddr_storage addr;
  char ipstr[INET6_ADDRSTRLEN];
  socklen_t addrlen = sizeof(addr);
  
  getpeername(sockfd, (struct sockaddr *) &addr, &addrlen);
  struct sockaddr_in *s = (struct sockaddr_in *) &addr;
  
  inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof(ipstr));
  return (std::string) ipstr;
}


