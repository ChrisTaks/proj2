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
  std::string finalInput;
  for (int i = 2; i < argc; ++i) {
    if (argv[i] != "") {
      std::string input(argv[i]);
      //input += kUS;
      finalInput += input;
      finalInput += kUS;
    }
  }
  finalInput += kEoT;
    std::cout << "[WIDPIO]: input: " << finalInput << std::endl;
    ::size_t bytes_wrote = Write(finalInput);

    if (bytes_wrote < 0) {
      std::cerr << "Client terminating..." << std::endl;
      exit(3);
    } else if (bytes_wrote == 0) {
      std::cerr << "Server disconnected" << std::endl;
      exit(4);
    }
 // }  // end for loop

  // recieve the data
  std::vector<std::string> theLinesOld;
  ssize_t final_bytes;
  std::string msg;
  
 while (true) {
    std::cout << "attempting to read..." << std::endl;
    //std::cout << "socket_fd_: " << socket_fd_ << std::endl;
    ::ssize_t bytes_read = Read(&theLinesOld, &msg);
    std::cout << "[WIDPIO]: theLines size: " << theLinesOld.size() << std::endl;
    std::cout << "[WIDPIO]: byte_read: " << bytes_read << std::endl;
    std::cout << "[WIDPIO]: msg" << msg << std::endl;

   if (bytes_read < 0) {
      std::cerr << "Server shutting down..." << std::endl;
      //exit(0);
   } else if (bytes_read) {
      std::cout << "[WIDPIO]: no more to read" << std::endl;
      Close(socket_fd_);
      break;
   }
  }
  // print/process the data
  std::cout << "[WIDPIO]: printing and processing data..." << std::endl;
  
  std::vector<std::string> theLines;
  std::string toBeAdded;
  for (size_t i = 0; i < msg.size(); ++i) {
    if (msg[i] == kUS || msg[i] == kEoT && toBeAdded.size() > 0) {
      theLines.push_back(toBeAdded);
      toBeAdded = "";
    } else {
      toBeAdded += msg[i];
    }
  }
  std::cout << "[WIDPIO]: toBeAdded final: " << toBeAdded << std::endl;
  for (std::string line : theLines) {
    std::cout << "[WIDPIO]: recieved line: " << line << std::endl;
  }

  int lineNumber = 0;
  for (std::string line : theLines) {
    ++lineNumber;
    std::string finishedLine = "line "; 
    finishedLine += std::to_string(lineNumber);
    finishedLine += ": ";
    finishedLine += line;
    finishedLine += " = ";
    finishedLine += processEquation(line);
    std::cout << finishedLine << std::endl;
  }
}

double DomainSocketClient::AddNumbers(double a, double b) {
  return a + b;
}

double DomainSocketClient::SubtractNumbers(double a, double b) {
  return a - b;
}

double DomainSocketClient::MultiplyNumbers(double a, double b) {
  return a * b;
}

double DomainSocketClient::DivideNumbers(double a, double b) {
  return a / b;
}

bool DomainSocketClient::IsOperator(std::string arg) {
  return arg == "+" || arg == "-" || arg == "x" || arg == "/";
}

std::string DomainSocketClient::processEquation(std::string line) {
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

int main(int argc, char *argv[]) {
  DomainSocketClient dsc(argv[1]);
  dsc.Run(argc, argv);

  return 0;
}