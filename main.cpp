#include"Bjs.h"
#include"lex.h"
#include"parser.h"

FILE *sourcefile;

int main()
{
	fopen_s(&sourcefile, "sc.txt",  "r");

	Parser parser;

	parser.parser();

	fclose(sourcefile);

	return 0;
}