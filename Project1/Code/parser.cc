/*
 * Copyright (C) Rida Bazzi, 2017
 *
 * Do not share this file with anyone
 *
 * Do not post this file or derivatives of
 * of this file online
 *
 */
#include <iostream>
#include <cstdlib>
#include "parser.h"

using namespace std;

void Parser::syntax_error()
{
    cout << "SYNTAX ERROR\n";
    exit(1);
}

// this function gets a token and checks if it is
// of the expected type. If it is, the token is
// returned, otherwise, synatx_error() is generated
// this function is particularly useful to match
// terminals in a right hand side of a rule.
// Written by Mohsen Zohrevandi
Token Parser::expect(TokenType expected_type)
{
    Token t = lexer.GetToken();
    if (t.token_type != expected_type)
        syntax_error();
    return t;
}

// this function simply checks the next token without
// consuming the input
// Written by Mohsen Zohrevandi
Token Parser::peek()
{
    Token t = lexer.GetToken();
    lexer.UngetToken(t);
    return t;
}

// Parsing

/*
input → program inputs
 */
void Parser::parseInput()
{
  parseProgram();
  parseInputs();
}

/*
program → main
program → proc_decl_section main
 */
void Parser::parseProgram()
{
  Token token =peek();

  if(token.token_type == MAIN)
  {
    parseMain();
  }
  else
  {
    parseProc_decl_section();
    parseMain();
  }
}

/*
proc_decl_section → proc_decl
proc_decl_section → proc_decl proc_decl_section
 */
void Parser::parseProc_decl_section()
{

  parseProc_decl();

  Token token = peek();

  if(token.token_type == PROC)
  {
    parseProc_decl_section();
  }
}

/*
proc_decl → PROC procedure_name procedure_body ENDPROC
 */
void Parser::parseProc_decl()
{
  std::string procName;
  Statement* statement;
  expect(PROC);
  procName=parseProcedure_name();
  statement = parseProcedure_body();
  expect(ENDPROC);

  program.addStatementList(procName,statement);
}

/*
procedure_name → ID
procedure_name → NUM
 */
std::string Parser::parseProcedure_name()
{
  Token token = lexer.GetToken();
  if(token.token_type == ID){}
  else if(token.token_type == NUM){}
  else {syntax_error();}
  return token.lexeme;
}

/*
procedure_body → statement_list
 */
Statement* Parser::parseProcedure_body()
{
  Statement* statement;
  statement = parseStatment_list();

  return statement;
}

/*
statement_list → statement
statement_list → statement statement_list
 */
Statement* Parser::parseStatment_list()
{
  Statement* statement;
  statement = parseStatment();
  statement->next=NULL;

  Token token = peek();

  if(token.token_type == INPUT ||token.token_type == OUTPUT ||
     token.token_type == DO)
  {
    statement->next=parseStatment_list();
  }
  else if(token.token_type==ID || token.token_type==NUM)
  {
    Token token1 = lexer.GetToken();
    Token token2 = lexer.GetToken();
    lexer.UngetToken(token2);
    lexer.UngetToken(token1);
    if(token2.token_type==SEMICOLON || token2.token_type==EQUAL)
    {
      statement->next=parseStatment_list();
    }
  }
  return statement;
}

/*
statement → input_statement
statement → output_statement
statement → procedure_invocation
statement → do_statement
statement → assign_statement
 */
Statement* Parser::parseStatment()
{
  Statement *statement;
  Token token1, token2;
  token1=lexer.GetToken();
  token2=lexer.GetToken();
  lexer.UngetToken(token2);
  lexer.UngetToken(token1);

  if(token1.token_type==INPUT){statement=parseInput_statment();}
  else if(token1.token_type==OUTPUT){statement=parseOutput_Statment();}
  else if(token1.token_type==DO){statement=parseDo_statement();}

  else if((token1.token_type==ID || token1.token_type==NUM)
           && token2.token_type==SEMICOLON)
  {statement=parseProcedure_invocation();}
  else if(token1.token_type==ID && token2.token_type==EQUAL)
  {statement=parsAessign_statement();}
  else
  {
    statement=NULL;
    syntax_error();
  }
  return statement;
}

/*
input_statement → INPUT ID SEMICOLON
 */
Statement* Parser::parseInput_statment()
{
  Statement *statement=new(Statement);
  Token id;

  expect(INPUT);
  id = expect(ID);
  expect(SEMICOLON);

  statement->type=INPUT;
  statement->op1=program.addVariable(id.lexeme);
  return statement;
}

/*
output_statement → OUTPUT ID SEMICOLON
 */
Statement* Parser::parseOutput_Statment()
{
  Token id;
  Statement *statement=new(Statement);

  expect(OUTPUT);
  id = expect(ID);
  expect(SEMICOLON);

  statement->type=OUTPUT;
  statement->op1=program.addVariable(id.lexeme);
  return statement;
}

/*
procedure_invocation → procedure_name SEMICOLON
 */
Statement* Parser::parseProcedure_invocation()
{
  Statement *statement=new(Statement);
  std::string name;

  name=parseProcedure_name();
  expect(SEMICOLON);

  statement->type=PROC;
  statement->procCall=program.getProcLocation(name);

  return statement;
}

/*
do_statement → DO ID procedure_invocation
 */
Statement* Parser::parseDo_statement()
{
  Token id;
  Statement *statement;

  expect(DO);
  id = expect(ID);
  statement=parseProcedure_invocation();

  statement->type=DO;
  statement->op1=program.addVariable(id.lexeme);

  return statement;
}

/*
assign_statement → ID EQUAL expr SEMICOLON
 */
Statement* Parser::parsAessign_statement()
{
  Token id;
  Statement* statement;

  id=expect(ID);
  expect(EQUAL);
  statement=parseExpr();
  expect(SEMICOLON);

  statement->type=EQUAL;
  statement->LHS=program.addVariable(id.lexeme);
  return statement;
}

/*
expr → primary
expr → primary operator primary
 */
Statement* Parser::parseExpr()
{
  Statement *statement=new(Statement);

  statement->op1=parsePrimary();
  statement->oppType=END_OF_FILE;

  Token token = peek();
  if(token.token_type==PLUS || token.token_type==MINUS ||
     token.token_type==MULT || token.token_type==DIV)
     {
       statement->oppType=parseOperator();
       statement->op2=parsePrimary();
     }
     return statement;
}

/*
operator → PLUS
operator → MINUS
operator → MULT
operator → DIV
 */
TokenType Parser::parseOperator()
{
  Token token = lexer.GetToken();

  if(token.token_type==PLUS){}
  else if(token.token_type==MINUS){}
  else if(token.token_type==MULT){}
  else if(token.token_type==DIV){}
  else{syntax_error();}

  return token.token_type;
}

/*
primary → ID
primary → NUM
 */
int Parser::parsePrimary()
{
  Token token = lexer.GetToken();
  int memoryLocation=0;

  if(token.token_type==ID)
  {
    memoryLocation=program.addVariable(token.lexeme);
  }
  else if(token.token_type==NUM)
  {
    memoryLocation=program.addConstant(std::atoi(token.lexeme.c_str()));
  }
  else{syntax_error();}

  return memoryLocation;
}

/*
main → MAIN procedure_body
 */
void Parser::parseMain()
{
  expect(MAIN);
  program.setMain(parseProcedure_body());

}

/*
inputs → NUM
inputs → NUM inputs
 */
void Parser::parseInputs()
{
  Token token = lexer.GetToken();
  Token lookAhead = peek();

  program.addInputs(atoi(token.lexeme.c_str()));

  if(lookAhead.token_type == NUM)
  {
    parseInputs();
  }
  else if(lookAhead.token_type==END_OF_FILE){}
  else{syntax_error();}
}


Program* Parser::Parse()
{
  parseInput();

  return &program;
}
