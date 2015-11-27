#ifndef CSELatticeNode_H_
#define CSELatticeNode_H_
#include <map>
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/InstIterator.h"
#include "llvm/Support/raw_ostream.h"
#include "LatticeNode.h"
using namespace std;
using namespace llvm;

class CSELatticeNode: public LatticeNode{
public:
 	map<Value*, Instruction*> statements;
	CSELatticeNode():LatticeNode(CSELATTICE), statements(map<Value*, Instruction*>()){}
	CSELatticeNode(bool isBottom, bool isTop):LatticeNode(CSELATTICE, isTop, isBottom), statements(map<Value*, Instruction*>()){errs() << "CSELatticeNode Initialized\n";} 
	CSELatticeNode(bool isBottom, bool isTop, std::map<Value*, Instruction*> statements1):LatticeNode(CSELATTICE, isTop, isBottom), statements(statements1){}
 	CSELatticeNode(CSELatticeNode& cseNode): LatticeNode(CSELATTICE, cseNode.isTop, cseNode.isBottom), statements(cseNode.statements){}
 	CSELatticeNode(CSELatticeNode* cseNodePtr): LatticeNode(CSELATTICE, cseNodePtr->isTop, cseNodePtr->isBottom), statements(cseNodePtr->statements){}
 	
 	/*Virtual functions that need to be implemented*/
 	virtual LatticeNode* join(LatticeNode* cseNode);
 	virtual bool equal(LatticeNode* cseNode);
 	bool equal(CSELatticeNode* cseNode);
	/*used for dyn_cast converstion*/
	static bool classof(const LatticeNode* nodePtr){
		return nodePtr->type == CSELATTICE;
	}
};

#endif


