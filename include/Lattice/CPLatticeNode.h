#ifndef CPLatticeNode_H_
#define CPLatticeNode_H_
#include <map>

#include "llvm/IR/Constants.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/InstIterator.h"
#include "llvm/Support/raw_ostream.h"

#include "LatticeNode.h"

using namespace std;
using namespace llvm;






class CPLatticeNode: public LatticeNode{
public:

	//CPLatticeNode():LatticeNode(CPLATTICE), statements(map<Value*, ConstantInt*>()), tmp_info(map<Value*, ConstantInt*>()), invalid_tmp(map<Value*, int>()){}

	CPLatticeNode():LatticeNode(CPLATTICE), data_info(map<Value*, int>()){}

	//CPLatticeNode(bool isBottom, bool isTop):LatticeNode(CPLATTICE, isTop, isBottom), statements(map<Value*, ConstantInt*>()), tmp_info(map<Value*, ConstantInt*>()), invalid_tmp(map<Value*, int>()){}

	CPLatticeNode(bool isBottom, bool isTop):LatticeNode(CPLATTICE, isTop, isBottom), data_info(map<Value*, int>()){}

  //CPLatticeNode(bool isBottom, bool isTop, std::map<Value*, ConstantInt*> statements1):LatticeNode(CPLATTICE, isTop, isBottom), statements(statements1), tmp_info(map<Value*, ConstantInt*>()), invalid_tmp(map<Value*, int>()){}

	CPLatticeNode(bool isBottom, bool isTop, std::map<Value*, int> data_info1):LatticeNode(CPLATTICE, isTop, isBottom), data_info(data_info1) {}

	//CPLatticeNode(CPLatticeNode& cpNode): LatticeNode(CPLATTICE, cpNode.isTop, cpNode.isBottom), statements(cpNode.statements), tmp_info(cpNode.tmp_info), invalid_tmp(map<Value*, int>()){}

  CPLatticeNode(CPLatticeNode& cpNode): LatticeNode(CPLATTICE, cpNode.isTop, cpNode.isBottom), data_info(cpNode.data_info) {}

	//CPLatticeNode(CPLatticeNode* cpNodePtr): LatticeNode(CPLATTICE, cpNodePtr->isTop, cpNodePtr->isBottom), statements(cpNodePtr->statements), tmp_info(cpNodePtr->tmp_info), invalid_tmp(map<Value*, int>()){}

	CPLatticeNode(CPLatticeNode* cpNodePtr): LatticeNode(CPLATTICE, cpNodePtr->isTop, cpNodePtr->isBottom), data_info(cpNodePtr->data_info){}


 	/*Virtual functions that need to be implemented*/
 	virtual LatticeNode* join(LatticeNode* cpNode);
 	virtual bool equal(LatticeNode* cpNode);
	virtual	void print();
	/*used for dyn_cast converstion*/
	static bool classof(const LatticeNode* nodePtr){
		return nodePtr->type == CPLATTICE;
	}
	/*Class member variable*/
	//map<Value*, ConstantInt*> statements;
	//map<Value*, ConstantInt*> tmp_info;
	//map<Value*, int> invalid_tmp;
	map<Value*, int> data_info;


};

#endif
