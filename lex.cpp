#include"lex.h"

bool Lexer::is_white()
{
	if (c == ' ' || c == '\n' || c == '\t' || c == '\r')
		return true;

	return false;
}

bool Lexer::is_num()
{
	if (c >= '0' && c <= '9')
		return true;
	
	return false;
}

bool Lexer::is_ident()
{

	if ((c >= 'a' && c <= 'z') ||
		(c >= 'A' && c <= 'Z') ||
		c == '_')
		return true;
	
	return false;
}

bool Lexer::is_delim()
{
	if (c == '(' || c == ')' || c == '{' || c == '}' || c == ',' || c == ';')
	    return true;

	return false;
}

bool Lexer::is_op()
{
	if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%' ||
		c == '=' || c == '>' || c == '<' || c == '!')
		return true;
	
	return false;
}

Token Lexer::GetToken()
{
	state = LEX_STATE::LEX_START;
	bool flag = true;

	Token token;

	while (flag)
	{
		c = fgetc(sourcefile);

		if (c == EOF)
		{
			token.tk = TOKEN::TOKEN_EOF;
			token.value = " ";
			break;
		}

		switch (state)
		{
			case LEX_STATE::LEX_START:
			{
				if (is_white())
					continue;
				else if (c == '+')
				{
					token.value.push_back(c);
					state = LEX_STATE::LEX_OP;
					flag = false;
				}
				else if (c == '-')
				{
					token.value.push_back(c);
					state = LEX_STATE::LEX_OP;
					flag = false;
				}
				else if (c == '*')
				{
					token.value.push_back(c);
					state = LEX_STATE::LEX_OP;
					flag = false;
				}
				else if (c == '/')
				{
					token.value.push_back(c);
					state = LEX_STATE::LEX_OP;
					flag = false;
				}
				else if (c == '%')
				{
					token.value.push_back(c);
					state = LEX_STATE::LEX_OP;
					flag = false;
				}
				else if (c == '=')
				{
					token.value.push_back(c);

					c = fgetc(sourcefile);

					if (c == '=')
						token.value.push_back(c);

					state = LEX_STATE::LEX_OP;
					flag = false;
				}
				else if (c == '>')
				{
					token.value.push_back(c);

					c = fgetc(sourcefile);

					if (c == '=')
						token.value.push_back(c);

					state = LEX_STATE::LEX_OP;
					flag = false;
				}
				else if (c == '<')
				{
					token.value.push_back(c);

					c = fgetc(sourcefile);

					if (c == '=')
						token.value.push_back(c);

					state = LEX_STATE::LEX_OP;
					flag = false;
				}
				else if (c == '!')
				{
					token.value.push_back(c);

					c = fgetc(sourcefile);

					if (c == '=')
						token.value.push_back(c);

					state = LEX_STATE::LEX_OP;
					flag = false;
				}
				else if (c == '"')
				{
					state = LEX_STATE::LEX_STRING;
				}
				else if (is_num())
				{
					token.value.push_back(c);
					state = LEX_STATE::LEX_NUMBER;
				}
				else if (is_ident())
				{
					token.value.push_back(c);
					state = LEX_STATE::LEX_IDENT;
				}
				else if (is_delim())
				{
					token.value.push_back(c);
					state = LEX_STATE::LEX_DELIM;
					flag = false;
				}
				break;
			}

			case LEX_STATE::LEX_NUMBER:
			{
				if (is_white() || is_delim() || is_op())
				{
					ungetc(c, sourcefile);
					flag = false;
				}
				else if (is_num())
					token.value.push_back(c);
				break;
			}

			case LEX_STATE::LEX_IDENT:
			{
				if (is_white() || is_delim() || is_op())
				{
					ungetc(c, sourcefile);
					flag = false;
				}
				else if (is_ident())
					token.value.push_back(c);
				break;
			}

			case LEX_STATE::LEX_STRING:
			{
				if (c == '"')
					flag = false;
				else if (c == '\\')
				{
					state = LEX_STATE::LEX_STRING_ESCAPE;
				}
				else
					token.value.push_back(c);

				break;
			}

			case LEX_STATE::LEX_STRING_ESCAPE:
			{
				if (c == '\'')
					token.value.push_back('\'');
				else if (c == '\"')
					token.value.push_back('\"');
				else if (c == '\\')
					token.value.push_back('\\');
				else if (c == 'r')
					token.value.push_back('\r');
				else if (c == 'n')
					token.value.push_back('\n');
				else if (c == 't')
					token.value.push_back('\t');
				else if (c == 's')
					token.value.push_back('\s');

				state = LEX_STATE::LEX_STRING;

				break;
			}

		    
			default:
			    break;
		}
	}

	switch (state)
	{
		case LEX_STATE::LEX_NUMBER:
		{
			token.tk = TOKEN::TOKEN_INT;
			break;
		}

		case LEX_STATE::LEX_STRING:
		{
			token.tk = TOKEN::TOKEN_STRING;
			break;
		}

		case LEX_STATE::LEX_IDENT:
		{
			token.tk = TOKEN::TOKEN_IDENT;

			if (token.value == "var")
				token.tk = TOKEN::KW_VAR;
			else if (token.value == "print")
				token.tk = TOKEN::KW_PRINT;
			else if (token.value == "if")
				token.tk = TOKEN::KW_IF;
			else if (token.value == "else")
				token.tk = TOKEN::KW_ELSE;
			else if (token.value == "while")
				token.tk = TOKEN::KW_WHILE;
			else if (token.value == "break")
				token.tk = TOKEN::KW_BREAK;
			else if (token.value == "function")
				token.tk = TOKEN::KW_FUNCTION;
			else if (token.value == "return")
				token.tk = TOKEN::KW_RETURN;
			else if (token.value == "true")
				token.tk = TOKEN::KW_TRUE;
			else if (token.value == "false")
				token.tk = TOKEN::KW_FALSE;
			break;
		}

		case LEX_STATE::LEX_DELIM:
		{
			if (token.value == "(")
				token.tk = TOKEN::TOKEN_OPEN_PA;
			else if (token.value == ")")
				token.tk = TOKEN::TOKEN_CLOSE_PA;
			else if (token.value == "{")
				token.tk = TOKEN::TOKEN_OPEN_BRACKET;
			else if (token.value == "}")
				token.tk = TOKEN::TOKEN_CLOSE_BRACKET;
			else if (token.value == ",")
				token.tk = TOKEN::TOKEN_COMMA;
			else if (token.value == ";")
				token.tk = TOKEN::TOKEN_SEMICOLON;
			break;
		}

		case LEX_STATE::LEX_OP:
		{
			if (token.value == "+")
				token.tk = TOKEN::TOKEN_ADD;
			else if (token.value == "-")
				token.tk = TOKEN::TOKEN_SUB;
			else if (token.value == "*")
				token.tk = TOKEN::TOKEN_MLU;
			else if (token.value == "/")
				token.tk = TOKEN::TOKEN_DIV;
			else if (token.value == "%")
				token.tk = TOKEN::TOKEN_MOD;
			else if (token.value == "<")
				token.tk = TOKEN::TOKEN_LT;
			else if (token.value == "<=")
				token.tk = TOKEN::TOKEN_LEQ;
			else if (token.value == ">")
				token.tk = TOKEN::TOKEN_GT;
			else if (token.value == ">=")
				token.tk = TOKEN::TOKEN_GEQ;
			else if (token.value == "=")
				token.tk = TOKEN::TK_ASSIGN;
			else if (token.value == "==")
				token.tk = TOKEN::TOKEN_EQ;
			else if (token.value == "!=")
				token.tk = TOKEN::TOKEN_NEQ;
			break;
		}
	}

	token_list.push(token);

	return token;
}