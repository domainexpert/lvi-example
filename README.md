# lvi-example
LLVM LazyValueInfo test for my own study. ATM I have not yet found an
example program that produces analysis result. This example also shows
how one can create and use own analysis that use an existing
FunctionPass in a standalone analysis. To build:
```
make
```
To run, simply say, e.g.:
```
./LVIExample program.bc
```
