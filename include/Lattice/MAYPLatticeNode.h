/*****************************/
/*
This is the may point to analysis
*/
#ifndef POINTERLATTICENODE_H_
#define POINTERLATTICENODE_H_
#include <set>
#include <map>
#include "llvm/IR/Instructions.h"
#include "llvm/Support/InstIterator.h"
#include "llvm/Support/raw_ostream.h"
#include "LatticeNode.h"
using namespace llvm;
using namespace std;
class MAYPLatticeNode: public LatticeNode{
public:
	MAYPLatticeNode():LatticeNode(MAYPLATTICE), point_to_all(set<Value*>()), point_to_info(map<Value*, set<Value*> >()){}
	MAYPLatticeNode(bool isBottom, bool isTop):LatticeNode(MAYPLATTICE, isTop, isBottom), point_to_all(set<Value*>()), point_to_info(map<Value*, set<Value*> >()){}
	MAYPLatticeNode(bool isBottom, bool isTop, set<Value*> ptAll, map<Value*, set<Value*> > ptInfo):LatticeNode(MAYPLATTICE, isTop, isBottom), point_to_all(ptAll), point_to_info(ptInfo){}
	
	/*virtual functions inherited from LatticeNode to be implemented*/
	virtual LatticeNode* join(LatticeNode* paNode);
	virtual bool equal(LatticeNode* paNode);
	virtual void print();
	/*used for dyn_cast conversion*/
	static bool classof(const LatticeNode* nodePtr){
		return nodePtr->type == MAYPLATTICE;
	}

	/*Override virtual functions of */
	set<Value*> point_to_all;
	map<Value*, set<Value*> > point_to_info;
};
#endif
