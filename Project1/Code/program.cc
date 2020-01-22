#include "program.h"

int Program::addConstant(int constant)
{
  memory.push_back(constant);
  return memory.size()-1;
}

int Program::addInputs(int constant)
{
  inputs.push_back(constant);
  return inputs.size()-1;
}

int Program::addVariable(std::string varName)
{
  if(var.insert(std::make_pair(varName,memory.size())).second)
  {
    memory.push_back(0);
  }
  return var[varName];
}

void Program::addStatementList(std::string procName,Statement *statement)
{
  if(procCalls.insert(make_pair(procName, statement)).second)
  {
    progStatementLists.push_back(statement);
  }
  else
  {
    *procCalls[procName]=*statement;
    delete(statement);
  }
}

void Program::setMain(Statement *MainPointer)
{
  addStatementList("Main", MainPointer);
  Main=MainPointer;
}

Statement* Program::getProcLocation(std::string name)
{
  Statement *temp=new(Statement);
  temp->type=END_OF_FILE;
  if(procCalls.insert(make_pair(name,temp)).second)
  {
    progStatementLists.push_back(temp);
  }
  else
  {
    delete(temp);
  }
  return procCalls[name];
}

void Program::run()
{
  inputCounter=0;
  run(Main);
}

void Program::run(Statement *statement)
{
  int doNumb;
  switch(statement->type)
  {
    case INPUT:
    memory[statement->op1]=inputs[inputCounter];
    inputCounter++;
    break;

    case OUTPUT:
    std::cout<<memory[statement->op1]<<" ";
    break;

    case PROC:
    run(statement->procCall);
    break;

    case DO:
    doNumb=memory[statement->op1];
    for(int i=0; i<doNumb; i++)
    {
      run(statement->procCall);
    }
    break;

    case EQUAL:
    memory[statement->LHS]=math(statement);
    break;
  }
  if(statement->next!=NULL)
  {
    run(statement->next);
  }
}

int Program::math(Statement* statement)
{
  int result;

  switch(statement->oppType)
  {
    case END_OF_FILE:
    result = memory[statement->op1];
    break;

    case PLUS:
    result = memory[statement->op1]+memory[statement->op2];
    break;

    case MINUS:
    result = memory[statement->op1]-memory[statement->op2];
    break;

    case MULT:
    result = memory[statement->op1]*memory[statement->op2];
    break;

    case DIV:
    result = memory[statement->op1]/memory[statement->op2];
    break;
  }
  return result;
}
