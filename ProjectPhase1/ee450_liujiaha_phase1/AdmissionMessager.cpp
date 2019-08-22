/*
** AdimissionMessager.cpp
** print the messages of admission center
*/

#include "AdmissionMessager.hpp"

AdmissionMesseger::AdmissionMesseger() 
{

}

int AdmissionMesseger::display_tcp_ip(std::string port, std::string ip) 
{
  std::vector<std::string> *args = new std::vector<std::string>();
  args->push_back(ip);
  args->push_back(port);
  reuse_tcp_port = port;
  reuse_ip_address = ip;
  display(AMSG_P1_START, args);
}



int AdmissionMesseger::redisplay_tcp_ip() {
  std::vector<std::string> *args = new std::vector<std::string>();
  args->push_back(reuse_ip_address);
  args->push_back(reuse_tcp_port);
}


void AdmissionMesseger::display_department_completed(char dept_name) {
  std::vector<std::string> *args = new std::vector<std::string>();
  std::string dept_name_s = "";
  dept_name_s = dept_name;
  args->push_back(dept_name_s);
  display(AMSG_DEPT_COMPLETED, args);
}


std::string AdmissionMesseger::display_phase1_completed() {
  display(AMSG_P1_END);
}

int AdmissionMesseger::display(int type) {
  display(type, NULL);
}

int AdmissionMesseger::display(int type, std::vector<std::string> *args) {
  switch(type) {
  case AMSG_P1_START:
    std::cout << "The admission office has TCP port "
	      << args->at(1)
	      << " and IP address " << args->at(0) 
	      << "\n";
    break;
  case AMSG_DEPT_COMPLETED:
    std::cout << "Received the program list from Department"
	      << args->at(0) << "\n";
    break;
  case AMSG_P1_END:
    std::cout << "End of Phase 1 for the admission office\n";
    break;
  case AMSG_DEPARTMENT_RESULT:
    std::cout << "The admission office has send one admitted student to Department"
	      << args->at(0)
	      << "\n";
    break;
  }
}
