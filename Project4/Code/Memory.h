#pragma once

#include <vector>
#include <map>
#include <string>
#include "compiler.h"

class MemoryManagment
{
private:
  std::vector<ValueNode*> stack;
  std::map<std::string, int> table;

protected:
public:
  MemoryManagment();

  struct ValueNode *addConstant(int value);
  struct ValueNode *addVariable(std::string id);
  struct ValueNode *lookUpVar(std::string id);
};
