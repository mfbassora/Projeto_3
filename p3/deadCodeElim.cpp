#include "GenKill.h"

#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/InstIterator.h"
#include "llvm/Analysis/ConstantFolding.h"
#include "llvm/ADT/BitVector.h"
#include "llvm/ADT/ValueMap.h"
#include "llvm/Support/CFG.h"
#include "llvm/ADT/SmallVector.h"
using namespace llvm;

namespace {
  struct deadCodeElim : public FunctionPass {
    static char ID;
    deadCodeElim() : FunctionPass(ID) {}

    virtual bool runOnFunction(Function &F) {
       GenKill &GK = getAnalysis<GenKill>();
       errs() << GK.name;
        //Liveness
    for (Function::iterator i = F.begin(), e = F.end(); i != e; ++i)
     {
        
            errs() << "Basic block (name=" << i->getName() << ") has "
             << i->size() << " instructions.\n";
        
        }
            
       
          
      return true;
    }
    virtual void getAnalysisUsage(AnalysisUsage &AU) const {
   AU.addRequired<GenKill>();
   }
  };
 

}

char deadCodeElim::ID = 0;
static RegisterPass<deadCodeElim> X("deadCodeElim", "deadCodeElim World Pass", false, false);
