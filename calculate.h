// Copyright 2024 Chris Taks

#ifndef _PROJ2_CALCULATE_H_
#define _PROJ2_CALCULATE_H_

#include<string>
#include<vector>

double AddNumbers(double a, double b);

double SubtractNumbers(double a, double b);

double MultiplyNumbers(double a, double b);

double DivideNumbers(double a, double b);

bool IsOperator(std::string arg);

std::string processEquation(std::string line);

#endif  // _PROJ2_CALCULATE_H_