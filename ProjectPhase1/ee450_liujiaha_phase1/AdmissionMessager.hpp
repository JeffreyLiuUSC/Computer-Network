/*
** AdmissionMessager.hpp
** Header of AdmissionMessager.cpp
*/

#ifndef AdmissionMessager_hpp
#define AdmissionMessager_hpp

#include "main.h"

#define AMSG_P1_START 0
#define AMSG_DEPT_COMPLETED 1
#define AMSG_P1_END 2
#define AMSG_DEPARTMENT_RESULT 7

class AdmissionMesseger {
public:
  AdmissionMesseger();
  int display_tcp_ip(std::string, std::string);
  void display_department_completed(char);
  std::string display_phase1_completed();
  int redisplay_tcp_ip();
private:
  int display(int);
  int display(int, std::vector<std::string>*);  
  std::string reuse_tcp_port;
  std::string reuse_ip_address;
};

#endif /* AdmissionMessager.hpp */
