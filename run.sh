./drive compile
opt -load Release/P3.so  -deadCodeElim tests/QuickSort.ll  > QuickSort.opt
lli QuickSort.opt
llvm-dis QuickSort.opt
