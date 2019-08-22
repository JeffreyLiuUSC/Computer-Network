CXX := g++

CXXFLAGS := -lnsl -lresolv

all: AdmissionAll DepartmentAll StudentAll

mac: Admission Department Student

clean:
	rm *.o Student Admission Department database.txt

AdmissionAll: Admission.o AdmissionMessenger.o Database.o Socket.o
	$(CXX) $^ -o Admission $(CXXFLAGS)

DepartmentAll: Department.o DepartmentParser.o DepartmentMessenger.o Socket.o
	$(CXX) $^ -o Department $(CXXFLAGS)

StudentAll: Student.o StudentParser.o StudentMessenger.o Socket.o
	$(CXX) $^ -o Student $(CXXFLAGS)

Admission: Admission.o AdmissionMessenger.o Database.o Socket.o
	$(CXX) $^ -o $@

Department: Department.o DepartmentParser.o DepartmentMessenger.o Socket.o
	$(CXX) $^ -o $@

Student: Student.o StudentParser.o StudentMessenger.o Socket.o
	$(CXX) $^ -o $@

Admission.o: Admission.cpp Admission.h
	$(CXX) -c $< -o $@

AdmissionMessenger.o: AdmissionMessenger.cpp AdmissionMessenger.hpp
	$(CXX) -c $< -o $@

Department.o: Department.cpp main.h Department.h
	$(CXX) -c $< -o $@

DepartmentMessenger.o: DepartmentMessenger.cpp DepartmentMessenger.hpp
	$(CXX) -c $< -o $@

DepartmentParser.o: DepartmentParser.cpp DepartmentParser.hpp
	$(CXX) -c $< -o $@

Student.o: Student.cpp main.h Student.h
	$(CXX) -c $< -o $@

StudentMessenger: StudentMessenger.cpp Student.hpp
	$(CXX) -c $< -o $@

StudentParser.o: StudentParser.cpp StudentParser.hpp
	$(CXX) -c $< -o $@

Database.o: Database.cpp Database.hpp
	$(CXX) -c $< -o $@

Socket.o: Socket.cpp Socket.hpp
	$(CXX) -c $< -o $@