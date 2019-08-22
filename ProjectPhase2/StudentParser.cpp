/*
 * StudentParser.cpp
 * Helper class of Student class
 */

#include "StudentParser.hpp"

StudentParser::StudentParser(uint32_t id) {
  this->id = id;
  std::sprintf(this->filepath, "%s%d.txt", FILENAME_PREFIX, id);
  this->gpa = 0;
  this->interests = new std::vector<std::string>();
  this->parse();
}

void StudentParser::parse() {
  std::string line = *new std::string();
  std::ifstream ifs(this->filepath, std::ifstream::in);
  uint32_t token_id = 0;
  uint32_t state = 0;
  char *element;
  
  if (ifs.is_open()) {
    while(std::getline(ifs, line)) {
      token_id = 0;
      element = std::strtok((char*) line.c_str(), TOKEN_SEPARATOR);
      if (element != NULL) {
        while (element != NULL) {
          switch(token_id) {
            case 0:
              if (strcmp(element, TOKEN_GPA) == 0) {
                state = 1;
              } else if (strcmp(((std::string) element).substr(0,8).c_str(), TOKEN_INTEREST) == 0) {
                state = 2;
              }
              break;
            case 1:
              switch (state) {
                case 1: this->gpa = atof(element); break;
                case 2: this->interests->push_back(element); break;
              }
              break;
          }
          token_id++;
          element = strtok(NULL, TOKEN_SEPARATOR);
        }
      }
    }
  }
}
