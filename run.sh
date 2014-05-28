./drive compile
opt -load Release/P3.so  -deadLoad tests/QuickSort.ll  > QuickSort.opt
lli QuickSort.opt
llvm-dis QuickSort.opt
