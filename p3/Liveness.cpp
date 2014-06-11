
#define DEBUG_TYPE "printCode"
#include "llvm/Pass.h"
#include "llvm/ADT/StringExtras.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/InstIterator.h"
#include "llvm/ADT/DenseMap.h"
#include <set>
#include "llvm/ADT/SmallVector.h"
#include "llvm/Support/CFG.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/User.h"
using namespace llvm;

    	void Liveness::addToMap(Function &F) {
      		static int id = 1;
      		static int basicBlock_id = 1;

      		for (inst_iterator i = inst_begin(F), E = inst_end(F); i != E; ++i, ++id)
        		// Convert the iterator to a pointer, and insert the pair
        		instMap.insert(std::make_pair(&*i, id));
		
			for (Function::iterator b = F.begin(), e = F.end(); b != e; ++b, ++basicBlock_id) 
	        	basicBlockMap.insert(std::make_pair(&*b, basicBlock_id));
    	}

    	void Liveness::computeBBGenKill(Function &F, DenseMap<const BasicBlock*, genKill> &bbMap) {
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
    	void Liveness::computeBBBeforeAfter(Function &F, DenseMap<const BasicBlock*, genKill> &bbGKMap, DenseMap<const BasicBlock*, beforeAfter> &bbBAMap) {
    		bool hasChanged = true;

    		while (hasChanged) {
    			hasChanged = false;

    			for (Function::iterator b = F.begin(), e = F.end(); b != e; ++b) {
    				genKill bb_genKill = bbGKMap.lookup(&*b);
    				beforeAfter bbBeforeAfter = bbBAMap.lookup(&*b);

    				// Take the union of all successors
       		        std::set<const Instruction*> a;
        			for (succ_iterator SI = succ_begin(b), E = succ_end(b); SI != E; ++SI) {
          				std::set<const Instruction*> s(bbBAMap.lookup(*SI).before);
				        a.insert(s.begin(), s.end());
        			}

        			if (a != bbBeforeAfter.after) hasChanged = true;

        			// Limpa o after do bloco básico e seta como as instruções sucessoras do bloco analisado.
        			bbBeforeAfter.after.clear();
        			bbBeforeAfter.after = a;
					
					// Cria e limpa o before do bloco básico e seta como 
        			std::set<const Instruction*> beforeOld;
        			beforeOld = bbBeforeAfter.before;
        			bbBeforeAfter.before.clear();
        			
        			std::set_difference(a.begin(), a.end(), bb_genKill.kill.begin(), bb_genKill.kill.end(), std::inserter(bbBeforeAfter.before, bbBeforeAfter.before.end()));
			    	bbBeforeAfter.before.insert(bb_genKill.gen.begin(), bb_genKill.gen.end());

			    	// Vê se o before mudou
			    	if (bbBeforeAfter.before != beforeOld) hasChanged = true;

			    	bbBAMap.erase(b);
			    	bbBAMap.insert(std::make_pair(b, bbBeforeAfter));
    			}
    		}
    }
    
    void Liveness::computeIBeforeAfter(Function &F, DenseMap<const BasicBlock*, beforeAfter> &bbBAMap, DenseMap<const Instruction*, beforeAfter> &iBAMap) {
    	for (Function::iterator b = F.begin(), e = F.end(); b != e; ++b) {
        	BasicBlock::iterator i = --b->end();
        	std::set<const Instruction*> liveAfter(bbBAMap.lookup(&*b).after);
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
    
  public:
    static char ID; // Pass identification, replacement for typeid
    printCode() : FunctionPass(ID) {}

    //**********************************************************************
    // runOnFunction
    //**********************************************************************
    virtual bool Liveness::runOnFunction(Function &F) {
      // Iterate over the instructions in F, creating a map from instruction address to unique integer.
      addToMap(F);

      bool changed = false;

      // LLVM Value classes already have use information. But for the sake of learning, we will implement the iterative algorithm.
      
      DenseMap<const BasicBlock*, genKill> bbGKMap;
      // For each basic block in the function, compute the block's GEN and KILL sets.
      computeBBGenKill(F, bbGKMap);

      DenseMap<const BasicBlock*, beforeAfter> bbBAMap;
      // For each basic block in the function, compute the block's liveBefore and liveAfter sets.
      computeBBBeforeAfter(F, bbGKMap, bbBAMap);

      DenseMap<const Instruction*, beforeAfter> iBAMap;
      computeIBeforeAfter(F, bbBAMap, iBAMap);

//      for (inst_iterator i = inst_begin(F), E = inst_end(F); i != E; ++i) {
//        beforeAfter s = iBAMap.lookup(&*i);
//        errs() << "%" << instMap.lookup(&*i) << ": { ";
//        std::for_each(s.before.begin(), s.before.end(), print_elem);
//        errs() << "} { ";
//        std::for_each(s.after.begin(), s.after.end(), print_elem);
//        errs() << "} { ";
//        errs() << i->getName() << " }\n";
//      }




      return changed;
    }

    //**********************************************************************
    // print (do not change this method)
    //
    // If this pass is run with -f -analyze, this method will be called
    // after each call to runOnFunction.
    //**********************************************************************
    virtual void print(llvm::raw_ostream &O, const Module *M) const {
        O << "This is printCode.\n";
    }

    //**********************************************************************
    // getAnalysisUsage
    //**********************************************************************


  char llvm::Liveness::ID = 0;

  // register the printCode class: 
  //  - give it a command-line argument
  //  - a name
  //  - a flag saying that we don't modify the CFG
  //  - a flag saying this is not an analysis pass
  RegisterPass<Liveness> X("Liveness", "Liveness analysis", false, true);

