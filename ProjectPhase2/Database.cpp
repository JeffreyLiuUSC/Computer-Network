/*
 * Database.cpp
 * Storing corresponding data from departments and students
 */

#include "Database.hpp"

Database::Database() {
  
}

// Check if student's interest matches programs
bool Database::has_program(char *x) {
  std::ifstream infile(DATABASE_FILE);
  std::string line;
  bool is_valid_program = false;
  
  while(std::getline(infile, line)) {
    char name[3] = "";
    if (line[0] == 'S') {
      continue;
    } else {
      name[0] = line[0];
      name[1] = line[1];
      name[2] = '\0';
      
      if (strcmp((const char*) x, (const char*) name) == 0) {
        is_valid_program = true;
        break;
      }
    }
  }
  
  infile.close();
  return is_valid_program;
}

// When all data has been sent from departments and students, build database
bool Database::check_is_complete(bool execute_build) {
  std::set<std::string> *members = new std::set<std::string>();
  std::ifstream infile(DATABASE_FILE);
  std::string line;
  int limit = NUM_DEPTS + NUM_STUDENTS;
  
  while(std::getline(infile, line)) {
    std::string name = "";

    if (line[0] == 'S') {               // if data from students
      name = line[0] + line[1];
      if (execute_build) {
        process_student_data(line);
      }
    } else {                            // if data from departments
      name = line[0];
      if (execute_build) {
        process_department_data(line);
      }
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
  student_grades = new std::map<int, float>();
  student_interests = new std::map<int, std::vector<std::string>*>();
  department_programs = new std::map<std::string, float>();
  participating_departments = new std::set<char>();
  decision = new std::vector<std::string>();
  
  check_is_complete(true);
}

// Build database by processing Students data
void Database::process_student_data(std::string x) {
  char *element;
  uint32_t e_pos = 0;
  char *buffer = new char[x.length() + 1];
  strcpy(buffer, x.c_str());
  element = strtok((char*) buffer, "#");
  int student_id = -1;
  bool parse_gpa = false;
  std::string interest = "";
  std::ofstream f;
  float student_gpa = 0.0;
  
  while (element != NULL) {
    switch(e_pos) {
      case 0:
        student_id = element[1] - 0x30;
        break;
      case 1:
        if (strcmp(element, "GPA") == 0) {
          parse_gpa = true;
        } else {
          interest = element;
        }
        break;
      case 2:
        if (parse_gpa) {
          student_gpa = atof(element);
        }
        break;
      default:
        std::cerr << "Shouldn't reach here\n";
    }
    
    element = strtok(NULL, "#");
    e_pos++;
  }
  
  if (parse_gpa) {
    if (student_grades->find(student_id) == student_grades->end()) {
      // New student
      student_grades->insert(std::make_pair(student_id, student_gpa));
    } else {
      // Updating student's information
      student_grades->at(student_id) = student_gpa;
    }
  } else {
    if (student_interests->find(student_id) == student_interests->end()) {
      // No matching
      std::vector<std::string> *k = new std::vector<std::string>();
      student_interests->insert(std::make_pair(student_id, k));
    }
    
    // Insert new record
    student_interests->at(student_id)->push_back(interest);
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

// Making decision of admission
void Database::make_decision() {
  for (std::map<int, float>::iterator g = student_grades->begin(); g != student_grades->end(); ++g) {
    int student_id = g->first;
    int matching_interest = 0;
    char student_id_s[MAXDATASIZE] = "";
    float student_gpa = g->second;
    bool admitted = false;
    
    sprintf(student_id_s, "%d", student_id);
    
    // Iterate through each students
    for (std::vector<std::string>::iterator p = student_interests->at(student_id)->begin(); p != student_interests->at(student_id)->end() && !admitted; ++p) {
      
      // Check their interests match any of the offered programs
      if (department_programs->find(*p) != department_programs->end()) {
        matching_interest++;
        std::string program_application = *p;
        float min_gpa = department_programs->at(*p);
        
        int min_gpa_rd = round(min_gpa * 10);
        int student_gpa_rd = round(student_gpa * 10);
        
        // Now check the GPA requirement
        if (student_gpa_rd >= min_gpa_rd) {
          std::string dec_s = "";
          dec_s += (std::string) student_id_s + "#Accept#" + *p + "#department" + p->at(0);
          
          // Insert new admittance record to decision table
          decision->push_back(dec_s);
          // Admit student. No longer need to iterate through
          admitted = true;
        }
      }
    }
    
    // Only when student has matching interest but not yet admitted
    // Insert new rejection record to decision table
    if (!admitted && matching_interest > 0) {
      std::string dec_s = "";
      dec_s += (std::string) student_id_s + "#Reject";
      decision->push_back(dec_s);
    }
  }
}

void Database::debug_database() {
    for (std::map<int, float>::iterator g = student_grades->begin(); g != student_grades->end(); ++g) {
        std::cout << "Student" << g->first << " GPA: " << g->second << "\n";
    }

    for (std::map<int, std::vector<std::string>*>::iterator s = student_interests->begin(); s != student_interests->end(); ++s) {
        std::cout << "Student" << s->first << " Interests: ";
        for (std::vector<std::string>::iterator ss = s->second->begin(); ss != s->second->end(); ++ss) {
            std::cout << ss->c_str() << " ";
        }
        std::cout << "\n";
    }

    for (std::map<std::string, float>::iterator r = department_programs->begin(); r!= department_programs->end(); ++r) {
        std::cout << "Department Program " << r->first << " min GPA: " << r->second << "\n";
    }
}

void Database::debug_decision() {
    for (std::vector<std::string>::iterator d = decision->begin(); d != decision->end(); ++d) {
        std::cout << *d << "\n";
    }
}

