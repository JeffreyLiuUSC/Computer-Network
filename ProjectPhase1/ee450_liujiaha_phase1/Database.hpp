/*
** Database.hpp
** Header of Database.cpp
*/
#ifndef Database_hpp
#define Database_hpp

#include "main.h"

#define DB_FILE "database.txt"

class Database {
public:
  Database();
  bool check_is_complete();
  bool check_is_complete(bool);
  void build();
  std::set<char> *participating_departments;
private:
  void process_department_data(std::string);
  void debug_database();
  std::map<std::string, float> *department_programs;
};

#endif
