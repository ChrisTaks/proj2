// Copyright 2024 Chris Taks

#include <proj2/calculate.h>

double AddNumbers(double a, double b) {
  return a + b;
}

double SubtractNumbers(double a, double b) {
  return a - b;
}

double MultiplyNumbers(double a, double b) {
  return a * b;
}

double DivideNumbers(double a, double b) {
  return a / b;
}

bool IsOperator(std::string arg) {
  return arg == "+" || arg == "-" || arg == "x" || arg == "/";
}

std::string ProcessEquation(std::string line) {
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
  // adding the args to their respective vectors
  for (int i = 0; i < args.size(); ++i) {
    if (IsOperator(args[i])) {
      operators.push_back(args[i]);
      if (operators.back() == "x" || operators.back() == "/") {
        double a = numbers.back();
        numbers.pop_back();
        if (operators.back() == "x") {
          numbers.push_back(MultiplyNumbers(a, std::stod(args[i+1])));
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
  return std::to_string(numbers[0]);
  }
}