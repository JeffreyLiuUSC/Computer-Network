/*
 * StudentMessenger.hpp
 * Header of StudentMessenger.cpp
 */
#ifndef StudentMessenger_hpp
#define StudentMessenger_hpp

#include "main.h"

#define SMSG_P2_TCP_START 0
#define SMSG_APPLICATION_COMPLETED 1
#define SMSG_RECEIVED_REPLY 2
#define SMSG_P2_UDP_START 3
#define SMSG_RECEIVED_RESULT 4
#define SMSG_P2_END 5

class StudentMessenger {
public:
  StudentMessenger();
  void set_student_name(int);
  void display_tcp_ip(std::string, std::string);
  void display_udp_ip(std::string, std::string);
  void display_received_application_result();
  void display_all_applications_sent();
  void display_p2_end();
private:
  void display(int);
  void display(int, std::vector<std::string>*);
  int student_name;
};

#endif
