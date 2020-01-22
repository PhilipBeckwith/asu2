#pragma once

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include "lexer.h"

struct Statement
{
  TokenType type;
  int LHS;
  int op1;
  TokenType oppType;
  int op2;
  struct Statement *next;
  struct Statement *procCall;
};

typedef Statement Statement;

class Program
{
private:
  // memory managment
  std::vector<int> memory;
  std::vector<int> inputs;
  std::map<std::string,int> var;
  int inputCounter;

  // program representation
  std::vector<Statement*> progStatementLists;
  Statement *Main;

  //procedure calls
  std::map<std::string, Statement*> procCalls;

  // running the program
  void run(Statement *statement);

public:
  //memory Managment
  int addConstant(int constant);
  int addInputs(int constant);
  int addVariable(std::string varName);

  // program representation managment
  void addStatementList(std::string procName,Statement *statement);
  void setMain(Statement* MainPointer);

  //proc call management
  Statement* getProcLocation(std::string name);

  //Run program;
  void run();
  int math(Statement* statement);

};
