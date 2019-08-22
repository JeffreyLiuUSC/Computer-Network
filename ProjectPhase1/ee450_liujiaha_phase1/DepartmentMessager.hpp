/*
** DepartmentMessager.h
** Header of DepartmentMessager.cpp
*/

#ifndef DepartmentMessager_hpp
#define DepartmentMessager_hpp

#include "main.h"

#define DMSG_P1_START 0
#define DMSG_ADM_CONNECTED 1
#define DMSG_PROG_SENT 2
#define DMSG_PROG_COMPLETED 3
#define DMSG_P1_END 4

class DepartmentMessenger {
public:
  DepartmentMessenger();
  void set_department_name(std::string);
  void display_tcp_ip(std::string, std::string);
  void display_connected();
  void display_one_program_sent(std::string);
  void display_all_program_sent();
  void display_phase1_completed();
private:
  void display(int);
  void display(int, std::vector<std::string>*);
  std::string department_name;
};

void *get_in_addr(struct sockaddr *sa);
#endif
