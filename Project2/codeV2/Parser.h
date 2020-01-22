#pragma once

#include "lexer.h"
#include "Grammar.h"
#include <vector>

class Parser{
private:
Grammar grammar;
LexicalAnalyzer lexer;

void parseRuleList();
std::vector<int> parseIDList();
void parseRule();
std::vector<int> parseRHS();

Token peek();

protected:
public:
  Parser();
  Grammar getGrammar();

};
