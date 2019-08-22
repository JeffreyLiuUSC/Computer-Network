/*
 * Department.h
 * Header of Department.cpp
 */

#ifndef Department_h
#define Department_h
#include "DepartmentMessenger.hpp"
#include "DepartmentParser.hpp"
#include "Socket.hpp"

int do_work(char);
int connect_to_admission_server(DepartmentParser*, char);
int send_data_to_admission_server(char, int, DepartmentParser*);
void wait_for_admission_response(char);
void spawn_iterative(int);
void spawn_one();

std::string get_socket_port(int);
DepartmentMessenger *dm = new DepartmentMessenger();
std::vector<int> *child_processes;

#endif /* Department_h */
