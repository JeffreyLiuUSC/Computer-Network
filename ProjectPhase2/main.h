/*
 * main.h
 * Header and global variables
 */

#ifndef main_h
#define main_h

#define NUM_STUDENTS 5
#define NUM_DEPTS 3

#define DEPT_WAIT_LIMIT_SECOND 20     // seconds
#define DEPT_SLEEP_BETWEEN_PROGRAM 1  // second

// Admission's TCP hardcoded settings(USCID: 6044109 297)
#define ADMISSION_PORT "3597"
#define ADMISSION_BACKLOG 128

// Static UDP ports base settings(USCID: 6055109 297)
#define DEPARTMENT_BASE_UDP_PORT 21397
#define STUDENT_BASE_UDP_PORT 21697

// "Database"-like file path for persistence
#define DATABASE_FILE "database.txt"
#define MAXDATASIZE 1024

// Server address
#define SERVER "localhost"
#define LOCAL "localhost"

// And just some readability
#define TCP_CLIENT 1
#define TCP_SERVER 0

#define UDP_LISTENER 1
#define UDP_TALKER 0

#define PROCEED_WITH_ITERATION 1
#define NO_MORE_ITERATION 0

#include <arpa/inet.h>
#include <errno.h>
#include <math.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <fstream>
#include <set>
#include <sstream>
#include <vector>
#include <map>

#endif /* main_h */
