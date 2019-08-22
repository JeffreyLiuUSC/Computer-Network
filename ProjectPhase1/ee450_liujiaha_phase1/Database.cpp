/*
** Database.cpp
** Storing the data from departments
*/
#include "Database.hpp"

Database::Database() {
  
}

// Check whether all Students and Departments have completed
// transmitting data
// When execute_build is true, build database
bool Database::check_is_complete(bool execute_build) {
  std::set<std::string> *members = new std::set<std::string>();
  std::ifstream infile(DATABASE_FILE);
  std::string line;
  int limit =  NUM_OF_DEPTS;
  while(std::getline(infile, line)) {
    std::string name = "";
    name = line[0];
    if (execute_build) {
      process_department_data(line);
    }    
    members->insert(name);
  }
  
  if (members->size() == limit) {
    return true;
  }
  
  infile.close();
  return false;
}

bool Database::check_is_complete() {
  return Database::check_is_complete(false);
}

void Database::build() {
  department_programs = new std::map<std::string, float>();
  participating_departments = new std::set<char>();
  if (PROJ_DEBUG) {
    debug_database();
  }
}

// Build database by processing Department data
void Database::process_department_data(std::string x) {
  char *element;
  uint32_t e_pos = 0;
  char *buffer = new char[x.length() + 1];
  strcpy(buffer, x.c_str());
  element = strtok((char*) buffer, "#");
  std::string dept_program = "";
  float dept_program_min_gpa = 0;
  
  while (element != NULL) {
    switch(e_pos) {
    case 0:
      dept_program = element;
      participating_departments->insert(element[0]);
      break;
    case 1:
      dept_program_min_gpa = atof(element);
      break;
      }
    element = strtok(NULL, "#");
    e_pos++;
  }
  
  if (department_programs->find(dept_program) == department_programs->end()) {
    department_programs->insert(std::make_pair(dept_program, dept_program_min_gpa));
  } else {
    department_programs->at(dept_program) = dept_program_min_gpa;
  }
}

void Database::debug_database() {
  for (std::map<std::string, float>::iterator r = department_programs->begin(); r!= department_programs->end(); ++r) {
    std::cout << "Department Program " << r->first << " min GPA: " << r->second << "\n";
  }
}
