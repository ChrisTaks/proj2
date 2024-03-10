// Copyright 2024 Chris Taks

#include <proj2/server.h>

#include <iostream>
#include <sys/sysinfo.h>

void DomainSocketServer::Run() {
  int socket_fd;

  // std::cout << "DomainSocketServer initializing..." << std::endl;
  if (!Init()) {
    exit(-1);
  }

  // std::cout << "DomainSocketServer binding socket to address..." << std::endl;
  if (!Bind()) {
    exit(-2);
  }

  // std::cout << "DomainSocketServer listening for client connections..." 
  //          << std::endl;
  size_t sizenum = 2;
  if (!Listen(sizenum)) {
    exit(-3);
  }
  
  std::cout << "SERVER STARTED" << std::endl;
  std::cout << "MAX CLIENTS: " << get_nprocs_conf() << std::endl;

  while (true) {
    std::vector<std::string> theArgs;
    ssize_t final_bytes;
    //socket_fd = ::accept(socket_fd_, nullptr, nullptr);
    if(Accept(&socket_fd)) {
      std::cout << "ACCEPTED" << std::endl;
    }
    std::cout << "SOCKET_FD read: " << socket_fd << std::endl;
    if (socket_fd < 0) {
      std::cerr << "Socket connection: " << ::strerror(errno) << std::endl;
      continue;
    }
    std:: cout << "CLIENT CONNECTED" << std::endl;

    while (true) {
      std::string msg;
     ::ssize_t bytes_read = Read(&theArgs, &msg, socket_fd);
     std::cout << "theArgs size: " << theArgs.size() << std::endl;
     std::cout << "bytes_read :" << bytes_read << std::endl;

     if (bytes_read < 0) {
       std::cerr << "Server shutting down..." << std::endl;
       exit(0);
     } else if (bytes_read) {
        std::cout << "reading done" << std::endl;
        //close(socket_fd);
        break;
      }
    final_bytes = bytes_read;
    }

    std::vector<std::string> finalLine;

    std::cout << "PATH: " << theArgs[0] << std::endl;
    std::ifstream equationFile(theArgs[0]);
    if (!equationFile.is_open()) {
      std::cerr << "INVALID FILE" << std::endl;
    }
    // sort line numbers
    std::vector<int> lineNumbers;
    for (size_t i = 1; i < theArgs.size(); ++i) {
      lineNumbers.push_back(std::stoi(theArgs[i]));
    }
    std::sort(lineNumbers.begin(), lineNumbers.end());
    std::cout << "LINES: "; 
    for (size_t i = 0; i < lineNumbers.size(); ++i) {
      std::cout << lineNumbers[i];
      if (i < lineNumbers.size()-1) {
        std::cout << ", ";
      }
    }
    std::cout << std::endl;

    int lineNumber = 0;
    std::string equationLine;
    while (std::getline(equationFile, equationLine)) {
      ++lineNumber;
      for (int line : lineNumbers) {
        if (line == lineNumber) {
          finalLine.push_back(equationLine);
        }
      }
    }
    std::cout << "BYTES SENT: " << final_bytes << std::endl;
    equationFile.close();
    for (std::string line : finalLine) {
      std::cout << line << std::endl;
    }
    // send the data
    // if(Accept(&socket_fd)) {
    //   std::cout << "ACCEPTED" << std::endl;
    // }
    for (std::string line : finalLine) {
      std::cout << "SOCKET_FD write: " << socket_fd << std::endl;
      std::cout << "socket_fd_: " << socket_fd_ << std::endl;
      //std::string testwrite("test write");
      ::size_t bytes_wrote = Write(line, socket_fd);

      if (bytes_wrote < 0) {
        std::cerr << "Server terminating..." << std::endl;
        exit(3); 
      } else if (bytes_wrote == 0) {
        std::cerr << "Client disconnected (during write)" << std::endl;
        exit(4);
      }
    }
    Close(socket_fd);
    //clears
    finalLine.clear();
    theArgs.clear();
    
  } // end while loop
}

double DomainSocketServer::AddNumbers(double a, double b) {
  return a + b;
}

double DomainSocketServer::SubtractNumbers(double a, double b) {
  return a - b;
}

double DomainSocketServer::MultiplyNumbers(double a, double b) {
  return a * b;
}

double DomainSocketServer::DivideNumbers(double a, double b) {
  return a / b;
}

bool DomainSocketServer::IsOperator(std::string arg) {
  return arg == "+" || arg == "-" || arg == "x" || arg == "/";
}

std::string DomainSocketServer::processEquation(std::string line) {
  std::vector<double> numbers;
  std::vector<std::string> operators;
  // break up the string
  std::vector<std::string> args;
  std::string component;
  for (char c : line) {
    if (c != ' ') {
      component += c;
    } else {
      args.push_back(component);
      component = "";
    }
  }
  args.push_back(component);
  // for (std::string test : args) {
  //   std::cout << "test: " << test << std::endl;
  // }

  // adding the args to their respective vectors
  for (size_t i = 0; i < args.size(); ++i) {
    if (IsOperator(args[i])) {
      operators.push_back(args[i]);
      if (operators.back() == "x" || operators.back() == "/") {
        double a = numbers.back();
        numbers.pop_back();
        if (operators.back() == "x") {
          numbers.push_back(MultiplyNumbers(a, std::stod(args[i+1])));
        }
        if (operators.back() == "/") {
          numbers.push_back(DivideNumbers(a, std::stod(args[i+1])));
        }
        ++i;  // iterate past the next variable since it was already used
      }
    } else {
      numbers.push_back(std::stod(args[i]));
    }
  }
  // next, just add / subtract the remaining numbers
  for (std::string& op : operators) {
    if (op == "+") {
      numbers[0] = AddNumbers(numbers[0], numbers[1]);
      numbers.erase(numbers.begin()+1);
    }
    if (op == "-") {
      numbers[0] = SubtractNumbers(numbers[0], numbers[1]);
      numbers.erase(numbers.begin()+1);
    }
  }
  //std::cout << numbers[0] << std::endl;
  return std::to_string(static_cast<int>(numbers[0]));
}


// socket()
// bind()
// listen()
// accept()
// read()/write()
// close()

int main(int arc, char *argv[]) {
  DomainSocketServer dss(argv[1]);
  
  dss.Run();

  return 0;
}