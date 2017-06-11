CXXFLAGS=-D_GLIBCXX_USE_CXX11_ABI=0

all:
	clang++ -o LVIExample ${CXXFLAGS} `llvm-config --cxxflags` `llvm-config --ldflags` LVIExample.cpp `llvm-config --libs` -lpthread -lncurses -ldl

clean:
	rm -f *.ll *~ LVIExample
