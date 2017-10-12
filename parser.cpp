#include"parser.h"

Lexer lexer;
vector<Value> VariableEnv;
map<string, Func> FuncEnv;
map<string, string> StringEnv;
string FuncName;
Token CurrToken;

shared_ptr<Ast> ParserDef()
{
	shared_ptr<Ast> defexp;

	GetNextToken();

	ReadToken(TOKEN::TOKEN_IDENT);

	if (match(TOKEN::TK_ASSIGN))
		defexp =  ParserAssign();
	else
		defexp = shared_ptr<LVarAst>(new LVarAst(CurrToken.value, FuncName));

	ReadToken(TOKEN::TOKEN_SEMICOLON);

	return defexp;
}

shared_ptr<Ast> ParserPrint()
{
	shared_ptr<Ast> printexp;
	shared_ptr<RVarAst> var;

	ReadToken(TOKEN::TOKEN_OPEN_PA);

	GetNextToken();

	ReadToken(TOKEN::TOKEN_IDENT);

	var = shared_ptr<RVarAst>(new RVarAst(CurrToken.value, FuncName));

	ReadToken(TOKEN::TOKEN_CLOSE_PA);

	printexp = shared_ptr<Ast>(new PrintExprAst(var));

	ReadToken(TOKEN::TOKEN_SEMICOLON);

	return printexp;
}

shared_ptr<Ast> ParserExpr()
{
	shared_ptr<Ast> exp;

	if (match(TOKEN::TOKEN_OPEN_PA))
		exp = ParserCaller();
	else if (match(TOKEN::TK_ASSIGN))
	    exp = ParserAssign();

	ReadToken(TOKEN::TOKEN_SEMICOLON);

	return exp;
}

shared_ptr<Ast> ParserAssign()
{
	shared_ptr<LVarAst> var;
	shared_ptr<Ast> rhs;

	var = shared_ptr<LVarAst>(new LVarAst(CurrToken.value, FuncName));

	ReadToken(TOKEN::TK_ASSIGN);

	rhs = ParserEquality();

	return shared_ptr<Ast>(new AssignExprAst(var, rhs));
}

shared_ptr<Ast> ParserEquality()
{
	shared_ptr<Ast> lhs;
	shared_ptr<Ast> rhs;
	string op;

	lhs = ParserRelation();

	while (true)
	{
		if (!match(TOKEN::TOKEN_EQ) && !match(TOKEN::TOKEN_NEQ))
			break;

		GetNextToken();

		op = CurrToken.value;

		ReadToken();

		rhs = ParserRelation();

		lhs = shared_ptr<Ast>(new CondExprAst(op, lhs, rhs));
	}

	return lhs;
}

shared_ptr<Ast> ParserRelation()
{
	shared_ptr<Ast> lhs;
	shared_ptr<Ast> rhs;
	string op;

	lhs = ParserAdditive();

	while (true)
	{
		if (!match(TOKEN::TOKEN_LT) && !match(TOKEN::TOKEN_LEQ) && !match(TOKEN::TOKEN_GT) && !match(TOKEN::TOKEN_GEQ))
			break;

		GetNextToken();

		op = CurrToken.value;

		ReadToken();

		rhs = ParserAdditive();

		lhs = shared_ptr<Ast>(new CondExprAst(op, lhs, rhs));
	}

	return lhs;
}

shared_ptr<Ast> ParserAdditive()
{
	shared_ptr<Ast> lhs;
	shared_ptr<Ast> rhs;
	string op;

	lhs = ParserMultiplicative();

	while (true)
	{
		if (!match(TOKEN::TOKEN_ADD) && !match(TOKEN::TOKEN_SUB))
			break;
		
		GetNextToken();

		op = CurrToken.value;

		ReadToken();

		rhs = ParserMultiplicative();

		lhs = shared_ptr<Ast>(new BinaryExprAst(op, lhs, rhs));
	}

	return lhs;
}

shared_ptr<Ast> ParserMultiplicative()
{
	shared_ptr<Ast> lhs;
	shared_ptr<Ast> rhs;
	string op;

	shared_ptr<Ast> binary;

	lhs = ParserPrimary();

	while (true)
	{
		if (!match(TOKEN::TOKEN_MLU) && !match(TOKEN::TOKEN_DIV) && !match(TOKEN::TOKEN_MOD))
			break;

		GetNextToken();

		op = CurrToken.value;

		ReadToken();

		rhs = ParserPrimary();

		lhs = shared_ptr<Ast>(new BinaryExprAst(op, lhs, rhs));
	}

	return lhs;
}

shared_ptr<Ast> ParserPrimary()
{
	bool minu_flag = false;

	GetNextToken();

	if (CurrToken.value == "-")
	{
		minu_flag = true;
		GetNextToken();
	}

	switch (CurrToken.tk)
	{
		case TOKEN::TOKEN_INT:
		{
			int val = atoi(CurrToken.value.c_str());

			if (minu_flag)
				val = -val;

			ReadToken();

			return shared_ptr<Ast>(new NumAst(val));
		}

		case TOKEN::TOKEN_STRING:
		{
			string val = CurrToken.value;

			ReadToken(TOKEN::TOKEN_STRING);

			return shared_ptr<Ast>(new ConstStrAst(val));
		}

		case TOKEN::TOKEN_IDENT:
		{
			string value = CurrToken.value;

			ReadToken();

			if (match(TOKEN::TOKEN_OPEN_PA))
				return ParserCaller();

			return shared_ptr<Ast>(new RVarAst(value, FuncName));
		}

		case TOKEN::TOKEN_OPEN_PA:
		{
			shared_ptr<Ast> e;

			while (CurrToken.tk != TOKEN::TOKEN_CLOSE_PA)
			{
				ReadToken(TOKEN::TOKEN_OPEN_PA);

				e = ParserEquality();

				GetNextToken();
			}

			ReadToken(TOKEN::TOKEN_CLOSE_PA);

			return e;
		}

		case TOKEN::KW_TRUE:
		{
			ReadToken(TOKEN::KW_TRUE);

			return shared_ptr<Ast>(new CondExprAst("true", nullptr, nullptr));
		}

		case TOKEN::KW_FALSE:
		{
			ReadToken(TOKEN::KW_FALSE);

			return shared_ptr<Ast>(new CondExprAst("false", nullptr, nullptr));
		}

		default:
			return nullptr;
	}
}

shared_ptr<Ast> ParserStmt()
{
	if (match(TOKEN::KW_VAR))
	{ 
		ReadToken(TOKEN::KW_VAR);

		return ParserDef();
	}
	else if (match(TOKEN::KW_FUNCTION))
	{
		ReadToken(TOKEN::KW_FUNCTION);

		return ParserFunction();
	}
	else if (match(TOKEN::KW_IF))
	{
		ReadToken(TOKEN::KW_IF);

		return ParserIf();
	}
	else if (match(TOKEN::KW_WHILE))
	{
		ReadToken(TOKEN::KW_WHILE);

		return ParserWhile();
	}
	else if (match(TOKEN::KW_BREAK))
	{
		ReadToken(TOKEN::KW_BREAK);

		return ParserBreak();
	}
	else if (match(TOKEN::KW_RETURN))
	{
		ReadToken(TOKEN::KW_RETURN);

		return ParserReturn();
	}
	else if (match(TOKEN::KW_PRINT))
	{
		ReadToken(TOKEN::KW_PRINT);

		return ParserPrint();
	}
	else if (match(TOKEN::TOKEN_IDENT))
	{
		GetNextToken();

		ReadToken(TOKEN::TOKEN_IDENT);

		return ParserExpr();
	}
	else if (match(TOKEN::TOKEN_SEMICOLON))
		ReadToken(TOKEN::TOKEN_SEMICOLON);

	return nullptr;
}

shared_ptr<Ast> ParserIf()
{
	shared_ptr<Ast> cond;
	vector<shared_ptr<Ast>> block;
	shared_ptr<Ast> elseif = nullptr;
	shared_ptr<Ast> Else = nullptr;
	shared_ptr<Ast> e;

	ReadToken(TOKEN::TOKEN_OPEN_PA);

	cond = ParserEquality();

	ReadToken(TOKEN::TOKEN_CLOSE_PA);

	if (match(TOKEN::TOKEN_OPEN_BRACKET))
	{
		ReadToken(TOKEN::TOKEN_OPEN_BRACKET);

		while (!match(TOKEN::TOKEN_CLOSE_BRACKET))
		{
			e = ParserStmt();

			if (e)
				block.push_back(e);
		}

		ReadToken(TOKEN::TOKEN_CLOSE_BRACKET);
	}
	else
	{                                                                                        
		e = ParserStmt();
		block.push_back(e);
	}

	if (match(TOKEN::KW_ELSE))
	{
		ReadToken(TOKEN::KW_ELSE);

		if (match(TOKEN::KW_IF))
		{
			ReadToken(TOKEN::KW_IF);

			elseif = ParserIf();
		}
		else
			Else = ParserElse();
	}

	return shared_ptr<Ast>(new IfStmtAst(cond, block, elseif, Else));
}

shared_ptr<Ast> ParserElse()
{
	vector<shared_ptr<Ast>> block;
	shared_ptr<Ast> e;

	if (match(TOKEN::TOKEN_OPEN_BRACKET))
	{
		ReadToken(TOKEN::TOKEN_OPEN_BRACKET);

		while (!match(TOKEN::TOKEN_CLOSE_BRACKET))
		{
			e = ParserStmt();

			if (e)
				block.push_back(e);
		}

		ReadToken(TOKEN::TOKEN_CLOSE_BRACKET);
	}
	else
	{
		e = ParserStmt();
		block.push_back(e);
	}

	return shared_ptr<Ast>(new ElseStmtAst(block));
}

shared_ptr<Ast> ParserWhile()
{
	shared_ptr<Ast> cond;
	vector<shared_ptr<Ast>> block;
	shared_ptr<Ast> e;

	ReadToken(TOKEN::TOKEN_OPEN_PA);

	cond = ParserEquality();

	ReadToken(TOKEN::TOKEN_CLOSE_PA);

	if (match(TOKEN::TOKEN_OPEN_BRACKET))
	{
		ReadToken(TOKEN::TOKEN_OPEN_BRACKET);

		while (!match(TOKEN::TOKEN_CLOSE_BRACKET))
		{
			e = ParserStmt();

			if (e)
				block.push_back(e);
		}

		ReadToken(TOKEN::TOKEN_CLOSE_BRACKET);
	}
	else
	{
		e = ParserStmt();
		block.push_back(e);
	}

	return shared_ptr<Ast>(new WhileStmtAst(cond, block));
}


shared_ptr<Ast> ParserBreak()
{
	ReadToken(TOKEN::TOKEN_SEMICOLON);

	return shared_ptr<Ast>(new BreakStmtAst());
}

shared_ptr<Ast> ParserFunction()
{
	string funcname;
	vector<shared_ptr<LVarAst>> params;
	vector<shared_ptr<Ast>> block;
	shared_ptr<LVarAst> var;
	shared_ptr<Ast> e;
	Func function;

	GetNextToken();

	funcname = CurrToken.value;
	FuncName = CurrToken.value;

	auto it = FuncEnv.find(FuncName);

	if (it != FuncEnv.end())
	{
		cout << "已经有同名函数\n";

		exit(0);
	}
	else
		function.func_name = FuncName;

	ReadToken(TOKEN::TOKEN_IDENT);

	ReadToken(TOKEN::TOKEN_OPEN_PA);

	while (!match(TOKEN::TOKEN_CLOSE_PA))
	{
		GetNextToken();

		ReadToken(TOKEN::TOKEN_IDENT);

		var = shared_ptr<LVarAst>(new LVarAst(CurrToken.value, FuncName));

		if (var)
			params.push_back(var);

		if (match(TOKEN::TOKEN_COMMA))
			ReadToken(TOKEN::TOKEN_COMMA);
	}

	ReadToken(TOKEN::TOKEN_CLOSE_PA);

	if (match(TOKEN::TOKEN_OPEN_BRACKET))
	{
		ReadToken(TOKEN::TOKEN_OPEN_BRACKET);

		while (!match(TOKEN::TOKEN_CLOSE_BRACKET))
		{
			e = ParserStmt();

			if (e)
				block.push_back(e);
		}

		ReadToken(TOKEN::TOKEN_CLOSE_BRACKET);
	}
	else
	{
		e = ParserStmt();
		block.push_back(e);
	}

	function.params = params;
	function.block = block;

	FuncEnv.insert(pair<string, Func>(funcname, function));

	return shared_ptr<Ast>(new FunctionAst(funcname, params, block));
}

shared_ptr<Ast> ParserCaller()
{
	string funcname;
	vector<shared_ptr<Ast>> params;
	shared_ptr<Ast> var;

	funcname = CurrToken.value;

	ReadToken(TOKEN::TOKEN_OPEN_PA);

	while (!match(TOKEN::TOKEN_CLOSE_PA))
	{
		var = ParserPrimary();

		if (var)
			params.push_back(var);

		if (match(TOKEN::TOKEN_COMMA))
			ReadToken(TOKEN::TOKEN_COMMA);
	}

	ReadToken(TOKEN::TOKEN_CLOSE_PA);


	return shared_ptr<Ast>(new CallerAst(funcname, params));
}

shared_ptr<Ast> ParserReturn()
{
	shared_ptr<Ast> exp;

	exp = ParserEquality();

	ReadToken(TOKEN::TOKEN_SEMICOLON);

	return shared_ptr<Ast>(new ReturnStmtAst(exp));
}


void Parser::parser()
{
	while (lexer.GetToken().tk != TOKEN::TOKEN_EOF);

	while (true)
	{
		if (lexer.token_list.front().tk == TOKEN::TOKEN_EOF)
			break;

		shared_ptr<Ast> E = ParserStmt();

		E->excute();
	}
}

void GetNextToken()
{
	CurrToken = lexer.token_list.front();
}

bool match(int tk)
{
	if (lexer.token_list.front().tk == tk)
		return true;

	return false;
}

void ReadToken(int tk)
{
	if (match(tk))
		lexer.token_list.pop();
	else
	{
		cout << "出错" << endl;
		exit(0);
	}
}

void ReadToken()
{
    lexer.token_list.pop();
}


