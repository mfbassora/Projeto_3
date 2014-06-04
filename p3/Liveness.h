#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/InstIterator.h"
#include "llvm/ADT/StringExtras.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/IR/User.h"
#include <set>
#include "llvm/ADT/SmallVector.h"
#include "llvm/Support/CFG.h"

using namespace llvm;

namespace { 
 
    DenseMap<const Instruction*, int> instMap;

  void print_elem(const Instruction* i) {
    errs() << i->getName() << " ";
     
  };
    
  class genKill {
  public:
    std::set<const Instruction*> gen;
    std::set<const Instruction*> kill;
  };
  
  class beforeAfter {
  public:
    std::set<const Instruction*> before;
    std::set<const Instruction*> after;
  };
     
}


namespace llvm {
struct Liveness : public FunctionPass {
private:
void addToMap(Function &F);
void computeBBGenKill(Function &F, DenseMap<const BasicBlock*, genKill> &bbMap);
void computeBBBeforeAfter(Function &F, DenseMap<const BasicBlock*, genKill> &bbGKMap,DenseMap<const BasicBlock*, beforeAfter> &bbBAMap);
void computeIBeforeAfter(Function &F, DenseMap<const BasicBlock*, beforeAfter> &bbBAMap,DenseMap<const Instruction*, beforeAfter> &iBAMap);

public:
static char ID;
char * name;
DenseMap<const Instruction*, beforeAfter> riBAMap;
DenseMap<const BasicBlock*, genKill> rbbGKMap;
DenseMap<const BasicBlock*, beforeAfter> rbbBAMap;
Liveness() : FunctionPass(ID) {}
virtual bool runOnFunction(Function &F); // implementação no Liveness.cpp
bool test2(char * string);
 };
}
