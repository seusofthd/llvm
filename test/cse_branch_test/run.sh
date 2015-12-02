clang -O0 -emit-llvm -c cse_branch1.cpp -o cse_branch1.bc

opt -mem2reg < cse_branch1.bc > cse_branch1_mem2reg.bc

llvm-dis cse_branch1.bc
llvm-dis cse_branch1_mem2reg.bc

opt -load $LLVMLIB/pass.so -CSEPass < cse_branch1_mem2reg.ll >/dev/null
