#ifndef PARSER_H
#define PARSER_H

#include"Bjs.h"
#include"lex.h"
#include"ast.h"

class Parser
{
    public:
		void parser();
};

void GetNextToken();
bool match(int tk);
void ReadToken(int tk);
void ReadToken();

shared_ptr<Ast> ParserStmt();

shared_ptr<Ast> ParserDef();
shared_ptr<Ast> ParserPrint();

shared_ptr<Ast> ParserIf();
shared_ptr<Ast> ParserElseIf();
shared_ptr<Ast> ParserElse();

shared_ptr<Ast> ParserWhile();
shared_ptr<Ast> ParserBreak();

shared_ptr<Ast> ParserFunction();
shared_ptr<Ast> ParserCaller();
shared_ptr<Ast> ParserReturn();

shared_ptr<Ast> ParserExpr();
shared_ptr<Ast> ParserAssign();
shared_ptr<Ast> ParserEquality();
shared_ptr<Ast> ParserRelation();
shared_ptr<Ast> ParserAdditive(); 
shared_ptr<Ast> ParserMultiplicative();
shared_ptr<Ast> ParserPrimary();

#endif