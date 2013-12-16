#include <iostream>
#include <map>
#include "ast.h"

extern int yyparse();

extern std::map<std::string, Ast*> functions;
extern std::stack< std::map<std::string, Primitive> > executionstack;

int main() {
	extern int yydebug;
	yydebug = 1;
	try {
		if ( yyparse() == 0 ) {
			std::cout << "Syntactically correct program." << std::endl;
			std::cout << "Generating AST." << std::endl;			
			// Make the AST graph
			std::map<std::string, Ast*>::const_iterator itrall;
			FILE * pFile = fopen ("AST.gv","w");
			fprintf(pFile, "digraph {\n	node [ fontname=Helvetica, fontsize=14]\n");
			for (itrall = functions.begin(); itrall != functions.end(); ++itrall) {
				makeGraph(itrall->second, pFile);
			}			
			fprintf(pFile, "}\n");
			fclose(pFile);
			std::cout << "	AST graph written to 'AST.gv'" << std::endl;

			std::cout << "Running program:" << std::endl;
			std::cout << "	Searching for main()..." << std::endl;
			std::map<std::string, Ast*>::const_iterator itr = functions.find("main");
			if(itr == functions.end()) {
				std::cout << "	No main() found!" << std::endl;				
				return 0;
			}
			
			// Get main
			std::cout << "	main() found, writing AST graph..." << std::endl;
			Ast *mainNode = itr->second;
			
			// Print debug info
			mainNode->printInfo();
			mainNode->printStructure();
			
			// Add first stack frame
			std::map<std::string, Primitive> firstframe;
			firstframe["x"] = Primitive();
			executionstack.push(firstframe);
						
			// Execute main
			std::cout << "Executing main()" << std::endl;			
			std::cout << "==========" << std::endl;
			eval(mainNode);
			std::cout << "==========" << std::endl;	
			std::cout << "Have a nice day" << std::endl;						
			return 0;
		}
		else {
			std::cout << "Syntactically incorrect program!" << std::endl;
			return -1;
		}
	}
	catch( const std::string& msg) {
		std::cout << msg << std::endl;
	}
	catch( ... ) {
		std::cout << "ERROR" << std::endl;
	}
	
	// FRee resources on the execution stack TODO
	
	// Free resources from AST
	std::map<std::string, Ast*>::const_iterator funcitr;
	for (funcitr = functions.begin(); funcitr != functions.end(); ++funcitr) {
		treefree(funcitr->second);
	}
	
	
	return 0;
}
