#include "Parser.h"
#include "Logger.h"

int main()
{
  Logger logger;
  Parser parse(&logger);
  parse.parse();
  logger.endLog();
  logger.blankLine();
  logger.blankLine();
  parse.printErrors();
}

/*
int main()
{
  Parser parse();
  parse.parse();
  parse.printErrors();
}
 */
