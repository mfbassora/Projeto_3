#include "Liveness.h"

using namespace llvm;

void Liveness::addToMap(Function &F) {
      static int id = 1;
      for (inst_iterator i = inst_begin(F), E = inst_end(F); i != E; ++i, ++id)
        // Convert the iterator to a pointer, and insert the pair
        instMap.insert(std::make_pair(&*i, id));
    }
void Liveness::computeBBGenKill(Function &F, DenseMap<const BasicBlock*, genKill> &bbMap) 
    {
      for (Function::iterator b = F.begin(), e = F.end(); b != e; ++b) {
        genKill s;
        for (BasicBlock::iterator i = b->begin(), e = b->end(); i != e; ++i) {
          // The GEN set is the set of upwards-exposed uses:
          // pseudo-registers that are used in the block before being
          // defined. (Those will be the pseudo-registers that are defined
          // in other blocks, or are defined in the current block and used
          // in a phi function at the start of this block.) 
          unsigned n = i->getNumOperands();
          for (unsigned j = 0; j < n; j++) {
            Value *v = i->getOperand(j);
            if (isa<Instruction>(v)) {
              Instruction *op = cast<Instruction>(v);
              if (!s.kill.count(op))
                s.gen.insert(op);
            }
          }
          // For the KILL set, you can use the set of all instructions
          // that are in the block (which safely includes all of the
          // pseudo-registers assigned to in the block).
          s.kill.insert(&*i);
        }
        bbMap.insert(std::make_pair(&*b, s));
      }
    }

// Do this using a worklist algorithm (where the items in the worklist are basic blocks).
    void Liveness::computeBBBeforeAfter(Function &F, DenseMap<const BasicBlock*, genKill> &bbGKMap,
                              DenseMap<const BasicBlock*, beforeAfter> &bbBAMap)
    {
       
      
      std::queue<BasicBlock*> workList;
      workList.push_back(--F.end());
      int contador = 0;
      while (!workList.empty()) {
        //vendo tamanho do worklist
        ++contador;
        errs() << contador << "=" << workList.size() <<"\n";
        BasicBlock *b = workList.pop_back_val();
        //Pega a classe before_after referente ao bloco basico
        beforeAfter b_beforeAfter = bbBAMap.lookup(b);
        bool shouldAddPred = !bbBAMap.count(b);
        genKill b_genKill = bbGKMap.lookup(b);
        
        // Take the union of all successors
        std::set<const Instruction*> a;
        for (succ_iterator SI = succ_begin(b), E = succ_end(b); SI != E; ++SI) {
          std::set<const Instruction*> s(bbBAMap.lookup(*SI).before);
          a.insert(s.begin(), s.end());
        }
        //debug
        errs() << a << "\n" << b_beforeAfter.after << "\n";
        if (a != b_beforeAfter.after){
          shouldAddPred = true;
          b_beforeAfter.after = a;
          // before = after - KILL + GEN
          b_beforeAfter.before.clear();
          std::set_difference(a.begin(), a.end(), b_genKill.kill.begin(), b_genKill.kill.end(),
                              std::inserter(b_beforeAfter.before, b_beforeAfter.before.end()));
          b_beforeAfter.before.insert(b_genKill.gen.begin(), b_genKill.gen.end());
        }
        
        if (shouldAddPred)
          for (pred_iterator PI = pred_begin(b), E = pred_end(b); PI != E; ++PI)
            workList.push_back(*PI);
        
        bbBAMap.erase(b);
        bbBAMap.insert(std::make_pair(b,b_beforeAfter
      }
    }

void Liveness::computeIBeforeAfter(Function &F, DenseMap<const BasicBlock*, beforeAfter> &bbBAMap,
        DenseMap<const Instruction*, beforeAfter> &iBAMap) {
    for (Function::iterator b = F.begin(), e = F.end(); b != e; ++b) {
        BasicBlock::iterator i = --b->end();
        std::set<const Instruction*> liveAfter(bbBAMap.lookup(b).after);
        std::set<const Instruction*> liveBefore(liveAfter);

        while (true) {
            // before = after - KILL + GEN
            liveBefore.erase(i);

            unsigned n = i->getNumOperands();
            for (unsigned j = 0; j < n; j++) {
                Value *v = i->getOperand(j);
                if (isa<Instruction>(v))
                    liveBefore.insert(cast<Instruction>(v));
            }

            beforeAfter ba;
            ba.before = liveBefore;
            ba.after = liveAfter;
            iBAMap.insert(std::make_pair(&*i, ba));

            liveAfter = liveBefore;
            if (i == b->begin())
                break;
            --i;
        }
    }
}

bool Liveness::runOnFunction(Function &F) {
    // Iterate over the instructions in F, creating a map from instruction address to unique integer.
    addToMap(F);
    errs() << "Liveness na Funcao: " << F.getName() << "\n";
    bool changed = false;

    // LLVM Value classes already have use information. But for the sake of learning, we will implement the iterative algorithm.

//    DenseMap<const BasicBlock*, genKill> bbGKMap;
    // For each basic block in the function, compute the block's GEN and KILL sets.
    computeBBGenKill(F, rbbGKMap);

//    DenseMap<const BasicBlock*, beforeAfter> bbBAMap;
    // For each basic block in the function, compute the block's liveBefore and liveAfter sets.
    //computeBBBeforeAfter(F, bbGKMap, bbBAMap);

    DenseMap<const Instruction*, beforeAfter> iBAMap;
    computeIBeforeAfter(F, rbbBAMap, riBAMap);

    for (inst_iterator i = inst_begin(F), E = inst_end(F); i != E; ++i) {
        beforeAfter s = iBAMap.lookup(&*i);
        errs() << i->getName() << "\n";
        errs() << "%" << instMap.lookup(&*i) << ": { ";
        std::for_each(s.before.begin(), s.before.end(), print_elem);
        errs() << "} { ";
        std::for_each(s.after.begin(), s.after.end(), print_elem);
        errs() << "}\n";
    }



 
    return changed;
}

bool Liveness::test2(char * string) {
    errs() << string << "\n";
    return true;
}
char llvm::Liveness::ID = 0;
RegisterPass<Liveness> X("Liveness", "Liveness analysis", false, false);

