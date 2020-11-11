#include <iostream>
#include <fstream>
#include "antlr4-runtime.h"
#include "PherriteParser.h"
#include "PherriteLexer.h"

using namespace std;
using namespace pherrite;
using namespace antlr4;

int main(int argc, char *argv[]) {
	ifstream stream;
	stream.open("samples/return0.ph");

	ANTLRInputStream input(stream);
	PherriteLexer lexer(&input);
	CommonTokenStream tokens(&lexer);

	tokens.fill();
	for (auto tok : tokens.getTokens()) {
		cout << tok->toString() << endl;
	}

	PherriteParser parser(&tokens);
	tree::ParseTree *tree = parser.topLevel();

	cout << tree->toStringTree(&parser) << endl << endl;

	return 0;
}
