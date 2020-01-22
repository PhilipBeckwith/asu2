#include "lexer.h"
#include "Logger.h"
#include <iostream>
#include <cstdlib>
#include <vector>
#include <map>
#include <iostream>

typedef struct{
  std::string name;
  TokenType type;
  int lineDeclared;
  bool initalized, used, native;
}Var;


class Scope
{
private:
  std::vector< std::map<std::string, Var> > scopes;
  Logger* log;

  bool contains(int index, std::string name);
  Var getVar(std::string name, int index);
protected:
public:
  Scope();
  void setLogger(Logger* logger);
  void newScope();
  Var exitScope();
  bool addVariables(Var var);
  void initalizeVar(std::string name);
  void useVar(std::string name);
  bool varIsInitalized(std::string name);
  Var getVar(std::string name);

};
