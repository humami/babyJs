#ifndef AST_H
#define AST_H

#include"Bjs.h"

class Ast
{
	public:
		virtual VALUE_TYPE getType(){ return VALUE_TYPE::Type_Unknow;  };

		virtual string getStr() { return "unknow"; };

		virtual int excute() = 0;

		virtual ~Ast(){};
};

class NumAst : public Ast
{
	int Value;

	public:
		VALUE_TYPE getType(){ return VALUE_TYPE::Type_Int; };

		int excute();

		NumAst(int v) : Value(v){};
};

class LVarAst : public Ast
{
	string Name;
	string FuncName;

    public:
		int excute();

		string getName() { return Name; };

		string getFuncName() { return FuncName; };

		LVarAst(string name, string funcname) : Name(name), FuncName(funcname){};
};

class RVarAst : public Ast
{
	string Name;
	string FuncName;

	public:
		int excute();

		string getName() { return Name; };

		string getFuncName() { return FuncName; };

		RVarAst(string name, string funcname) : Name(name), FuncName(funcname) {};
};

class ConstStrAst : public Ast
{
	string Str;

public:
	VALUE_TYPE getType(){ return VALUE_TYPE::Type_String; };

	int excute();

	string getStr() { return Str; };

	ConstStrAst(string str) : Str(str) {};
};

class BinaryExprAst : public Ast
{
	string Op;
	shared_ptr<Ast> Lhs, Rhs;

	public:
		VALUE_TYPE getType() { return VALUE_TYPE::Type_Int; };

		int excute();

		BinaryExprAst(string op, shared_ptr<Ast>lhs, shared_ptr<Ast> rhs) : Op(op), Lhs(lhs), Rhs(rhs){};
};

class CondExprAst : public Ast
{
	string Op;
	shared_ptr<Ast> Lhs, Rhs;

	public:
		int excute();

		CondExprAst(string op, shared_ptr<Ast>lhs, shared_ptr<Ast> rhs) : Op(op), Lhs(lhs), Rhs(rhs){};
};

class AssignExprAst : public Ast
{
	shared_ptr<LVarAst> Var;
	shared_ptr<Ast> Rhs;

	public:
		int excute();
	
		AssignExprAst(shared_ptr<LVarAst>var, shared_ptr<Ast> rhs) : Var(var), Rhs(rhs){};
};

class PrintExprAst : public Ast
{
	shared_ptr<RVarAst> Var;

	public:
		int excute();

		PrintExprAst(shared_ptr<RVarAst> var) : Var(var){};
};

class IfStmtAst : public Ast
{
	shared_ptr<Ast> Cond;
	vector<shared_ptr<Ast>> Block;
	shared_ptr<Ast> ElseIf;
	shared_ptr<Ast> ELse;

    public:
		int excute();
		
		IfStmtAst(shared_ptr<Ast> cond, vector<shared_ptr<Ast>> &block, shared_ptr<Ast> elseif, shared_ptr<Ast> Else)
			: Cond(cond), Block(block), ElseIf(elseif), ELse(Else){};
};

class ElseStmtAst: public Ast
{
	vector<shared_ptr<Ast>> Block;

	public:
		int excute();

		ElseStmtAst(vector<shared_ptr<Ast>> &block) : Block(block){};
};

class WhileStmtAst : public Ast
{
	shared_ptr<Ast> Cond;
	vector<shared_ptr<Ast>> Block;

	public:
		int excute();

		WhileStmtAst(shared_ptr<Ast> cond, vector<shared_ptr<Ast>> &block) : Cond(cond), Block(block){};
};

class BreakStmtAst : public Ast
{
	public:
		int excute();

		BreakStmtAst(){};
};


class FunctionAst : public Ast
{
	string FuncName;
	vector<shared_ptr<LVarAst>> Params;
	vector<shared_ptr<Ast>> Block;

	public:
		int excute();

		FunctionAst(string funcname, vector<shared_ptr<LVarAst>> &params, vector<shared_ptr<Ast>> &block) : FuncName(funcname), Params(params), Block(block){};
};

class CallerAst : public Ast
{
	string FuncName;
	vector<shared_ptr<Ast>> Params;

	public:
		VALUE_TYPE getType(){ return VALUE_TYPE::Type_Int; };

		int excute();

		CallerAst(string funcname, vector<shared_ptr<Ast>> &params) : FuncName(funcname), Params(params){};
};

class ReturnStmtAst : public Ast
{
	shared_ptr<Ast> Exp;

	public:
		int excute();

		ReturnStmtAst(shared_ptr<Ast> exp) : Exp(exp){};
};

#endif