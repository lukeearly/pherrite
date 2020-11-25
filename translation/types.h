#ifndef __TYPES_H_
#define __TYPES_H_
#include "llvm/IR/Type.h"

namespace types {
	struct Type {
		virtual llvm::Type *llvm(llvm::LLVMContext *ctx) = 0;
	};

	Type *integral(unsigned int width, unsigned int isSigned);
}

#endif // __TYPES_H_
