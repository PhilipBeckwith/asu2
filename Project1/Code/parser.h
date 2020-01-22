#pragma once
/*
 * Copyright (C) Rida Bazzi, 2019
 *
 * Do not share this file with anyone
 */
#ifndef __PARSER_H__
#define __PARSER_H__

#include <string>
#include "lexer.h"
#include "program.h"

class Parser
{
  private:
    LexicalAnalyzer lexer;
    Program program;

    void syntax_error();
    Token expect(TokenType expected_type);
    Token peek();

    //Parsing Functions
    void parseInput();
    void parseProgram();
    void parseProc_decl_section();
    void parseProc_decl();
    std::string parseProcedure_name();
    Statement* parseProcedure_body(); // this is where the lits are added to the program
    Statement* parseStatment_list(); // builds the linked lists and returns
    Statement* parseStatment(); // kinda acts like a switch statment to deturmin type
    Statement* parseInput_statment(); // returns a built statment object pointer null pointer to next
    Statement* parseOutput_Statment(); //..
    Statement* parseProcedure_invocation(); //..
    Statement* parseDo_statement();//..
    Statement* parsAessign_statement(); //..
    Statement* parseExpr();//deturmins what kind of operation so needs to return *statement
    TokenType parseOperator();
    int parsePrimary();
    void parseMain();
    void parseInputs();

public:
    Program* Parse();

};

#endif
