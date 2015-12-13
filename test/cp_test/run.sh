clang -O0 -emit-llvm -c test3.cpp -o test3.bc

llvm-dis test3.bc

opt -load $LLVMLIB/pass.so -CPPass < test3.bc >/dev/null
