#include "parser.h"


int main()
{

  Parser parse;
  Program *program;
  program = parse.Parse();
  program->run();

}
