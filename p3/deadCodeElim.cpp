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
#include "llvm/IR/IntrinsicInst.h"

using namespace llvm;

namespace {
  struct deadCodeElim : public FunctionPass {
    static char ID;
    bool check_Inst(Instruction * instruction);
    deadCodeElim() : FunctionPass(ID) {}

    virtual bool runOnFunction(Function &F) {
       GenKill &GK = getAnalysis<GenKill>();
       errs() << GK.name;
        //Liveness
    for (Function::iterator i = F.begin(), e = F.end(); i != e; ++i)
     {

         //Liveness
         
         //Para cada BasicBLock vamos olhar seu anterior para comparar
         for (BasicBlock::iterator i2 = i->begin(), e2 = i->end(); i2 != e2; ++i2)
         {
             bool teste;
             teste = check_Inst(i2);
             errs()<<teste<<"\n";
          }
             
        }
        
            errs() << "Basic block (name=" << i->getName() << ") has "
             << i->size() << " instructions.\n";
        
        }
            
       
          
      return true;
    }
    virtual void getAnalysisUsage(AnalysisUsage &AU) const {
   AU.addRequired<GenKill>();
   }
  };

  bool deadCodeElim :: check_Inst(Instruction * instruction)
  {
      bool situation = true;
      //Verificando condicoes
      if(instruction->mayHaveSideEffects()){
          situation = false;
          errs()<<"entrou side effect \n";
      }
      if(llvm::TerminatorInst::classof(instruction)){
          situation = false;
          errs()<<"entrou terminator \n";
      }
      if(llvm::LandingPadInst::classof(instruction)){
          situation = false;
          errs()<<"entrou landingpad \n";
      }
      if(llvm::DbgInfoIntrinsic::classof(instruction)){
          situation = false;
          errs()<<"entrou dbginfo \n";
      }
      return situation;
  }
}

char deadCodeElim::ID = 0;
static RegisterPass<deadCodeElim> X("deadCodeElim", "deadCodeElim World Pass", false, false);
