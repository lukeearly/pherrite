#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include "antlr4-runtime.h"
#include "PherriteParser.h"
#include "PherriteParserVisitor.h"
#include "PherriteLexer.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/ADT/APInt.h"
#include "codegen.h"
#include "types.h"

using namespace llvm;
using namespace pherrite;

#define DEF_VISIT(NodeType, node) antlrcpp::Any visit##NodeType(PherriteParser::NodeType##Context *node) override

void translationError(std::string e) {
	std::cerr << "ERROR: " << e << std::endl;
	exit(1);
}

struct ValueEntry {
	Value *value;
	types::Type *type;
};

struct CodeGenVisitor: public PherriteParserVisitor {
	std::unique_ptr<LLVMContext> ctx = std::make_unique<LLVMContext>();
	std::unique_ptr<Module> curModule = std::make_unique<Module>("default pherrite module", *ctx);
	std::unique_ptr<IRBuilder<>> builder = std::make_unique<IRBuilder<>>(*ctx);
	std::map<std::string, ValueEntry> localFunctionScope;
	// TODO store static type in symbol value - custom higher level Type
	// TODO make separate visitors FunctionBuilder, ModuleBuilder, TypeBuilder, to use recursively

	Value *retval;
	Type *retty;
	Function *retfn;

	DEF_VISIT(TopLevel, node) {
		return visitChildren(node), 0;
	}
	DEF_VISIT(Definition, node) {
		return visitChildren(node), 0;
	}

	DEF_VISIT(FunDecl, node) {
		return visit(node->proto), 0;
	}

	DEF_VISIT(FunProto, node) {
		Type *type;

		if (node->retType)
			visit(node->retType), type = retty;
		else
			type = Type::getVoidTy(*ctx);

		retfn = Function::Create(
			FunctionType::get(type, std::vector<Type *>(0), false),
			Function::ExternalLinkage,
			StringRef(node->name->getText()),
			curModule.get()
		);
		return 0;
	}

	DEF_VISIT(FunDef, node) {
		Function *f = curModule->getFunction(node->proto->name->getText());

		if (!f)
			visit(node->proto), f = retfn;

		if (!f)
			translationError("Error generating prototype " + node->proto->name->getText());

		if (!f->empty())
			translationError("Function " + node->proto->name->getText() + " cannot be redefined");

		BasicBlock *bb = BasicBlock::Create(*ctx, "entry", f);
		builder->SetInsertPoint(bb);

		if (node->body) {
			visit(node->body);
			verifyFunction(*f);
			return 0;
		}

		f->eraseFromParent();
		return 0;
	}

	DEF_VISIT(ParamList, node) {
		return 0;
	}

	DEF_VISIT(Block, node) {
		return visitChildren(node), 0;
	}

	DEF_VISIT(ReturnValue, node) {
		visit(node->expr());
		Value *ret = retval;
		if (!ret)
			return nullptr;
		return builder->CreateRet(ret), 0;
	}

	DEF_VISIT(DebugValue, node) {
		return visitChildren(node), 0; // TODO
	}

	DEF_VISIT(LetState, node) {
		visit(node->value);
		localFunctionScope[std::string(node->name->getText())] = {.value = retval};
		return 0;
	}

	//const std::array<Constant *, 4> cs = {
		//ConstantInt::get(Type::getInt8Ty(*ctx), (uint64_t) '%', false),
		//ConstantInt::get(Type::getInt8Ty(*ctx), (uint64_t) 's', false),
		//ConstantInt::get(Type::getInt8Ty(*ctx), (uint64_t) '\n', false),
		//ConstantInt::get(Type::getInt8Ty(*ctx), (uint64_t) '\0', false)
	//};
//
	//Value *format = ConstantArray::get(
		//ArrayType::get(Type::getInt8Ty(*ctx), 4),
		//ArrayRef<Constant *>(&cs)
	//);
//
	//DEF_VISIT(DebugStatement, node) {
		//Function *callee = curModule->getFunction("printf");
		//if (!callee)
			//return nullptr;
//
		//Value *num;
//
		//if (node->expr()) {
			//num = (Value *) visit(node->expr());
			//if (!num)
				//return nullptr;
			//return builder->CreateRet(num);
		//} else {
			//num = ConstantInt::get(Type::getInt32Ty(*ctx), 0);
		//}
//
		//return builder->CreateCall(callee, (std::vector<Value *>){format, num}, "debugtmp");
	//}

	DEF_VISIT(NumLitExpr, node) {
		antlr4::tree::TerminalNode *sym;
		uint32_t val;
		if (sym = node->ZERO()) {
			val = 0;
		} else if (sym = node->DEC_LIT()) {
			val = std::stoi(sym->getText(), 0, 10);
		} else if (sym = node->OCT_LIT()) {
			val = std::stoi(sym->getText().substr(1), 0, 010);
		} else if (sym = node->HEX_LIT()) {
			val = std::stoi(sym->getText().substr(2), 0, 0x10);
		} else if (sym = node->BIN_LIT()) {
			val = std::stoi(sym->getText().substr(2), 0, 2); // no standard binary literal :(
		} else {
			return nullptr;
		}

		return retval = ConstantInt::get(Type::getInt32Ty(*ctx), val), 0;
	}

	DEF_VISIT(CallExpr, node) {
		Function *f = curModule->getFunction(node->callee->getText());
		if (!f)
			translationError("Function " + node->callee->getText() + " not found");
		return retval = builder->CreateCall(f, (std::vector<Value *>){}, "calltmp"), 0;
	}

	DEF_VISIT(IdExpr, node) {
		Value *v = localFunctionScope[node->ID()->getText()].value;
		if (!v)
			translationError("Value " + node->ID()->getText() + " not found");
		return retval = v, 0;
	}

	DEF_VISIT(TypeExpr, node) {
		return visitChildren(node), 0;
	}

	DEF_VISIT(BuiltinType, node) {
		antlr4::tree::TerminalNode *sym;
		Type *t;
		if (sym = node->INT()) {
			t = IntegerType::get(*ctx, 32);
		} else if (sym = node->INTN()) {
			t = IntegerType::get(*ctx, std::stoi(sym->getText().substr(3), 0, 10));
		}
		retty = t;
		return 0;
	}
};

void printIRFromString(std::string str) {
	antlr4::ANTLRInputStream input(str);
	PherriteLexer lexer(&input);
	antlr4::CommonTokenStream tokens(&lexer);
	PherriteParser parser(&tokens);
	printIR(&parser);
}

void printIR(PherriteParser *parser) {
	CodeGenVisitor cg;
	cg.visit(parser->topLevel());
	cg.curModule->print(outs(), nullptr);
}

