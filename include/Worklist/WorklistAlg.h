#ifndef WORKLIST_H_
#define WORKLIST_H_
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/BasicBlock.h"
/*********************/
#include "../FlowFunction/CSEFlowFunction.h"
/*********************/
#include <map>
#include <queue>
#include <vector>
#include <iterator>
using namespace std;
using namespace llvm;
class WorklistAlg{
public:
	WorklistAlg(){}
	map<Instruction*, LatticeNode*> Run_Worklist(Function &F, FlowFunction* flowFunc, LatticeNode* beginNode);
	/*class variables*/
	map<Instruction*, LatticeNode*> output_map;
	/*This is used to find the predecessor of the Instruction node*/
	map<Instruction*, vector<Instruction*> > predecessor;
	map<Instruction*, vector<Instruction*> > successor;
	map<Instruction*, LatticeNode*> finalMap;
	queue<Instruction*> worklist;
private:
	void init(Function &F, FlowFunction* flowFunc, LatticeNode* beginNode);
	bool matchFlowFunc(Instruction* inst, FlowFunction* flowFunc);
	LatticeNode* merge(vector<LatticeNode*> input);
};


#endif
