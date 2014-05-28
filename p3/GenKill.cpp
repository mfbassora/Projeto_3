#include "GenKill.h"
using namespace llvm;

bool GenKill::runOnFunction(Function &F)
{
    this->name="teste_ASDasdasd \n\n\n\n";
    return true;
}
bool GenKill::test2(char * string)
{
    errs() << string << "\n";
}
char llvm::GenKill::ID = 0;
RegisterPass<GenKill> X("genkill", "Live vars analysis", false, false);

