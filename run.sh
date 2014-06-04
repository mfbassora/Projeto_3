./drive clean
./drive compile
opt -load Release/P3.so  -deadLoad -deadCodeElim  tests/minijava/LinkedList.bc  > LinkedList.opt
