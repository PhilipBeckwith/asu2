#include "Grammar.h"

int Grammar::addSymbol(std::string name){

  if(symbolIndexTable.insert(std::make_pair(name,allSymbols.size())).second){
    Symbol symbol;
    symbol.name=name;
    symbol.isTerminal=true;
    symbol.isUsefull=false;
    allSymbols.push_back(symbol);
  }

  return symbolIndexTable[name];
}

void Grammar::addRule(Rule rule){

  rules.push_back(rule);

}

void Grammar::finishInitalization(){
  for(int i=0; i<rules.size(); i++){
    allSymbols[rules[i].LHS].isTerminal=false;
  }
}

void Grammar::printTerminals()
{
  for(int i=0; i<allSymbols.size(); i++)
  {
    if(allSymbols[i].isTerminal)
    {
      std::cout<<allSymbols[i].name<<" ";
    }
  }
}
void Grammar::printNonTerminals()
{

}


//tasks
void Grammar::task1(){
  printNonTerminals();
  printTerminals();
}

void Grammar::task2(){
  //setUsefullSymbols();
  //printUsefullRules();
}

void Grammar::task3(){
  //setUsefullSymbols();
  //findFirstSets();

  //print first sets
}

void Grammar::task4(){
  //setUsefullSymbols();
  //findFirstSets();
  //findFallowSets();

  //print fallow sets
}

void Grammar::task5(){
  //setUsefullSymbols();
  //findFirstSets();
  //findFallowSets();

  //decide if it has a parser...

  //print
}
