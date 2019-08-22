CXX := g++

all: Admission Department

clean:
	$(RM) Admission Department *.o

Admission: Admission.o AdmissionMessager.o Database.o Socket.o
	$(CXX) $^ -o $@
Department: Department.o DepartmentParser.o DepartmentMessager.o Socket.o
	$(CXX) $^ -o $@
Socket.o: Socket.cpp Socket.hpp
	$(CXX) -c $< -o $@
Department.o: Department.cpp main.h Department.h
	$(CXX) -c $< -o $@
DepartmentMessager.o: DepartmentMessager.cpp DepartmentMessager.hpp
	$(CXX) -c $< -o $@
DepartmentParser.o: DepartmentParser.cpp DepartmentParser.hpp
	$(CXX) -c $< -o $@
Admission.o: Admission.cpp Admission.h
	$(CXX) -c $< -o $@
Database.o: Database.cpp Database.hpp
	$(CXX) -c $< -o $@
AdmissionMessager.o: AdmissionMessager.cpp AdmissionMessager.hpp
	$(CXX) -c $< -o $@
