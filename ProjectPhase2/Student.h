/*
 * Student.h
 * Header of Student.cpp
 */

#ifndef Student_h
#define Student_h

#include "main.h"
#include "StudentParser.hpp"
#include "StudentMessenger.hpp"
#include "Socket.hpp"

void spawn_iterative(int);
void do_work(uint32_t);
void spawn_one();

int connect_to_admission_server(StudentParser*, uint32_t);
int send_data_to_admission_server(int, int, StudentParser*);
void wait_for_admission_response(uint32_t);

StudentMessenger *sm;

#endif /* Student_h */
