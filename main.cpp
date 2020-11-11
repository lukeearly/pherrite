#include <iostream>
#include <fstream>
#include <string>
#include "antlr4-runtime.h"
#include "PherriteParser.h"
#include "PherriteLexer.h"

using namespace std;
using namespace pherrite;
using namespace antlr4;

int parseTest(int argc, char *argv[]) {
	if (argc < 3) return 1;

	ifstream source(argv[2]);

	ANTLRInputStream input(source);
	PherriteLexer lexer(&input);
	CommonTokenStream tokens(&lexer);
	PherriteParser parser(&tokens);
	tree::ParseTree *tree;

	if (argc > 3 && string(argv[3]) == "expr")
		tree = parser.expr();
	else
		tree = parser.topLevel();

	cout << tree->toStringTree(&parser) << endl;

	return 0;
}

int main(int argc, char *argv[]) {
	if (argc >= 2 && string(argv[1]) == "parse")
		return parseTest(argc, argv);
	return 0;
}
