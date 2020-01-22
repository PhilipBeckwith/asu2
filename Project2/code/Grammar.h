#pragma once

#include "lexer.h"
#include <map>
#include <vector>
#include <algorithm> //for sorting

typedef struct
{
  int LHS;
  std::vector<int> RHS;
  std::vector<int> firsts;
  bool maybeEmpty;
}Rule;

typedef struct
{
  std::string name;
  bool isTerminal;
  bool isGenerating;
  bool isReachable;
  bool maybeEmpty;
} SymbolType;

class Grammar
{
private:
  int symbolCount;
  bool hasUseless;
  std::string startSymbol;
  std::map<std::string, int> symbols;
  std::vector<SymbolType> symbolTable;
  std::vector<Rule> rules;

  std::vector<Rule> firstSets;
  std::vector<Rule> followSets;

  void addSymbol(std::map<std::string,int>* mp,std::vector<SymbolType>* vec,std::string symbolName, bool isTerm);
  void addTerminalRules();

  //Useless Symbols
  void findGenoratingSymbols();
  bool checkGeneratingRules();
  bool isGenerating(int index);

  void findReachableSymbols();
  bool checkReachableSymbols();
  bool makeReachable(int i);

  bool ruleIsUsefull(int index);

  void removeUselessSymbols();
  void removeNonGenorating();

  //First sets
  void initalizeFirstSets();
  bool findFirsts();
  bool checkRuleForFirts(Rule *rule);

  //Fallow sets
  void initalizeFallowSets();
  void fallowFirstpass(Rule *rule);
  bool findFallows();
  bool checkRuleForFallows(Rule *rule);

  //predictive parse
  bool predictiveParseRule1();
  bool predictiveParseRule2();


  // helper fuctions from everything
  void sortSets(std::vector<Rule>* sets);
  bool findUnion(std::vector<int> *vec,std::vector<int> *add);
  bool hasIntersection(std::vector<int> vec1, std::vector<int> vec2);

protected:
public:
  Grammar();
  void printNonTerminals();
  void printTerminals();

  void printRules();
  void printFirstSets();
  void printFallowSets();
  void printRule(Rule *rule);

  void findUselessSymbols();
  void findFirstSets();
  void findFallowSets();
  void hasPredictiveParse();

};
