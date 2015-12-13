clang -O0 -emit-llvm -c paSimpleTest.cpp -o paSimpleTest.bc

#opt -mem2reg < paSimpleTest.bc > paSimpleTest_mem2reg.bc

llvm-dis paSimpleTest.bc
#llvm-dis paSimpleTest_mem2reg.bc

opt -load $LLVMLIB/pass.so -MAYPPass < paSimpleTest.bc >/dev/null
