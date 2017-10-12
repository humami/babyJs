#ifndef BJS_H
#define BJS_H

#include<iostream>
#include<vector>
#include<map>
#include<string>
#include<memory>
#include<queue>
using namespace std;

extern FILE *sourcefile;

enum VALUE_TYPE
{
	Type_Unknow,
	Type_Int,
	Type_String,
};

enum FLAG_TYPE
{
	Type_Break = 2,
};

class Ast;
class LVarAst;

struct Func
{
	string func_name;
	vector<shared_ptr<LVarAst>> params;
	vector<shared_ptr<Ast>> block;
};

struct Value
{
	string var_name;
	VALUE_TYPE type;
	string func_name;
	string str;

	union
	{
		int int_val;               //整形数值           
	}v;
};

//variable_table
extern vector<Value> VariableEnv;
//function_table
extern map<string, Func> FuncEnv;
//string_table
extern map<string, string> StringEnv;

#endif