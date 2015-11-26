#ifndef FLOWFUNCTION_H_
#define FLOWFUNCTION_H_

#include "llvm/IR/Instruction.h"
#include "llvm/InstVisitor.h"
#include "../../include/Lattice/LatticeNode.h"
#include "../../include/Lattice/CSELatticeNode.h"
#include <vector>

using namespace std;
using namespace llvm;

enum FlowFunctionType{
	CPFLOW,
	CSEFLOW,
	RAFLOW,
	PAFLOW
};

class FlowFunction{
public:
	FlowFunction(FlowFunctionType t):type(t){}
	virtual LatticeNode* PassFunction(Instruction *inst, vector<LatticeNode*> input){return NULL;}
/*The only class member variable*/
	FlowFunctionType type;


};
#endif
