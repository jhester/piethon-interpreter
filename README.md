piethon-interpreter
===================

A bastardized, poorly written, barely functional python interpreter with all the useful features of the language stripped out. Purely an academic exercise with lex / flex and yacc / bison.

### Features

* ##### Abstract Syntax Tree:
		- The program builds and draws an AST of all functions on run using graphviz
	Semantics checking throughout, similar to how clang does it:
		- Checks to see if called function arguments match function signature
		- Checks to see you have declared a main function
		- Checks if program has instantiated a variable before use

* ##### Python style comments supported:
		- multiline and single line

* ##### List / Array Support
		- Can instantiate arrays with multiple types, including identifiers (resolved on creation) floats and integers
		- Can set an index in the array
		- Can get values from the array to set variables
		- can use the pop() and append() methods on lists

* ##### Built-in functions:
		- len() now provided for lists

* ##### while loops
		- super awesome, super powerful
		
### USAGE
Use `make` to build the program:

Run individual test cases with:
./run < testsuite/[NAME_OF_TEST_CASE].py

To create the AST, just enter `make pdf`
This generates the AST `ast.pdf` from AST.gv

`make clean` clears all the intermediate files and executables.