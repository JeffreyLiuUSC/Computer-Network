EE450 Summer 2019

1.Name: Jiahao Liu
2.USCID: 6044109297
3.Departments open input .txt files, parse and read data from it. Departments use data to server(Admission), Admission stores the data in .txt file
4.main.h:#define the static variable and includes files
  socket.h/cpp: socket related code, involving get_in_addr, getsockname, getaddrinfo, create_socket, connect(client side), setsockopt, bind.
  database.h/cpp: stores the data from Departments
  Admission.h/cpp & AdmissionMessager.h/cpp: create empty database, conduct the tcp communication with client and print the information during communication.
  Department.h/cpp & DepartmentMessager.h/cpp & DepartmentParser.h/cpp: read data from local .txt files, do the tcp socket as client, send data to Admission, echo the messages from communications
5.To run code(in terminals):
     make all
     ./Admission
     ./Department 
6. reference from http://www.beej.us/guide/bgnet/
