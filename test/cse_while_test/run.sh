clang -O0 -emit-llvm -c cse_while.cpp -o cse_while.bc

opt -mem2reg < cse_while.bc > cse_while_mem2reg.bc

llvm-dis cse_while.bc
llvm-dis cse_while_mem2reg.bc

opt -load $LLVMLIB/pass.so -CSEPass < cse_while_mem2reg.bc >/dev/null
