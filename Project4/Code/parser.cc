#include "parser.h"

Parser::Parser(){}

void Parser::parseProgram()
{
  parseVar_Section();
  head=parseBody();
  expect(END_OF_FILE);
}

void Parser::parseVar_Section()
{
  parseID_List();
  expect(SEMICOLON);
}

void Parser::parseID_List()
{
    Token id=getToken();
    memory.addVariable(id.lexeme);
    if(!checkNext(SEMICOLON))
    {
      expect(COMMA);
      parseID_List();
    }
}

struct StatementNode *Parser::parseBody()
{
  struct StatementNode *statement;
  expect(LBRACE);
  statement=parseStatement_List();
  expect(RBRACE);
  return statement;
}

struct StatementNode *Parser::parseStatement_List()
{
  struct StatementNode *statement;
  statement=parseStatement();

  if(statement->type==IF_STMT)
  {
    StatementNode* noOp=new(StatementNode);
    noOp->type=NOOP_STMT;
    statement->next=noOp;
    statement->if_stmt->false_branch=noOp;

    StatementNode *node=statement->if_stmt->true_branch;
    while(node->next!=NULL){node=node->next;}
    node->next=noOp;
  }

  if(checkNext(ID)||checkNext(WHILE)||checkNext(PRINT)||checkNext(FOR)||checkNext(IF)||checkNext(SWITCH))
  {
    if(statement->type==IF_STMT){statement->next->next=parseStatement_List();}
    else if(statement->type==ASSIGN_STMT && statement->next!=NULL)
    {
      if(statement->next->type==IF_STMT){statement->next->next->next=parseStatement_List();}
    }
    else if(statement->type==NOOP_STMT)
    {
      struct StatementNode *caselist=statement;
      while(caselist->next!=NULL){caselist=caselist->next;}
      caselist->next=parseStatement_List();
    }
    else{statement->next=parseStatement_List();}
  }

  if(statement->type==NOOP_STMT){statement->type=IF_STMT;}

  return statement;
}

struct StatementNode *Parser::parseStatement()
{
  Token token=peek();
  struct StatementNode *statement=makeStatmentNode();
  statement->next=NULL;
  switch (token.token_type)
  {
    case ID:
      statement->assign_stmt=parseAssign();
      statement->type=ASSIGN_STMT;
      break;

    case WHILE:
      statement->if_stmt=parseWhile(statement);
      statement->type=IF_STMT;
      break;

    case SWITCH:
      delete(statement);
      statement=parseSwitch();
      statement->type=NOOP_STMT;
      break;

    case PRINT:
      statement->print_stmt=parsePrint();
      statement->type=PRINT_STMT;
      break;

    case FOR:
    delete(statement);
    statement=parseFor();
      break;

    case IF:
      statement->if_stmt=parseIF();
      statement->type=IF_STMT;
      break;

    default: syntaxError();
  }
  return statement;
}

struct AssignmentStatement *Parser::parseAssign()
{
  struct AssignmentStatement *assign=new(struct AssignmentStatement);
  Token id=expect(ID);
  assign->left_hand_side=memory.lookUpVar(id.lexeme);
  expect(EQUAL);
  if(checkSecond(SEMICOLON))
  {
    assign->operand1=parsePrimary();
    assign->operand2=NULL;
    assign->op=OPERATOR_NONE;
  }
  else{parseExper(assign);}
  expect(SEMICOLON);

  return assign;
}

void Parser::parseExper(struct AssignmentStatement *assign)
{
  assign->operand1=parsePrimary();
  assign->op=parseOP();
  assign->operand2=parsePrimary();
}

struct ValueNode* Parser::parsePrimary()
{
  ValueNode *node=NULL;

  if(!(checkNext(ID)||checkNext(NUM))){ syntaxError();}
  Token primary=getToken();

  if(primary.token_type==NUM)
  {
    node=memory.addConstant(std::stoi(primary.lexeme));
  }
  else{node=memory.lookUpVar(primary.lexeme);}

  return node;
}

ArithmeticOperatorType Parser::parseOP()
{
  if(!(checkNext(PLUS)||checkNext(MINUS)||checkNext(MULT)||checkNext(DIV)))
  {
    syntaxError();
  }
  Token op=getToken();

  switch(op.token_type)
  {
    case PLUS: return OPERATOR_PLUS;
    case MINUS: return OPERATOR_MINUS;
    case MULT: return OPERATOR_MULT;
    case DIV: return OPERATOR_DIV;
  }

  return OPERATOR_NONE;
}

struct PrintStatement *Parser::parsePrint()
{
  struct PrintStatement *print=new(struct PrintStatement);
  expect(PRINT);
  Token id=expect(ID);
  print->id=memory.lookUpVar(id.lexeme);
  expect(SEMICOLON);
  return print;
}

struct IfStatement *Parser::parseWhile(StatementNode *Statement)
{
  struct IfStatement *statement;
  expect(WHILE);
  statement=parseCondition();
  statement->true_branch=parseBody();

  struct StatementNode *goTo=new(struct StatementNode);
  goTo->goto_stmt=new(struct GotoStatement);
  goTo->goto_stmt->target=Statement;
  goTo->type=GOTO_STMT;

  struct StatementNode *node=statement->true_branch;

  while(node->next!=NULL){node=node->next;}
  node->next=goTo;

  return statement;
}

struct IfStatement *Parser::parseIF()
{
    struct IfStatement *ifStatment;
    expect(IF);
    ifStatment=parseCondition();
    ifStatment->true_branch=parseBody();

    return ifStatment;
}

struct IfStatement *Parser::parseCondition()
{
  struct IfStatement *ifStatment=new(struct IfStatement);
  ifStatment->true_branch=NULL;
  ifStatment->false_branch=NULL;
  ifStatment->condition_operand1=parsePrimary();
  ifStatment->condition_op=parseRelationOP();
  ifStatment->condition_operand2=parsePrimary();
  return ifStatment;
}

 ConditionalOperatorType Parser::parseRelationOP()
{
  if(!(checkNext(GREATER)||checkNext(LESS)||checkNext(NOTEQUAL)))
  {syntaxError();}
  Token relationOP=getToken();

  switch(relationOP.token_type)
  {
      case GREATER: return CONDITION_GREATER;
      case LESS: return CONDITION_LESS;
      case NOTEQUAL: return CONDITION_NOTEQUAL;
  }
  return CONDITION_NOTEQUAL;
}

struct StatementNode* Parser::parseSwitch()
{
  struct StatementNode* caseList;
  struct StatementNode* goTo=new(struct StatementNode);
  goTo->type=NOOP_STMT;
  goTo->next=NULL;

  expect(SWITCH);
  Token id=expect(ID);
  expect(LBRACE);
  caseList=parseCase_List(memory.lookUpVar(id.lexeme), goTo);


  if(!checkNext(RBRACE))
  {
    struct StatementNode* caseNode=caseList;
    while(caseNode->next->type!=NOOP_STMT){caseNode=caseNode->next;}
    caseNode->next=parseDefaultCase(goTo);
    caseNode->if_stmt->true_branch=caseNode->next;
  }
  expect(RBRACE);

  return caseList;
}

struct StatementNode *Parser::parseFor()
{
  struct StatementNode *assignStat1=new(struct StatementNode);
  assignStat1->type=ASSIGN_STMT;
  struct StatementNode *ifStat=new(struct StatementNode);
  ifStat->type=IF_STMT;
  assignStat1->next=ifStat;

  struct StatementNode *assignStat2=new(struct StatementNode);
  assignStat2->type=ASSIGN_STMT;

  struct StatementNode *goTo=new(struct StatementNode);
  goTo->type=GOTO_STMT;
  goTo->goto_stmt=new(struct GotoStatement);
  goTo->goto_stmt->target=ifStat;
  goTo->next=ifStat;
  assignStat2->next=goTo;

  struct StatementNode *body;

  expect(FOR);
  expect(LPAREN);
  assignStat1->assign_stmt=parseAssign();
  ifStat->if_stmt=parseCondition();
  expect(SEMICOLON);
  assignStat2->assign_stmt=parseAssign();
  expect(RPAREN);
  body=parseBody();

  ifStat->next=new(struct StatementNode);
  ifStat->next->type=NOOP_STMT;
  ifStat->if_stmt->false_branch=ifStat->next;
  ifStat->if_stmt->true_branch=body;

  while(body->next!=NULL){body=body->next;}
  body->next=assignStat2;

  return assignStat1;
}

struct StatementNode* Parser::parseCase_List(struct ValueNode* value, struct StatementNode* goTo)
{
  struct StatementNode *caseNode;
  caseNode=parseCase(value, goTo);
  if(checkNext(CASE))
  {
    caseNode->next=parseCase_List(value, goTo);
    caseNode->if_stmt->true_branch=caseNode->next;
  }
  return caseNode;
}

struct StatementNode* Parser::parseCase(struct ValueNode* value, struct StatementNode* goTo)
{
  struct StatementNode *ifstat=new(struct StatementNode);
  ifstat->type=IF_STMT;
  ifstat->if_stmt=new(struct IfStatement);
  ifstat->if_stmt->condition_operand1=value;
  ifstat->if_stmt->condition_op=CONDITION_NOTEQUAL;
  ifstat->if_stmt->true_branch=goTo;
  ifstat->next=goTo;

  expect(CASE);
  Token num=expect(NUM);
  ifstat->if_stmt->condition_operand2=memory.addConstant(stoi(num.lexeme));
  expect(COLON);
  ifstat->if_stmt->false_branch=parseBody();

  struct StatementNode *body = ifstat->if_stmt->false_branch;

  while(body->next!=NULL){body=body->next;}
  body->next=goTo;

  return ifstat;
}

struct StatementNode* Parser::parseDefaultCase(struct StatementNode* goTo)
{
  expect(DEFAULT);
  expect(COLON);
  struct StatementNode* body=parseBody();
  struct StatementNode *node=body;

  while(node->next!=NULL){node=node->next;}
  node->next=goTo;

  return body;

}

void Parser::syntaxError()
{
  std::cout<<"You done did an OOPSIE!";
  std::vector<int> vec;
  vec[600]=3;
  exit(false);
}

bool Parser::checkNext(TokenType type)
{
  Token first=getToken();
  returnToken(first);
  return type==first.token_type;
}
bool Parser::checkSecond(TokenType type)
{
  Token first=getToken();
  Token second=getToken();
  returnToken(second);
  returnToken(first);
  return second.token_type==type;
}

bool Parser::checkThird(TokenType type)
{
  Token first=getToken();
  Token second=getToken();
  Token third=getToken();
  returnToken(third);
  returnToken(second);
  returnToken(first);
  return second.token_type==type;
}

Token Parser::peek()
{
  Token token=getToken();
  returnToken(token);
  return token;
}

Token Parser::expect(TokenType type)
{
  if(!checkNext(type)){
    syntaxError();
  }
  return getToken();
}

Token Parser::getToken()
{
  return lexer.GetToken();
}
void Parser::returnToken(Token token)
{
  lexer.UngetToken(token);
}


struct StatementNode *Parser::makeStatmentNode()
{
  struct StatementNode *statment=new(struct StatementNode);
  statment->next=NULL;
  return statment;
}


struct StatementNode* Parser::parse()
{
  parseProgram();
  return head;
}
