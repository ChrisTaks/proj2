// Copyright 2024 Chris Taks

#include <proj2/client.h>

#include <iostream>

void DomainSocketClient::Run(int argc, char *argv[]) {
  std::cout << "Client initializing..." << std::endl;
  if (!Init()) {
    exit(1);
  }

  std::cout << "Client connecting..." << std::endl;
  if (!Connect()) {
    exit(2);
  }
  // send the data
  for (int i = 2; i < argc; ++i) {
    std::string input(argv[i]);
    std::cout << input << std::endl;
    ::size_t bytes_wrote = Write(input);

    if (bytes_wrote < 0) {
      std::cerr << "Client terminating..." << std::endl;
      exit(3);
    } else if (bytes_wrote == 0) {
      std::cerr << "Server disconnected" << std::endl;
      exit(4);
    }
  }

  // recieve the data
  std::vector<std::string> theLines;
  ssize_t final_bytes;
  std::string msg;
  
  while (true) {
    std::cout << "attempting to read..." << std::endl;
    std::cout << "socket_fd_: " << socket_fd_ << std::endl;
    ::ssize_t bytes_read = Read(&theLines, &msg);
    std::cout << "read succesfull" << std::endl;

   if (bytes_read < 0) {
      std::cerr << "Server shutting down..." << std::endl;
      //exit(0);
   } else if (!bytes_read) {
      std::cout << "Client disconnected" << std::endl;
      Close(socket_fd_);
      break;
   }
  }
  // print the data
  std::cout << "printing data..." << std::endl;
  for (std::string line : theLines) {
    std::cout << line << std::endl;
  }
}

int main(int argc, char *argv[]) {
  DomainSocketClient dsc(argv[1]);
  dsc.Run(argc, argv);

  return 0;
}