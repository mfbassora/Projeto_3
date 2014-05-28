#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/InstIterator.h"
#include "llvm/Support/raw_ostream.h"
namespace { 
    
}


namespace llvm {
struct GenKill : public FunctionPass {
private:
//void compute1(...); // implementação no KillGen.cpp
public:
static char ID;
char * name;
GenKill() : FunctionPass(ID) {}
virtual bool runOnFunction(Function &F); // implementação no KillGen.cpp
bool test2(char * string);
 };
}
