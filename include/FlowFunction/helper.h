#ifndef HELPER_H_
#define HELPER_H_


// IR stuff.
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Constants.h"

// Support stuff.
#include "llvm/Support/InstIterator.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/ConstantRange.h"


// Standard library stuff
#include <utility>
#include <set>
#include <map>

using namespace llvm;

class helper {
public:
  // Returns the operands of a binary operator.
  static std::pair<Use*, Use *> getOperands(BinaryOperator &BO){
    Use* S1;
    Use* S2;
    int i = 0;
    for (User::op_iterator OP = BO.op_begin(), OPE = BO.op_end(); OP != OPE; ++OP){
      if(i == 0){
        S1 = &*OP;
      }
      else{
        S2 = &*OP;
      }
      i++;
    }
    return std::make_pair(S1, S2);
  }

  static Value* getCastOperand(CastInst &I){
    Value* val;
    for (User::op_iterator OP = I.op_begin(), OPE = I.op_end(); OP != OPE; ++OP){
      val = OP->get();
    }
    return val;
  }

  static Value* getGEPIndex(GetElementPtrInst &I){
    Value* val;
    int i = 0;
    for (User::op_iterator OP = I.op_begin(), OPE = I.op_end(); OP != OPE; ++OP){
      if (i == 2) {
        val = OP->get();
      }
      i++;
    }
    return val;
  }

  // Returns the operands of a comparison operator.
  static std::pair<Use*, Use *> getOperands(CmpInst &BO){
    Use* S1;
    Use* S2;
    int i = 0;
    for (User::op_iterator OP = BO.op_begin(), OPE = BO.op_end(); OP != OPE; ++OP){
      if(i == 0){
        S1 = &*OP;
      }
      else{
        S2 = &*OP;
      }
      i++;
    }

    // S1 = LHS, S2 = RHS
    return std::make_pair(S1, S2);
  }

  static std::pair<Use*, Use *> getOps(Instruction &I){
    Use* S1;
    Use* S2;
    int i = 0;
    for (User::op_iterator OP = I.op_begin(), OPE = I.op_end(); OP != OPE; ++OP){
      if(i == 1){
        S1 = &*OP;
      }
      else if(i == 2){
        S2 = &*OP;
      }
      i++;
    }
    return std::make_pair(S2, S1);
  }

  static std::pair<Use*, Use *> getBranches(BranchInst &BI){
    Use* S1;
    Use* S2;
    int i = 0;
    for (User::op_iterator OP = BI.op_begin(), OPE = BI.op_end(); OP != OPE; ++OP){
      if(i == 1){
        S1 = &*OP;
      }
      else if(i == 2){
        S2 = &*OP;
      }
      i++;
    }
    return std::make_pair(S2, S1);
  }

  /*
  static std::pair<Use*, Use *> getOperands(ICmpInst &cmp){
    Use* S1;
    Use* S2;
    int i = 0;
    for (User::op_iterator OP = cmp.op_begin(), OPE = cmp.op_end(); OP != OPE; ++OP){
      if(i == 0){
        S1 = &*OP;
      }
      else{
        S2 = &*OP;
      }
      i++;
    }
    return std::make_pair(S1, S2);
  }
   */

  /*
   folds the binary operator for the special case of both arguments being ConstantInts.
   */
  static ConstantInt* foldBinaryOperator(unsigned Opcode ,ConstantInt *C1, ConstantInt *C2){
    LLVMContext &context = C1->getContext();
    const APInt &c1_value = C1->getValue();
    const APInt &c2_value = C2->getValue();
    ConstantInt* result = ConstantInt::get(context, APInt::getNullValue(C1->getBitWidth()));
    switch (Opcode) {
      case Instruction::Xor:
        result = ConstantInt::get(context, c1_value.Xor(c2_value));
        break;
      case Instruction::Add:
        result = ConstantInt::get(context, c1_value + c2_value);
        break;
      case Instruction::Sub:
        result = ConstantInt::get(context, c1_value - c2_value);
        break;
      case Instruction::Mul:
        result = ConstantInt::get(context, c1_value * c2_value);
        break;
      case Instruction::UDiv:
        result = ConstantInt::get(context, c1_value.udiv(c2_value));
        break;
      case Instruction::SDiv:
        result = ConstantInt::get(context, c1_value.sdiv(c2_value));
        break;
      case Instruction::URem:
        result = ConstantInt::get(context, c1_value.urem(c2_value));
        break;
      case Instruction::SRem:
        result = ConstantInt::get(context, c1_value.srem(c2_value));
        break;
      case Instruction::Or:
        result = ConstantInt::get(context, c1_value.Or(c2_value));
        break;
      case Instruction::And:
        result = ConstantInt::get(context, c1_value.And(c2_value));
        break;
      case Instruction::LShr:
        result = ConstantInt::get(context, c1_value.lshr(c2_value));
        break;
      case Instruction::AShr:
        result = ConstantInt::get(context, c1_value.ashr(c2_value));
        break;
      case Instruction::Shl:
        result = ConstantInt::get(context, c1_value.shl(c2_value));
        break;
      default:
        errs() << "default\n";
    }
    return result;
  }

  /*
   folds the binary operator for the special case of both arguments being ConstantRanges.
   */
  static ConstantRange* foldBinaryOperator(unsigned Opcode ,ConstantRange *C1, ConstantRange *C2){
    ConstantRange* result = new ConstantRange(C1->getBitWidth(), true);
    switch (Opcode) {
      case Instruction::SDiv:
      case Instruction::URem:
      case Instruction::SRem:
      case Instruction::AShr:
      case Instruction::Xor:
        break;
      case Instruction::Add:
        *result = C1->add(*C2);
        break;
      case Instruction::Sub:
        *result = C1->sub(*C2);
        break;
      case Instruction::Mul: // Not fully implemented, apparently.
        *result = C1->multiply(*C2);
        break;
      case Instruction::UDiv:
        *result = C1->udiv(*C2);
        break;
      case Instruction::Or:
        *result = C1->binaryOr(*C2);
        break;
      case Instruction::And:
        *result = C1->binaryAnd(*C2);
        break;
      case Instruction::LShr:
        *result = C1->lshr(*C2);
        break;
      case Instruction::Shl:
        *result = C1->shl(*C2);
        break;
    }
    return result;
  }

};

#endif /* HELPER_H_ */
