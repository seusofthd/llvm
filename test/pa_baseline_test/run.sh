clang -O0 -emit-llvm -c paSimpleTest.cpp -o paSimpleTest.bc

llvm-dis paSimpleTest.bc

opt -mem2reg < paSimpleTest.bc > pamem2reg.bc

llvm-dis pamem2reg.bc
