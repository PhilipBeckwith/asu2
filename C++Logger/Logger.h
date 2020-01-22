#include <string>
#include <iostream>

#define DEBUG_ON 1
#define SYNTAX_ON 1
#define BrACE_ON 0
#define TAB_SIZE 2

class Logger{
private:
  int logLevel;

protected:
  void printLine(std::string message);
  void print(std::string message);
  void print(int message);
  void print(float value);
  void print(double value);
  void print(char value);
  void printSyntax(std::string syntax);
  void printBrace();
  void printSpace();
  void endLine();

  void incLogLevel();
  void decLogLevel();

public:
  Logger();
  //Function methods
  void enterFunction(std::string functionName, std::string arguments);
  void enterFunction(std::string functionName);
  void exitFunction(std::string exitMessage);
  void exitFunction();

  //Argument Functions
  void argument(std::string argumentName, std::string value);
  void argument(std::string argumentName, char value);
  void argument(std::string argumentName, int value);
  void argument(std::string argumentName, float value);
  void argument(std::string argumentName, double value);

  //Variable Methods
  void variable(std::string argumentName, std::string value);
  void variable(std::string argumentName, char value);
  void variable(std::string argumentName, int value);
  void variable(std::string argumentName, float value);
  void variable(std::string argumentName, double value);

  //Message Functions
  void info(std::string message);
  void blankLine();
};
