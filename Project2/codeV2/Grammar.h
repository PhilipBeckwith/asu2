#pragma once

#include <vector>
#include <map>
#include <string>
#include <iostream>


typedef struct{
  int LHS;
  std::vector<int>RHS;
}Rule;

typedef struct{
std::string name;
bool isTerminal;
bool isUsefull;
}Symbol;

class Grammar
{
private:
  std::map<std::string, int> symbolIndexTable; //Remembers what index a string is at
  std::vector<Symbol> allSymbols; //
  std::vector<Rule> rules;
  std::vector<Rule> firstSets;
  std::vector<Rule> fallowSets;

  //task 1 only uses print fuctions

  //Task 2
  void setUsefullSymbols(); //sets usefull flag in allSymbols
  std::vector<bool> findGeneratingSymbols(); //sub function of setusefulSymbols
  std::vector<bool> findReachableSymbols(); //sub fuction of setUsefullSymbols
  void removeUselessRules(); //Removes rules from rules that have useless symbols


  //task3
  void findFirstSets();

  //task4
  void findFallowSets();

  //print statements
  //task 1
  void printTerminals();
  void printNonTerminals();

  //task 2
  void printUsefullRules();


protected:
public:
  //initalization fuctions
  int addSymbol(std::string name);
  void addRule(Rule rule);
  void finishInitalization();

  //tasks
  void task1();
  void task2();
  void task3();
  void task4();
  void task5();
};
