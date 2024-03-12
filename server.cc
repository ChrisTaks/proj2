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
    std::vector<std::string> badArgs;
    ssize_t final_bytes;
    //socket_fd = ::accept(socket_fd_, nullptr, nullptr);
    if(!Accept(&socket_fd)) {
      std::cout << "CLIENT NOT ACCEPTED" << std::endl;
    }
    //std::cout << "SOCKET_FD read: " << socket_fd << std::endl;
    if (socket_fd < 0) {
      std::cerr << "Socket connection: " << ::strerror(errno) << std::endl;
      continue;
    }
    std:: cout << "CLIENT CONNECTED" << std::endl;

    std::string msg;
    ssize_t oldBytes;
    while (true) {
      //std::string msg;
     ::ssize_t bytes_read = Read(&badArgs, &msg, socket_fd);
     std::cout << "[WIDPIO]: theArgs size: " << badArgs.size() << std::endl;
     std::cout << "[WIDPIO]: bytes_read :" << bytes_read << std::endl;
     std::cout << "[WIDPIO]: msg: " << msg << std::endl;
      if (msg[msg.size()-1] == kEoT) {
        std::cout << "[WIDPIO]: end of transmission found " << msg[msg.size()-1] << std::endl;
      } else {
        std::cout << "[WIDPIO]: eot never found! " << msg[msg.size()-1] << std::endl;
      }

     if (bytes_read < 0) {
       std::cerr << "Server shutting down..." << std::endl;
       exit(0);
     } else if (bytes_read) {
        std::cout << "[WIDPIO]: reading done" << std::endl;
        //close(socket_fd);
        break;
      }
      oldBytes = bytes_read;
    final_bytes = bytes_read;
    }
    //testing msg
    std::vector<std::string> theArgs;
    std::string toBeAdded;
    for (size_t i = 0; i < msg.size(); ++i) {
      //std::cout << "[WIDPIO]: msg[i]: " << msg[i] << std::endl;
      if (msg[i] == kUS || msg[i] == kEoT && toBeAdded.size() > 0) {
        //std::cout << "[WIDPIO]: toBeAdded (kEoT found): " << toBeAdded << std::endl;
        theArgs.push_back(toBeAdded);
        toBeAdded = "";
      } else {
        toBeAdded += msg[i];
        //std::cout << "[WIDPIO]: toBeAdded + msg[i]" << toBeAdded << std::endl;
      }
    }
    //theArgs.push_back(toBeAdded);  // final one
    for (std::string line : theArgs) {
      std::cout << "[WIDPIO]: theArg: " << line <<std::endl;
    }


    // for (std::string line : theArgs) {
    //   std::cout << "[WIDPIO]: line recieved: " << line << std::endl;
    // }

    std::vector<std::string> finalLine;

    std::cout << "PATH: " << theArgs[0] << std::endl;
    std::ifstream equationFile(theArgs[0]);
    if (!equationFile.is_open()) {
      std::cerr << "INVALID FILE" << std::endl;
    }
    // sort line numbers
    std::vector<int> lineNumbers;
    for (size_t i = 1; i < theArgs.size(); ++i) {
      std::cout << "pushing back: [" << theArgs[i] << "]" <<std::endl;
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

    std::string finalInput;
    for (std::string line : finalLine) {
      std::cout << line << std::endl;
      finalInput += line;
      finalInput += kUS;
    }
    finalInput += kEoT;
    // send the data
    ::size_t bytes_wrote = Write(finalInput, socket_fd);
    //for (std::string line : finalLine) {
      //std::cout << "[WIDPIO]: writing " << line << std::endl;
      //::size_t bytes_wrote = Write(line, socket_fd);

      if (bytes_wrote < 0) {
        std::cerr << "Server terminating..." << std::endl;
        exit(3); 
      } else if (bytes_wrote == 0) {
        std::cerr << "Client disconnected (during write)" << std::endl;
        exit(4);
      }
    //}
    Close(socket_fd);
    //clears
    finalLine.clear();
    theArgs.clear();
    
  } // end while loop
}

int main(int arc, char *argv[]) {
  DomainSocketServer dss(argv[1]);
  
  dss.Run();

  return 0;
}