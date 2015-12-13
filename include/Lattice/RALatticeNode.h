#ifndef RALatticeNode_H_
#define RALatticeNode_H_
#include <map>
#include <set>
#include <vector>
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/InstIterator.h"
#include "llvm/Support/raw_ostream.h"
#include "LatticeNode.h"
#include "llvm/Support/ConstantRange.h"
using namespace std;
using namespace llvm;

class RALatticeNode: public LatticeNode{
public:

	RALatticeNode():LatticeNode(RALATTICE), RangeMap(map<Value*, ConstantRange*>()){}
	RALatticeNode(bool isBottom, bool isTop):LatticeNode(RALATTICE, isTop, isBottom), RangeMap(map<Value*, ConstantRange*>()),counter(0){} 
	RALatticeNode(bool isBottom, bool isTop, std::map<Value*, ConstantRange*> RangeMap1):LatticeNode(RALATTICE, isTop, isBottom), RangeMap(RangeMap1),counter(0){}
 	RALatticeNode(RALatticeNode& raNode): LatticeNode(RALATTICE, raNode.isTop, raNode.isBottom), RangeMap(raNode.RangeMap),counter(0){}
 	RALatticeNode(RALatticeNode* raNodePtr): LatticeNode(RALATTICE, raNodePtr->isTop, raNodePtr->isBottom), RangeMap(raNodePtr->RangeMap),counter(0){}
 	
 	/*Virtual functions that need to be implemented*/
 	virtual LatticeNode* join(LatticeNode* raNode);
 	virtual bool equal(LatticeNode* raNode);
	virtual	void print();
	std::vector<Value *> differInRange(RALatticeNode* in);
	/*used for dyn_cast converstion*/
	static bool classof(const LatticeNode* nodePtr){
		return nodePtr->type == RALATTICE;
	}
	/*Class member variable*/
	map<Value*, ConstantRange*> RangeMap;
	int counter;
};

#endif


