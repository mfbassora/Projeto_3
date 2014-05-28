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
  struct deadLoad : public FunctionPass {
    static char ID;
    deadLoad() : FunctionPass(ID) {}

    virtual bool runOnFunction(Function &F) {
        BasicBlock::iterator  last_inst=0;
        bool ret = false;
        LoadInst* loadRemove;
        int remove =0;
     for (Function::iterator i = F.begin(), e = F.end(); i != e; ++i)
     {
         
         //Para cada BasicBLock vamos olhar seu anterior para comparar
         for (BasicBlock::iterator i2 = i->begin(), e2 = i->end(); i2 != e2; )
         {
            
             if (LoadInst* loadInst = dyn_cast<LoadInst>(&*i2)) {
                //Caso a instrucao seja de load, vamos verificar se a anterior e uma instrucao de store
                  if (StoreInst* storeInst = dyn_cast<StoreInst>(&*last_inst)) 
                  {
                      
//                      errs() << "Load na variavel"<< loadInst->getOperand(0)<<" \n";
//                      errs() << "Store na variavel"<< storeInst->getOperand(1)<<" \n";
                      if(loadInst->getOperand(0)->getName()==storeInst->getOperand(1)->getName())
                      {
                          //Temos aqui um Load a ser tirado do codigo
                          errs() << "Load inutil na variavel : "<< loadInst->getOperand(0)->getName() <<" \n";
                          //Vamos dar um replace no registrador da funcao load
                         loadInst->replaceAllUsesWith(storeInst->getValueOperand());
                          //Removemos entao o load
                          //errs() << "Deletando a funcao de load  \n ";
                          loadRemove=loadInst;
                         //Vai entrar na parte da remocao
                          remove =1;
                          ret=true;
                          
                      }
             
                  };
             
//                   errs() << last_inst->getName() << "\n";
//                 errs() << "Temos uma funcao de load" << callInst->getOperand(0)->getName() << "\n";
//                
          }
             last_inst = i2;
             i2++;
             //Removendo
           if(remove ==1)
             {
               errs() << "Deletando a funcao de load " << loadRemove->getOperand(0)->getName()<< " \n ";
               loadRemove->eraseFromParent();
                 remove =0;
                 
             }
             
            
             
        }
        }
            
       
          
      return ret;
    }
  };
}

char deadLoad::ID = 0;
static RegisterPass<deadLoad> X("deadLoad", "deadLoad World Pass", false, false);
