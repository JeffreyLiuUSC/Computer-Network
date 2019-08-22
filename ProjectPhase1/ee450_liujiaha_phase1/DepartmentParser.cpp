/*
** DepartmentParser.cpp
** Helper of Department class
*/

#include "DepartmentParser.hpp"

DepartmentParser::DepartmentParser(std::string name) {
  this->dept_name = name;
  std::sprintf(this->filepath, "%s%s.txt", FILENAME_PREFIX, this->dept_name.c_str());
  this->requirements = new std::map<std::string, float>();
  
  this->parse();
}

void DepartmentParser::parse() {
  std::string line = *new std::string();
  std::ifstream ifs(this->filepath, std::ifstream::in);
  uint32_t token_id = 0;

  std::string dept_program;
  float gpa_req;
  char *element;
  
  if (ifs.is_open()) {
    while(std::getline(ifs, line)) {
      token_id = 0;
      element = std::strtok((char*) line.c_str(), TOKEN_SEPARATOR);
      
      if (element != NULL) {
        while (element != NULL) {
          switch(token_id) {
	  case 0:
	    dept_program = (std::string) element;
	    break;
	  case 1:
	    gpa_req = atof(element);
	    requirements->insert(std::make_pair(dept_program, gpa_req));
	    break;
          }
          token_id++;
          element = strtok(NULL, TOKEN_SEPARATOR);
        }
      }
    }
    
  } else {
    if (PROJ_DEBUG) {
      std::cerr << "Failed to open file " << this->filepath << "\n";
    }
  }

}
