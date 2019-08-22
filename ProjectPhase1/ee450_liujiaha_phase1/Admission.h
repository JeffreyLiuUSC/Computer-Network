/*
** Admission.h
** Header for admission.cpp
*/

#ifndef Admission_h
#define Admission_h

#define CLIENT_IS_DEPARTMENT 1
#define CLIENT_IS_STUDENT 2

#include "main.h"
#include "AdmissionMessager.hpp"
#include "Socket.hpp"
#include "Database.hpp"

void create_empty_database_file();
void create_tcp_and_process();

std::string debug_receive_buffer(char*, int);
uint32_t process_department_message(std::string);

bool check_department_completion(int*, int*);

int handle_department_messages(int, const char*, char*);
AdmissionMesseger *am = new AdmissionMesseger();

std::map<std::string, float> *database;
std::vector<std::string> *interest_database;

bool build_database();

char receive_buffer[MAXDATASIZE];
int receive_length;
Database *db;
#endif /* Admission_h */
