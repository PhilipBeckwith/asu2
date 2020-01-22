#include "Logger.h"

Logger::Logger(){
  logLevel=0;
}

void Logger::info(std::string message){
  printLine(message);
  endLine();
}

void Logger::enterFunction(std::string functionName, std::string arguments){
  printLine(functionName);
  printSyntax("(");
  printSyntax(arguments);
  printSyntax("){");
  endLine();
  incLogLevel();
}

void Logger::enterFunction(std::string functionName){
  enterFunction(functionName, "");
}

void Logger::exitFunction(std::string exitMessage){
  decLogLevel();
  printSpace();
  printSyntax("}");
  print(exitMessage);
  endLine();
}

void Logger::exitFunction(){
  exitFunction("");
}

void Logger::print(std::string debugMSG){
  #if DEBUG_ON
  std::cout<<debugMSG;
  #endif
}

void Logger::print(int value){
  #if DEBUG_ON
  std::cout<<value;
  #endif
}

void Logger::print(float value){
  #if DEBUG_ON
  std::cout<<value;
  #endif
}

void Logger::print(double value){
  #if DEBUG_ON
  std::cout<<value;
  #endif
}

void Logger::print(char value){
  #if DEBUG_ON
  std::cout<<value;
  #endif
}

void Logger::printSyntax(std::string syntax){
  #if SYNTAX_ON
  print(syntax);
  #endif
}

void Logger::printSpace(){
  for(int i=0; i<logLevel*TAB_SIZE; i++){
    if(i%TAB_SIZE==0){
      printBrace();
    }
    print(" ");
  }
}

void Logger::endLine(){
  print("\n");
}

void Logger::printLine(std::string message){
  printSpace();
  print(message);
}

void Logger::printBrace(){
#if BrACE_ON
  print("|");
#endif
}

void Logger::incLogLevel(){
  logLevel++;
}

void Logger::decLogLevel(){
  logLevel --;
  if(logLevel < 0){
    print("Error: Function Level < 0 ");
    endLine();
    print("Are you missing an enterFunction()?");
    endLine();
  }
}


//Argument Functions
void Logger::argument(std::string argumentName, std::string value){
  printLine(argumentName);
  print(" : ");
  print(value);
  endLine();
}

void Logger::argument(std::string argumentName, char value){
  printLine(argumentName);
  print(" : ");
  print(value);
  endLine();
}

void Logger::argument(std::string argumentName, int value){
  printLine(argumentName);
  print(" : ");
  print(value);
  endLine();
}

void Logger::argument(std::string argumentName, float value){
  printLine(argumentName);
  print(" : ");
  print(value);
  endLine();
}

void Logger::argument(std::string argumentName, double value){
  printLine(argumentName);
  print(" : ");
  print(value);
  endLine();
}

//Variable Methods
void Logger::variable(std::string argumentName, std::string value){
  printLine(argumentName);
  print(" = ");
  print(value);
  endLine();
}

void Logger::variable(std::string argumentName, char value){
  printLine(argumentName);
  print(" = ");
  print(value);
  endLine();
}

void Logger::variable(std::string argumentName, int value){
  printLine(argumentName);
  print(" = ");
  print(value);
  endLine();
}

void Logger::variable(std::string argumentName, float value){
  printLine(argumentName);
  print(" = ");
  print(value);
  endLine();
}

void Logger::variable(std::string argumentName, double value){
  printLine(argumentName);
  print(" = ");
  print(value);
  endLine();
}

void Logger::blankLine(){
  printSpace();
  endLine();
}
