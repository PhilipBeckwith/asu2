#include "Grammar.h"

Grammar::Grammar()
{
  LexicalAnalyzer lexer;
  bool isTerminal=false;
  bool isEmpty=false;
  Rule *rule;

  symbolCount = 0;

  Token token = lexer.GetToken();
  addSymbol(&symbols,&symbolTable,"#",true);

  while(token.token_type!=DOUBLEHASH)
  {
    if(token.token_type==HASH)
    {
      isTerminal=false;
      if(isEmpty)
      {
        rule->RHS.push_back(symbols["#"]);
      }
      rules.push_back(*rule);
      delete(rule);
    }
    else if(token.token_type==ARROW)
    {
      isTerminal=true;
      isEmpty=true;
    }
    else{
      addSymbol(&symbols, &symbolTable,token.lexeme,isTerminal);
      if(!isTerminal)
      {
        rule=new(Rule);
        rule->LHS=symbols[token.lexeme];
      }
      else{rule->RHS.push_back(symbols[token.lexeme]);}
      isEmpty=false;
    }
    token=lexer.GetToken();
  }
  if(rules.size()!=0)
  {
      startSymbol=symbolTable[rules[0].LHS].name;
  }

}


void Grammar::addSymbol(std::map<std::string,int>* mp,std::vector<SymbolType>* vec,std::string symbolName, bool isTerm)
{

  SymbolType ruleType;
  ruleType.isTerminal=isTerm;
  ruleType.isGenerating=false;
  ruleType.isReachable=false;
  ruleType.name=symbolName;

  if (mp->insert(std::make_pair(symbolName,vec->size())).second)
  {
    vec->push_back(ruleType);
  }
  else
  {
    if(!isTerm)
    {
      vec->at(mp->at(symbolName)).isTerminal=false;
    }
  }
}

void Grammar::printNonTerminals()
{
  for(int i=1; i<symbolTable.size(); i++)
  {
    if(!symbolTable[i].isTerminal)
    {
      std::cout<<symbolTable[i].name<<" ";
    }
  }
}

void Grammar::printTerminals()
{
  for(int i=1; i<symbolTable.size(); i++)
  {
    if(symbolTable[i].isTerminal)
    {
      std::cout<<symbolTable[i].name<<" ";
    }
  }
}

void Grammar::printRules()
{
  for(int i=0; i<rules.size(); i++)
  {
    printRule(&rules[i]);
  }
}

void Grammar::printFirstSets()
{
  if(firstSets.size()==1){std::cout<<"FIRST() = { }";}
  bool printed=false;
  for(int i=0; i<firstSets.size(); i++)
  {
    if(!symbolTable[firstSets[i].LHS].isTerminal)
    {
      printed=false;
      std::cout<<"FIRST("<<symbolTable[firstSets[i].LHS].name<<") = { ";
      if(symbolTable[firstSets[i].LHS].maybeEmpty)
      {
        std::cout<<"#";
        printed=true;
      }
      for(int k=0; k<firstSets[i].RHS.size(); k++)
      {
        if(firstSets[i].RHS[k]!=0)
        {
          if(printed){std::cout<<", ";}
          std::cout<<symbolTable[firstSets[i].RHS[k]].name;
          printed=true;
        }
      }
      if(printed)
      {
        std::cout<<" }\n";
      }
      else
      {
        std::cout<<"}\n";
      }
    }
  }
}

void Grammar::printRule(Rule *rule)
{
  std::cout<<symbolTable[rule->LHS].name<<" -> ";
  for(int k=0; k<rule->RHS.size(); k++)
  {
    std::cout<<symbolTable[rule->RHS[k]].name<<" ";
  }
  std::cout<<std::endl;
}

void Grammar::addTerminalRules()
{
  for(int i=0; i<symbolTable.size(); i++)
  {
    if(symbolTable[i].isTerminal)
    {
      Rule rule;
      rule.LHS=i;
      rule.RHS.push_back(i);
      rules.push_back(rule);
    }
  }
}

void Grammar::findGenoratingSymbols()
{
  std::vector<bool> bitMap;
  bool changed=true;

  for(int i=0; i<symbolTable.size(); i++)
  {
    symbolTable[i].isGenerating=symbolTable[i].isTerminal;
  }

  while(changed)
  {
    changed=checkGeneratingRules();
  }
}

bool Grammar::checkGeneratingRules()
{
  bool flag=false;

  for(int i=0; i<rules.size(); i++)
  {
    if(isGenerating(i)){flag=true;}
  }
  return flag;
}


bool Grammar::isGenerating(int index)
{
  bool flag=false;

  if(!symbolTable[rules[index].LHS].isGenerating)
  {
    flag=true;
    for(int i=0; i<rules[index].RHS.size(); i++)
    {
      if(!symbolTable[rules[index].RHS[i]].isGenerating)
      {
        symbolTable[rules[index].LHS].isGenerating=false;
        flag=false;
      }
    }
    symbolTable[rules[index].LHS].isGenerating=flag;
  }
  return flag;
}

void Grammar::findReachableSymbols()
{
  bool flag=true;
  if(symbols.find(startSymbol)!=symbols.end())
  {
    symbolTable[symbols[startSymbol]].isReachable=true;
  }
  while(flag)
  {
    flag=checkReachableSymbols();
  }
}


bool Grammar::checkReachableSymbols()
{
  bool flag=false;
  for(int i=0; i<rules.size(); i++)
  {
    if(symbolTable[rules[i].LHS].isReachable )
    {
      if(makeReachable(i)){flag=true;}
    }
  }
  return flag;
}

bool Grammar::makeReachable(int index)
{
  bool flag=false;
  for(int i=0; i<rules[index].RHS.size(); i++)
  {
    if(!symbolTable[rules[index].RHS[i]].isReachable)
    {
      symbolTable[rules[index].RHS[i]].isReachable=true;
      flag=true;
    }
  }
  return flag;
}

bool Grammar::ruleIsUsefull(int index)
{
  bool flag=true;

  if(!(symbolTable[rules[index].LHS].isGenerating&&symbolTable[rules[index].LHS].isReachable))
  {
    flag=false;
  }

  for(int i=0; i<rules[index].RHS.size(); i++)
  {
    if(!(symbolTable[rules[index].RHS[i]].isGenerating&&symbolTable[rules[index].RHS[i]].isReachable))
    {
      flag=false;
    }
  }

  return flag;
}

void Grammar::removeNonGenorating()
{
  for(int i=0; i<symbolTable.size(); i++)
  {
    symbolTable[i].isReachable=true;
  }
  removeUselessSymbols();
  for(int i=0; i<symbolTable.size(); i++)
  {
    symbolTable[i].isReachable=false;
  }
}

void Grammar::removeUselessSymbols()
{
  std::vector<SymbolType> newSymbolTable;
  std::vector<Rule>newRules;
  std::map<std::string, int> newSymbols;
  std::map<int,int> mapingMap;

  addSymbol(&newSymbols,&newSymbolTable,"#", true);

  for(int i=0; i<symbolTable.size(); i++)
  {
    if(symbolTable[i].isGenerating && symbolTable[i].isReachable)
    {
      SymbolType symbol=symbolTable[i];
      addSymbol(&newSymbols,&newSymbolTable,symbol.name, symbol.isTerminal);
      newSymbolTable[newSymbolTable.size()-1].isGenerating=true;
      newSymbolTable[newSymbolTable.size()-1].isReachable=true;
    }
  }

  for(int i=0;i<rules.size();i++)
  {
    if(ruleIsUsefull(i))
    {
        newRules.push_back(rules[i]);
    }
  }

  for(int i=0; i<newSymbolTable.size(); i++)
  {
    mapingMap.insert(std::make_pair(symbols[newSymbolTable[i].name],i));
  }

  for(int i=0; i<newRules.size(); i++)
  {
    Rule* rule;
    rule=&newRules[i];

    rule->LHS=mapingMap[rule->LHS];

    for(int k=0; k<rule->RHS.size(); k++)
    {
      rule->RHS[k]=mapingMap[rule->RHS[k]];
    }
  }

  symbols.clear();
  symbolTable.clear();
  rules.clear();

  for(int i=0; i<newSymbolTable.size();i++)
  {
    SymbolType symbol=newSymbolTable[i];
    addSymbol(&symbols,&symbolTable,symbol.name, symbol.isTerminal);
    symbolTable[symbolTable.size()-1].isGenerating=true;
    symbolTable[symbolTable.size()-1].isReachable=true;
    symbolTable[symbolTable.size()-1].maybeEmpty=false;
  }

  for(int i=0; i<newRules.size(); i++)
  {
    rules.push_back(newRules[i]);
  }
}

void Grammar::findUselessSymbols()
{
  int numOfSymbols=symbolTable.size();
  findGenoratingSymbols();
  removeNonGenorating();
  findReachableSymbols();
  removeUselessSymbols();

  if(numOfSymbols!=symbolTable.size())
  {
    hasUseless=true;
  }
  else{hasUseless=false;}
}

void Grammar::findFirstSets()
{
  initalizeFirstSets();
  while(findFirsts()){}
  sortSets(&firstSets);
}

void Grammar::initalizeFirstSets()
{
  for(int i=0; i<symbols.size(); i++)
  {
    Rule rule;
    rule.LHS=i;
    if(symbolTable[i].isTerminal)
    {
      rule.RHS.push_back(i);
    }
    firstSets.push_back(rule);
  }
  for(int i=0; i<symbolTable.size(); i++)
  {
    symbolTable[i].maybeEmpty=false;
  }
  symbolTable[0].maybeEmpty=true;

  for(int i=0; i<rules.size(); i++)
  {
    if(rules[i].RHS[0]==0)
    {
      rules[i].maybeEmpty=true;
    }
    else
    {
      rules[i].maybeEmpty=false;
    }
  }

}

void Grammar::sortSets(std::vector<Rule> *sets)
{
  for(int i=0; i<sets->size(); i++)
  {
    std::sort(sets->at(i).RHS.begin(),sets->at(i).RHS.end());
  }
}

bool Grammar::findFirsts()
{
  bool flag=false;

  for(int i=0; i<rules.size(); i++)
  {
    if(checkRuleForFirts(&rules[i])){flag=true;}
  }
  return flag;
}

bool Grammar::checkRuleForFirts(Rule *rule)
{
  bool flag=false;
  int index=0;
  bool isEmpty=true;

  while(index<rule->RHS.size() && isEmpty)
  {
    if(!symbolTable[rule->RHS[index]].maybeEmpty){isEmpty=false;}
    if(findUnion(&firstSets[rule->LHS].RHS, &firstSets[rule->RHS[index]].RHS))
    {
      flag=true;
    }

    findUnion(&rule->firsts, &firstSets[rule->RHS[index]].RHS);

    index++;
  }

  if(isEmpty)
  {
    if(!symbolTable[rule->LHS].maybeEmpty){flag=true;}
    symbolTable[rule->LHS].maybeEmpty=true;
    rule->maybeEmpty=true;
  }

  return flag;
}


bool Grammar::findUnion(std::vector<int> *vec,std::vector<int> *add)
{
  bool hasChanged=false;
  bool contains=false;

  for(int i=0; i<add->size(); i++){
    contains=false;
    for(int k=0; k<vec->size(); k++){
      if(add->at(i)==vec->at(k)){contains=true;}
    }
    if(!contains && add->at(i)!=0)
    {
      vec->push_back(add->at(i));
      hasChanged=true;
    }
  }
  return hasChanged;
}

void Grammar::findFallowSets()
{
  initalizeFallowSets();
  while(findFallows()){}
  sortSets(&followSets);
}

void Grammar::initalizeFallowSets()
{
  for(int i=0; i<symbolTable.size(); i++)
  {
      Rule fallow;
      fallow.LHS=i;
      fallow.RHS.push_back(0);
      followSets.push_back(fallow);
  }
  addSymbol(&symbols, &symbolTable, "$", true);
  followSets[symbols[startSymbol]].RHS.push_back(symbols["$"]);

  for(int i=0; i<rules.size(); i++)
  {
      fallowFirstpass(&rules[i]);
  }
}

void Grammar::fallowFirstpass(Rule* rule)
{
  for(int i=0; i<rule->RHS.size()-1;i++)
  {
    bool doNext=!symbolTable[rule->RHS[i]].isTerminal;
    int k=i+1;
    while(k<rule->RHS.size()&&doNext)
    {
      findUnion(&followSets[rule->RHS[i]].RHS,&firstSets[rule->RHS[k]].RHS);
      doNext=symbolTable[rule->RHS[k]].maybeEmpty;
      k++;
    }
  }
}


bool Grammar::findFallows()
{
  bool flag=false;
  for(int i=0; i<rules.size(); i++)
  {
    if(checkRuleForFallows(&rules[i])){flag=true;}
  }
  return flag;
}

bool Grammar::checkRuleForFallows(Rule *rule)
{
  bool flag=false;
  int index=rule->RHS.size()-1;
  bool isEmpty=true;

  while(index>=0 && isEmpty)
  {
    if(!symbolTable[rule->RHS[index]].maybeEmpty){isEmpty=false;}
    if(findUnion(&followSets[rule->RHS[index]].RHS, &followSets[rule->LHS].RHS))
    {
      flag=true;
    }
    index--;
  }
  return flag;
}


void Grammar::printFallowSets()
{
  bool printed=false;
  for(int i=0; i<followSets.size(); i++)
  {
    if(!symbolTable[followSets[i].LHS].isTerminal)
    {
      printed=false;
      int symbolsToPrint=followSets[i].RHS.size();
      std::cout<<"FOLLOW("<<symbolTable[followSets[i].LHS].name<<") = { ";
      if((followSets[i].RHS[followSets[i].RHS.size()-1])==symbols["$"])
      {
        std::cout<<"$";
        printed=true;
        symbolsToPrint--;
      }
      for(int k=0; k<symbolsToPrint; k++)
      {
        if(followSets[i].RHS[k]!=0 && followSets[i].RHS[k]!=symbols["$"])
        {
          if(printed){std::cout<<", ";}
          std::cout<<symbolTable[followSets[i].RHS[k]].name;
          printed=true;
        }
      }
      std::cout<<" }\n";
    }
  }
}


bool Grammar::predictiveParseRule1()
{
  bool flag=true;

  for(int i=0; i<rules.size()-1; i++){
    for(int k=i+1; k<rules.size(); k++){
      if(rules[i].LHS==rules[k].LHS)
      {
        if(hasIntersection(rules[i].firsts,rules[k].firsts))
        {
          flag=false;
        }
        if(rules[i].maybeEmpty&&rules[k].maybeEmpty){flag=false;}
      }
    }
  }


  return flag;
}


bool Grammar::predictiveParseRule2()
{
  bool flag=true;

  for(int i=0; i<symbolTable.size(); i++)
  {
    if(symbolTable[i].maybeEmpty && !symbolTable[i].isTerminal)
    {
      if(hasIntersection(firstSets[i].RHS, followSets[i].RHS))
      {
        flag=false;
      }
    }
  }

  return flag;
}

bool Grammar::hasIntersection(std::vector<int> vec1, std::vector<int> vec2)
  {
    bool hasIntersect=false;
    for(int i=0; i<vec1.size();i++){
      for(int k=0; k<vec2.size(); k++)
      {
        if((vec1[i]==vec2[k]) && (vec1[i] !=0))
        {
          hasIntersect=true;
        }
      }
    }
    return hasIntersect;
  }

void Grammar::hasPredictiveParse()
{
  bool rule1,rule2;
  if(hasUseless)
  {
    rule1=rule2=false;
  }
  else
  {
    rule1=predictiveParseRule1();
    rule2=predictiveParseRule2();
  }

  if(rule1 && rule2)
  {
    std::cout<<"YES";
  }
  else
  {
    std::cout<<"NO";
  }

  //std::cout<<"\n"<<rule1<<"\n"<<rule2<<"\n";
  //

}
