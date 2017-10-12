#ifndef LEX_H
#define LEX_H

#include"Bjs.h"

enum LEX_STATE
{
	LEX_START,
	LEX_NUMBER,
	LEX_STRING,
	LEX_STRING_ESCAPE,
	LEX_IDENT,
	LEX_DELIM,
	LEX_OP,
};

enum TOKEN
{
	KW_VAR,                           // var
	KW_PRINT,                         // print
	KW_IF,                            // if
	KW_ELSE,                          // else
	KW_WHILE,                         // while
	KW_FUNCTION,                      // function
	KW_BREAK,                         // break
	KW_RETURN,                        // return
	KW_TRUE,                          // true
	KW_FALSE,                         // false

	TOKEN_INT,
	TOKEN_STRING,

	TOKEN_ADD,                         // +
	TOKEN_SUB,                         // -
	TOKEN_MLU,                         // *
	TOKEN_DIV,                         // /
	TOKEN_MOD,                         // %
	TOKEN_EQ,                          // ==
	TOKEN_NEQ,                         // !=
	TOKEN_LT,                          // <
	TOKEN_LEQ,                         // <=
	TOKEN_GT,                          // >
	TOKEN_GEQ,                         // >=
	TK_ASSIGN,                         // =

	TOKEN_OPEN_PA,                     // (
	TOKEN_CLOSE_PA,                    // )
	TOKEN_OPEN_BRACKET,                // {
	TOKEN_CLOSE_BRACKET,               // }
	TOKEN_COMMA,                       // ,
	TOKEN_SEMICOLON,                   // ;

	TOKEN_IDENT,

	TOKEN_EOF
};

struct Token
{
	TOKEN tk;
	string value;
};

class Lexer
{
	LEX_STATE state;
	char c;

    public:
		queue<Token> token_list;
		
		bool is_white();
		bool is_num();
		bool is_ident();
		bool is_delim();
		bool is_op();

		Token GetToken();
};


#endif
