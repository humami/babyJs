#include"ast.h"

static void modifyvalue(string varname, string funcname, Value val)
{
	Value value;

	for (auto it = VariableEnv.begin(); it != VariableEnv.end(); it++)
	{
		if (it->var_name == varname && it->func_name == funcname)
			*it = val;
	}

	return;
}

static Value findvalue(string varname, string funcname)
{
	Value value;

	for (auto it = VariableEnv.begin(); it != VariableEnv.end(); it++)
	{
		if (it->var_name == varname && it->func_name == funcname)
			value = *it;
	}

	return value;
}

int NumAst::excute()
{
	return Value;
}

int LVarAst::excute()
{
	Value value;
	Value v;

	value.var_name = Name;
	value.func_name = FuncName;

	v = findvalue(Name, FuncName);

	if (v.var_name == Name && v.func_name == FuncName)
		return 0;
	else
	    VariableEnv.push_back(value);

	return 0;
}

int RVarAst::excute()
{
	string key = getName();

	string funcname = getFuncName();

	Value value = findvalue(key, funcname);

	return value.v.int_val;
}

int ConstStrAst::excute()
{
	StringEnv.insert(pair<string, string>(Str, Str));

	return 0;
}

int PrintExprAst::excute()
{
	Value value;

	string key = Var->getName();

	string funcname = Var->getFuncName();

	value = findvalue(key, funcname);

	switch (value.type)
	{
		case VALUE_TYPE::Type_Int:
			cout << value.v.int_val << endl;
			break;

		case VALUE_TYPE::Type_String:
			cout << value.str;
			break;
	}

	return 0;
}

int AssignExprAst::excute()
{
	Value value;

	Var->excute();

	string key = Var->getName();

	string funcname = Var->getFuncName();

	value.type = Rhs->getType();

	value.var_name = key;
	value.func_name = funcname;

	if (value.type == VALUE_TYPE::Type_Int)
		value.v.int_val = Rhs->excute();
	else if (value.type == VALUE_TYPE::Type_String)
	{
		Rhs->excute();
		value.str = StringEnv.find(Rhs->getStr())->second;
	}

	modifyvalue(key, funcname, value);

	return 0;
}

int CondExprAst::excute()
{
	bool result;

	if (Op == "==")
		result = Lhs->excute() == Rhs->excute();
	else if (Op == "!=")
		result = Lhs->excute() != Rhs->excute();
	else if (Op == "<")
		result = Lhs->excute() < Rhs->excute();
	else if (Op == "<=")
		result = Lhs->excute() <= Rhs->excute();
	else if (Op == ">")
		result = Lhs->excute() > Rhs->excute();
	else if (Op == ">=")
		result = Lhs->excute() >= Rhs->excute();
	else if (Op == "true")
		result = true;
	else if (Op == "false")
		result = false;

	//cout << result << endl;

	return result;
}

int BinaryExprAst::excute()
{
	int result;

	if (Op == "+")
		result = Lhs->excute() + Rhs->excute();
	else if (Op == "-")
		result = Lhs->excute() - Rhs->excute();
	else if (Op == "*")
		result = Lhs->excute() * Rhs->excute();
	else if (Op == "/")
		result = Lhs->excute() / Rhs->excute();
	else if (Op == "%")
		result = Lhs->excute() % Rhs->excute();

	//cout << result << endl;

	return result;
}

int IfStmtAst::excute()
{
	int cond;

	cond = Cond->excute();

	if (cond)
	{
		for (auto it = Block.begin(); it != Block.end(); it++)
		{
			int result = (*it)->excute();

			if (result == FLAG_TYPE::Type_Break)
				return result;
		}
	}
	else
	{
		if (ElseIf)
			ElseIf->excute();
		else if (ELse)
			ELse->excute();
	}

	return 0;
}

int ElseStmtAst::excute()
{
	for (auto it = Block.begin(); it != Block.end(); it++)
	{
		int result = (*it)->excute();

		if (result == FLAG_TYPE::Type_Break)
			return result;
	}

	return 0;
}

int WhileStmtAst::excute()
{
	int cond;

	cond = Cond->excute();

	while (cond)
	{
		for (auto it = Block.begin(); it != Block.end(); it++)
		{
			int result = (*it)->excute();

			if (result == FLAG_TYPE::Type_Break)
				cond = false;
		}

		cond = Cond->excute();
	}

	return 0;
}

int BreakStmtAst::excute()
{
	return FLAG_TYPE::Type_Break;
}

int FunctionAst::excute()
{
	for (auto it = Params.begin(); it != Params.end(); it++)
	{
		(*it)->excute();
	}

	if (FuncName == "main")
	{
		for (auto it = Block.begin(); it != Block.end(); it++)
		{
			(*it)->excute();
		}
	}

	return 0;
}


int CallerAst::excute()
{
	int result;

	string funcname = FuncName;

	Func function = FuncEnv.find(funcname)->second;

	if (Params.size() > function.params.size())
	{
		cout << "参数形式不符" << endl;

		exit(0);
	}

	auto pit = function.params.begin();

	for (auto ait = Params.begin(); ait != Params.end(); ait++)
	{
		string varname = (*pit)->getName();

		Value value = findvalue(varname, funcname);

		value.v.int_val = (*ait)->excute();

		modifyvalue(varname, funcname, value);

		pit++;
	}

	
	for (auto it = function.block.begin(); it != function.block.end(); it++)
	{
		result = (*it)->excute();
	}

	return result;
}

int ReturnStmtAst::excute()
{
	int result = Exp->excute();

	return result;
}
