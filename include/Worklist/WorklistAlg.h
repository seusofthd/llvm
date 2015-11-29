#ifndef WORKLISTALG_H_
#define WORKLISTALG_H_
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/Support/raw_ostream.h"
/*********************/
#include "../FlowFunction/CSEFlowFunction.h"
/*********************/
#include <map>
#include <queue>
#include <vector>
#include <iterator>
#include <stdio.h>
using namespace std;
using namespace llvm;
class WorklistAlg{
public:
	WorklistAlg(){errs()<< "worklist algorithm constructed\n";}
	map<Instruction*, LatticeNode*> Run_Worklist(Function &F, FlowFunction* flowFunc, LatticeNode* beginNode);
	void test();
	/*class variables*/
private:
	map<Instruction*, LatticeNode*> output_map;
	/*This is used to find the predecessor of the Instruction node*/
	map<Instruction*, vector<Instruction*> > predecessor;
	map<Instruction*, vector<Instruction*> > successor;
	map<Instruction*, LatticeNode*> finalMap;
	queue<Instruction*> worklist;
	void init(Function &F, FlowFunction* flowFunc, LatticeNode* beginNode);
	bool matchFlowFunc(Instruction* inst, FlowFunction* flowFunc, LatticeNode* beginNode);
	LatticeNode* merge(vector<LatticeNode*> input);
};


/*Member Function Implementation*/

#endif
