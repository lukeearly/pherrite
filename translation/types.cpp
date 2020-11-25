#include "types.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/DerivedTypes.h"

namespace types {
	struct IntegralType: public Type {
		IntegralType(unsigned int width, unsigned int isSigned): width(width), isSigned(isSigned) {};

		llvm::Type *llvm(llvm::LLVMContext *ctx) {
			return llvm::IntegerType::get(*ctx, width);
		}
		unsigned int width;
		unsigned int isSigned;
	};

	Type *integral(unsigned int width, unsigned int isSigned) {
		return new IntegralType(width, isSigned);
	}
}
