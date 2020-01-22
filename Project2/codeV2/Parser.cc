#include "Parser.h"

Parser::Parser()
{
  parseRuleList();
  grammar.finishInitalization();
}

Token Parser::peek(){
  Token token=lexer.GetToken();
  lexer.UngetToken(token);
  return token;
}

void Parser::parseRuleList(){
  parseRule();
  Token token=peek();
  if(token.token_type==ID)
  {
    parseRuleList();
  }

}

std::vector<int> Parser::parseIDList(){
  std::vector<int> idList;
  Token token=peek();
  if(token.token_type==HASH)
  {
    idList.push_back(grammar.addSymbol("#"));
  }
  while(token.token_type!=HASH)
  {
    token=lexer.GetToken();
    idList.push_back(grammar.addSymbol(token.lexeme));
    token=peek();
  }
  return idList;
}

void Parser::parseRule(){
  Rule rule;
  Token token=lexer.GetToken();
  rule.LHS=grammar.addSymbol(token.lexeme);
  lexer.GetToken();//this should be an arrow
  rule.RHS=parseRHS();
  lexer.GetToken();//This should be a hash
  grammar.addRule(rule);
}

std::vector<int> Parser::parseRHS(){//doesn't strictly fallow grammar.. allwell

  return parseIDList();
}

Grammar Parser::getGrammar(){
  return grammar;
}
