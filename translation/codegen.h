#ifndef __CODEGEN_H_
#define __CODEGEN_H_
#include <string>
#include "PherriteParser.h"

void printIR(pherrite::PherriteParser *parser);
void printIRFromString(std::string input);

#endif // __CODEGEN_H_
