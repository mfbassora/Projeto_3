#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/InstIterator.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace {
  struct deadCodeElim : public FunctionPass {
    static char ID;
    deadCodeElim() : FunctionPass(ID) {}

    virtual bool runOnFunction(Function &F) {
        BasicBlock::iterator  last_inst=0;
        bool ret = false;
        LoadInst* loadRemove;
        int remove =0;
     for (Function::iterator i = F.begin(), e = F.end(); i != e; ++i)
     {
         //Liveness
         
         //Para cada BasicBLock vamos olhar seu anterior para comparar
         for (BasicBlock::iterator i2 = i->begin(), e2 = i->end(); i2 != e2; )
         {
            
            
             
          }
             
        }
        
            
       
          
      return true;
    }
  };
  bool check_Inst(Instruction * intruction)
  {
      bool situation = true;
      //Verificando condições
      
      return situation;
  }
}

char deadCodeElim::ID = 0;
static RegisterPass<deadCodeElim> X("deadCodeElim", "deadCodeElim World Pass", false, false);
