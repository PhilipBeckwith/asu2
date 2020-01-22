#include "lexer.h"
#include "compiler.h"
#include "Memory.h"
#include <iostream>


class Parser
{
private:
  LexicalAnalyzer lexer;
  MemoryManagment memory;

  struct StatementNode *head;

  //Parsing functions
  void parseProgram();
  void parseVar_Section();
  void parseID_List();
  struct StatementNode *parseBody();
  struct StatementNode *parseStatement_List();
  struct StatementNode *parseStatement();
  struct AssignmentStatement *parseAssign();
  void parseExper(struct AssignmentStatement *assign);
  struct ValueNode* parsePrimary();
  ArithmeticOperatorType parseOP();
  struct PrintStatement *parsePrint();
  struct IfStatement *parseWhile(StatementNode* statement);
  struct IfStatement *parseIF();
  struct IfStatement *parseCondition();
  ConditionalOperatorType parseRelationOP();
  struct StatementNode* parseSwitch();
  struct StatementNode* parseFor();
  struct StatementNode * parseCase_List(struct ValueNode* value, struct StatementNode* fallow);
  struct StatementNode *parseCase(struct ValueNode* value,struct StatementNode* fallow);
  struct StatementNode* parseDefaultCase(struct StatementNode* fallow);

  //Parsing tools
  void syntaxError();

  bool checkNext(TokenType type);
  bool checkSecond(TokenType type);
  bool checkThird(TokenType type);

  Token peek();
  Token expect(TokenType type);
  Token getToken();
  void returnToken(Token token);

  struct StatementNode *makeStatmentNode();

protected:
public:
  Parser();
  struct StatementNode* parse();

};
