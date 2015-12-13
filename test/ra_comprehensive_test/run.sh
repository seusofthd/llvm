clang -O0 -emit-llvm -c welcome2.cpp -o welcome2.bc

opt -mem2reg < welcome2.bc > welcome2_mem.bc

llvm-dis welcome2.bc
llvm-dis welcome2_mem.bc

opt -load $LLVMLIB/pass.so -RAPass < welcome2_mem.ll >/dev/null
