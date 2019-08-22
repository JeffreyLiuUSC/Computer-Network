/*
** main.h
** Header and global variables 
*/

#ifndef main_h
#define main_h

#define PROJ_DEBUG 0        // enable STDOUT debug
#define NUM_OF_DEPTS 3

// Admission's TCP hardcoded settings(uscid: 6044109297)
#define ADMISSION_PORT "3597"
#define ADMISSION_BACKLOG 128

// "Database"-like file path for persistence
#define DATABASE_FILE "database.txt"
#define MAXDATASIZE 1024

// Server address
#define SERVER "localhost"
#define LOCAL "localhost"

#define TCP_CLIENT 1
#define TCP_SERVER 0

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
#include <string.h>
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
