
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
#include "Liveness.h"
using namespace llvm;

namespace {
  struct deadCodeElim : public FunctionPass {
    static char ID;
    bool check_Inst(Instruction * instruction);
    deadCodeElim() : FunctionPass(ID) {}

    virtual bool runOnFunction(Function &F) {  
        int count=0;
        Liveness &Live = getAnalysis<Liveness>();
        //Liveness
    for (Function::iterator i = F.begin(), e = F.end(); i != e; ++i)
     {
         
         for (BasicBlock::iterator i2 = i->begin(), e2 = i->end(); i2 != e2; ++i2)
         {
             //Para cada instrução vamos verificar ela tem def
             Instruction * i = i2;
             if(i2->getName() != ""){
             //É uma definição de variável
             beforeAfter s = Live.riBAMap.lookup(i2);
             if(!s.after.count(i2) && check_Inst(i2)){
                 count++;
             errs() << "Def: " << i2->getName() << "\n";
             errs() << "In: ";
             std::for_each(s.before.begin(), s.before.end(), print_elem);
             errs() << "\n";
              errs() << "Out: ";              
             std::for_each(s.after.begin(), s.after.end(), print_elem);
             errs() << "\n\n";
             errs() << i2->getName() << "\n";
             };
             };
           
         }
         
        }
            
       
         errs() << count << "\n";
      return true;
    }
    virtual void getAnalysisUsage(AnalysisUsage &AU) const {
   AU.addRequired<Liveness>();
   }
  };

  bool deadCodeElim :: check_Inst(Instruction * instruction)
  {
      bool situation = true;
      //Verificando condicoes
      if(instruction->mayHaveSideEffects()){
          situation = false;
          
      }
      if(llvm::TerminatorInst::classof(instruction)){
          situation = false;
          
      }
      if(llvm::LandingPadInst::classof(instruction)){
          situation = false;
         
      }
      if(llvm::DbgInfoIntrinsic::classof(instruction)){
          situation = false;
         
      }
      return situation;
  }
}

char deadCodeElim::ID = 0;
static RegisterPass<deadCodeElim> X("deadCodeElim", "deadCodeElim World Pass", false, false);
