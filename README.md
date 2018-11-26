decaf compiler
==============

## Repo Structure

- [ast.h](ast.h): It contains the ast structure of a  decaf program. I have made passes like printing the ast and the code generation in llvm using the visitor design pattern.
- BlockTable.h: It contains a class to store all the block variables in the local scope 
- compiler.l: This is the lex file and it generates the lexical analyzer of this language
- compiler.y: This file if the parser generator for the decaf language, and the grammar of the language has been specified in this file
- Visitor.h:  It contains the definitions of all the visitors, which are neccessary for a pass to be run on it
- PrintVisitor.cpp: This is a pass which prints the ast that has been generated after running the compiler on a decaff program
- CodeGenVisitor.h: This is a pass which generates the llvm IR, which can be furthur compiled using lli or clang and an executable can be generated
- main_codegen_visitor.cpp: This is the main file to run the CodeGenVisitor pass on the generated ast
- main_print_visitor.cpp: This is the main file to run the PrintVisitor pass on the generated ast

## Running the executable
```
// To run the CodeGenVisitor pass
make
./compiler <input_file> > <output_file>
clang-3.6 <output_file>
./a.out
```
