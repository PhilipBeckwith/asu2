#include "Memory.h"
#include "iostream"

MemoryManagment::MemoryManagment(){}

struct ValueNode *MemoryManagment::addConstant(int value)
{
  struct ValueNode* constant=new(struct ValueNode);
  constant->name=std::to_string(value);
  constant->value=value;

  stack.push_back(constant);
  return stack.back();
}

struct ValueNode *MemoryManagment::addVariable(std::string id)
{
  struct ValueNode* variable=new(struct ValueNode);
  variable->name=id;
  variable->value=0;

  if(table.insert(std::make_pair(id,stack.size())).second)
  {
    stack.push_back(variable);
  }
  return stack[table[id]];
}

struct ValueNode *MemoryManagment::lookUpVar(std::string id)
{
  int index=table[id];
  return stack[index];
}
