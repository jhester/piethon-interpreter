// Companion source code for "flex & bison", published by O'Reilly
// helper functions for fb3-1
#include <iostream>
#include <stdlib.h>
#include <math.h> 
#include "ast.h"
int Ast::ID_INCR = 0;

// Call stack and function listing
extern std::stack< std::map<std::string, Primitive> > executionstack;
extern std::map<std::string, Ast*> functions;

void makeGraph(Ast* node, FILE* out) {
	std::vector<Parameter> array;
	
	// Print the labels for the nodes
	switch( node->getNodetype() ) {
		case 'F':
		if(node->getName() == "main") {
			fprintf(out, "	%d [fontcolor = \"#ffffff\" label = \"function : %s\",  style=filled, color=\"#EA6A20\"]\n", node->getIdentifier(), node->getName().c_str());
		} else {
			fprintf(out, "	%d [fontcolor = \"#ffffff\" label = \"function : %s\",  style=filled, color=\"#3A4958\"]\n", node->getIdentifier(), node->getName().c_str());
		}
		break;
		case 'K':
		if(node->getNumber().type == INTEGER) {
			fprintf(out, "	%d [label = \"int:%d\", style=filled, color=\"#B5C327\"]\n", node->getIdentifier(), (int)node->getNumber().value);	
		} else {
			fprintf(out, "	%d [label = \"float:%.3F\", style=filled, color=\"#B5C327\"]\n", node->getIdentifier(), node->getNumber().value);	
		}
		
		break;
		case 'M':
		fprintf(out, "	%d [label = \"-\", style=filled, color=\"#D4C99E\"]\n", node->getIdentifier());
		break;
		case 'm':
		fprintf(out, "	%d [label = \"%s.%s()\", style=filled, color=\"#D4C99E\"]\n", node->getIdentifier(), node->getName().c_str(), node->getMethod().c_str());
		break;
		case 'I':
		fprintf(out, "	%d [label = \"%s\", style=filled, color=\"#D4C99E\"]\n", node->getIdentifier(), node->getName().c_str());
		break;
		case 'l':
		fprintf(out, "	%d [label = \"len(%s)\", style=filled, color=\"#D4C99E\"]\n", node->getIdentifier(), node->getName().c_str());
		break;
		case 'A':
		// Array, print the contents?
		fprintf(out, "	%d [label = \"Array: ", node->getIdentifier());
		array = *node->getMembers();
		for (int i=0;i< array.size();i++) {
			Parameter item = array[i];
			if(item.name == "") {
				fprintf(out, "|<f%d>%d", i, (int)item.value.value);
			}else {
				fprintf(out, "|<f%d>%s", i, item.name.c_str());
			}
		}
		fprintf(out, "\", shape=record, color=\"#D4C99E\"]\n");

		break;
		case 'a':
		fprintf(out, "	%d [label = \"%s[x]\", style=filled, color=\"#D4C99E\"]\n", node->getIdentifier(), node->getName().c_str());
		break;
		case 'C':
		fprintf(out, "	%d [fontcolor = \"#ffffff\" label = \"%s()\", style=filled, color=\"#3A4958\"]\n", node->getIdentifier(), node->getName().c_str());
		break;
		case 'S':
		fprintf(out, "	%d [label = \"stmt\",  style=filled, color=\"#109dc0\"]\n", node->getIdentifier());
		break;
		case '?':
		fprintf(out, "	%d [label = \"if\",  style=filled, color=\"#109dc0\"]\n", node->getIdentifier());
		break;
		case ':':
		fprintf(out, "	%d [label = \"ifelse\",  style=filled, color=\"#109dc0\"]\n", node->getIdentifier());
		break;	
		case 'w':
		fprintf(out, "	%d [label = \"while\",  style=filled, color=\"#109dc0\"]\n", node->getIdentifier());
		break;	
		case 'L':
		fprintf(out, "	%d [label = \"line\",  style=filled, color=\"#109dc0\"]\n", node->getIdentifier());
		break;
		case '!':
		fprintf(out, "	%d [label = \"!=\",  style=filled, color=\"#109dc0\"]\n", node->getIdentifier());
		break;
		case '~':
		fprintf(out, "	%d [label = \"==\",  style=filled, color=\"#109dc0\"]\n", node->getIdentifier());
		break;
		case ',':
		fprintf(out, "	%d [label = \"<=\",  style=filled, color=\"#109dc0\"]\n", node->getIdentifier());
		break;
		case '.':
		fprintf(out, "	%d [label = \">=\",  style=filled, color=\"#109dc0\"]\n", node->getIdentifier());
		break;
		case 'P':
		fprintf(out, "	%d [label = \"print\",  style=filled, color=\"#109dc0\"]\n", node->getIdentifier());
		break;
		case 'R':
		fprintf(out, "	%d [label = \"return\",  style=filled, color=\"#109dc0\"]\n", node->getIdentifier());
		break;
		case 'n':
		fprintf(out, "	%d [label = \"no op\",  style=filled, color=\"#aaaaaa\"]\n", node->getIdentifier());
		break;
		default:
		fprintf(out, "	%d [label = \"%c\",  style=filled, color=\"#109dc0\"]\n", node->getIdentifier(), node->getNodetype());
		break;
	}
	
	// Print the edges
	if(node->getLeft() != NULL) {
			fprintf(out, "	%d -> %d\n", node->getIdentifier(), node->getLeft()->getIdentifier());
			makeGraph(node->getLeft(), out);
	}
	if(node->getMiddle() != NULL) {
		fprintf(out, "	%d -> %d\n", node->getIdentifier(), node->getMiddle()->getIdentifier());		
		makeGraph(node->getMiddle(), out);
	}
	if(node->getRight() != NULL) {
			fprintf(out, "	%d -> %d\n", node->getIdentifier(), node->getRight()->getIdentifier());		
			makeGraph(node->getRight(), out);
	}

}

PrimitiveType getType(Primitive v, Primitive t) {
	if(v.type == FLOATING || t.type == FLOATING) {
		return FLOATING;
	} else return INTEGER;
}

Primitive eval(Ast *astNode) {
	// Switch cases == glorified GOTO == lame
	// I waste stack space like its my job
	Primitive v, left, right;
	std::string funcName;
	std::map<std::string, Ast*>::const_iterator itr;
	std::map<std::string, Primitive> newstackframe;
	std::map<std::string, Primitive>::const_iterator stackitr;
	
	// Function call and param stuff
	std::list<Parameter> functionparams;
	std::list<Parameter> callparams;
	std::list<Parameter>::const_iterator paramitr;
	std::vector<Primitive> *vec;
	std::vector<Primitive> vec2;	
	std::vector<Parameter> members;
	switch( astNode->getNodetype() ) {
		case 'F': 
			// Function only has a left branch
			v = eval(astNode->getLeft()); 
			break;
		case 'S': 
			// Only need to eval the right hand side since thats your return statement
			// The left hand side is either a no op, or the line before that does not matter
			eval(astNode->getLeft()); 
			v = eval(astNode->getRight()); 
			break; 
		case 'A':
			// Handle the right hand side of an array declaration
			// Set the values to real values
			vec = new std::vector<Primitive>();
			members = *astNode->getMembers();
			for(int i=0;i<members.size();i++) {
				if(members[i].name == "") {
					vec->push_back(members[i].value);
				} else {
					stackitr = executionstack.top().find(members[i].name);
					if(stackitr == executionstack.top().end()) {
						std::cout << "	Semantics error: undefined variables in array declaration" << std::endl;				
						exit (EXIT_FAILURE);
					}
					vec->push_back(executionstack.top()[members[i].name]);
				}
			}
			v = Primitive(vec);
			v.type = ARRAY;
			break;
		case 'a':
			// Handle rhs of a array index value retrieval
			stackitr = executionstack.top().find(astNode->getName());
			if(stackitr == executionstack.top().end()) {
				std::cout << "	Semantics error: list named " << astNode->getName() << " not defined" << std::endl;				
				exit (EXIT_FAILURE);
			}
			
			vec2 = *(executionstack.top()[astNode->getName()].array);
			v = vec2[(int)eval(astNode->getLeft()).value];
			break;
		case 'l':
			// Handle len()
			stackitr = executionstack.top().find(astNode->getName());
			if(stackitr == executionstack.top().end()) {
				std::cout << "	Semantics error: list named " << astNode->getName() << " not defined" << std::endl;				
				exit (EXIT_FAILURE);
			}
			if(executionstack.top()[astNode->getName()].type != ARRAY) {
				std::cout << "	Semantics error: " << astNode->getName() << " is not a list, cant call len()" << std::endl;				
				exit (EXIT_FAILURE);
			}
			vec2 = *(executionstack.top()[astNode->getName()].array);
			v = Primitive((int)vec2.size());
			break;
		case 'm':
			// Handle list.append(expr) and list.pop(expr)
			stackitr = executionstack.top().find(astNode->getName());
			if(stackitr == executionstack.top().end()) {
				std::cout << "	Semantics error: list named " << astNode->getName() << " not defined" << std::endl;				
				exit (EXIT_FAILURE);
			}
			if(executionstack.top()[astNode->getName()].type != ARRAY) {
				std::cout << "	Semantics error: " << astNode->getName() << " is not a list, cant call append()" << std::endl;				
				exit (EXIT_FAILURE);
			}
			if(astNode->getMethod() == "append") {
				executionstack.top()[astNode->getName()].array->push_back(eval(astNode->getLeft()));
			} else if(astNode->getMethod() == "pop") {
				executionstack.top()[astNode->getName()].array->pop_back();
			} else {
				std::cout << "	Semantics error: " << astNode->getMethod()  << " method not supported" << std::endl;				
				exit (EXIT_FAILURE);
			}
			
			break;	
		case 'C':
			// Get function node and evaluate it
			funcName = astNode->getName();
		 	itr = functions.find(funcName);
			if(itr == functions.end()) {
				std::cout << "	Semantics error: no function \"" << funcName << "\" found!" << std::endl;				
				exit (EXIT_FAILURE);
			}
			
			// If they try to call main, stop them
			if(funcName == "main") {
				std::cout << "	Semantics error: Not allowed to call \"main()\", duh." << std::endl;
				exit (EXIT_FAILURE);
			}
			
			// Match given parameters to function signature in number only (no typing for piethon)
			functionparams = *itr->second->getParams();
			callparams = *astNode->getParams();
			if(functionparams.size() != callparams.size()) {
				std::cout << "	Semantics error: given parameters do not match function signature of \"" << funcName << "\"!" << std::endl;				
				exit (EXIT_FAILURE);
			}
			
			// New stack with given params included to match signature
			for (paramitr = functionparams.begin(); paramitr != functionparams.end(); ++paramitr) {
				// If identifier get the value of it
				if(callparams.front().name != "") {
					stackitr = executionstack.top().find(callparams.front().name);
					if(stackitr == executionstack.top().end()) {
						std::cout << "	Semantics error: no variable named \"" << callparams.front().name << "\" found!" << std::endl;				
						exit (EXIT_FAILURE);
					}
					newstackframe[(*paramitr).name] = executionstack.top()[callparams.front().name];
				} else {
					// Otherwise just set to value
					newstackframe[(*paramitr).name] = callparams.front().value;
				}
				callparams.pop_front();
			}
			// Push new stack frame
			executionstack.push(newstackframe);
			// Call function
			v = eval(itr->second);
			
			// Pop, back to old stack frame
			executionstack.pop();
			break;
		case '?': 
			// If 
			if((int)eval(astNode->getLeft()).value == 1) {
				v = eval(astNode->getRight()); 	
			}
			break;
		case ':': 
			// If-else
			if((int)eval(astNode->getLeft()).value == 1) {
				v = eval(astNode->getMiddle()); 	
			} else {
				v = eval(astNode->getRight()); 	
			}
			break;
		case 'w': 
			// while
			while((int)eval(astNode->getLeft()).value == 1) {
				v = eval(astNode->getRight()); 	
			}
			break;
		case 'I': 
			// Look up value in table
			stackitr = executionstack.top().find(astNode->getName());
			if(stackitr == executionstack.top().end()) {
				std::cout << "	Semantics error: no variable \"" << astNode->getName() << "\" found!" << std::endl;				
				return 0;
			}
			v = executionstack.top()[astNode->getName()];
			break;
		case '=':
			// Set value of identifier in table
			if(astNode->getLeft()->getNodetype() == 'a') {
				vec2 = *(executionstack.top()[astNode->getLeft()->getName()].array);
				vec2[(int)eval(astNode->getLeft()->getLeft()).value] = eval(astNode->getRight());
				executionstack.top()[astNode->getLeft()->getName()].array->assign(vec2.begin(), vec2.end());
			} else {
				executionstack.top()[astNode->getLeft()->getName()] = eval(astNode->getRight());	
			}
			break;
		case '>':
			if(eval(astNode->getLeft()).value >  eval(astNode->getRight()).value) {
				v = Primitive(1.0); 	
				v.type = BOOLEAN;
			} else {
				v = Primitive(0.0); 	
				v.type = BOOLEAN;
			}
			break;
		case '.':
			if(eval(astNode->getLeft()).value >=  eval(astNode->getRight()).value) {
				v = Primitive(1.0); 	
				v.type = BOOLEAN;
			} else {
				v = Primitive(0.0); 	
				v.type = BOOLEAN;
			}
			break;
		case '<':
			if(eval(astNode->getLeft()).value <  eval(astNode->getRight()).value) {
				v = Primitive(1.0); 	
				v.type = BOOLEAN;
			} else {
				v = Primitive(0.0); 	
				v.type = BOOLEAN;
			}
			break;
		case ',':
			if(eval(astNode->getLeft()).value <=  eval(astNode->getRight()).value) {
				v = Primitive(1.0); 	
				v.type = BOOLEAN;
			} else {
				v = Primitive(0.0); 	
				v.type = BOOLEAN;
			}
			break;

		case '~':
			if(eval(astNode->getLeft()).value == eval(astNode->getRight()).value) {
				v = Primitive(1.0); 	
				v.type = BOOLEAN;
			} else {
				v = Primitive(0.0); 	
				v.type = BOOLEAN;
			}
			break;	
		case '!':
			if(eval(astNode->getLeft()).value != eval(astNode->getRight()).value) {
				v = Primitive(1.0); 	
				v.type = BOOLEAN;
			} else {
				v = Primitive(0.0); 	
				v.type = BOOLEAN;
			}
			break;			
		case 'n': 
			// Do nothing!
		break;
		case 'P':
			// Handle printing short string messages
			if(astNode->getName() != "") {
				std::cout << astNode->getName();
			} else {
			
			 	// Print stuff
				v = eval(astNode->getLeft());
				if(v.type == INTEGER) {
					printf("%d\n", (int)v.value); 
				} 
				if(v.type == FLOATING) {
					printf("%lf\n", v.value); 
				}
				// Arrays of arrays not really allowed to print
				if(v.type == ARRAY) {
					std::vector<Primitive> vec = *v.array;
					printf("[");
					for(int i=0;i<vec.size();i++) {
						if(vec[i].type == INTEGER) {
							printf("%d,", (int)vec[i].value); 
						} 
						if(vec[i].type == FLOATING) {
							printf("%lf,", vec[i].value); 
						}
					}
					printf("]\n");
				}
			}
			break;
		case 'R': v = eval(astNode->getLeft()); break; 
		
		case 'K': v = astNode->getNumber(); break;
		case '+': 
			left = eval(astNode->getLeft());
			right = eval(astNode->getRight());
			v = Primitive(left.value + right.value); 	
			v.type = getType(left, right);
			break;
		case '-': 
			left = eval(astNode->getLeft());
			right = eval(astNode->getRight());
			v = Primitive(left.value - right.value); 	
			v.type = getType(left, right);
			break;
		case '*': 
			left = eval(astNode->getLeft());
			right = eval(astNode->getRight());
			v = Primitive(left.value * right.value); 	
			v.type = getType(left, right);
		break;
		case '/': 
			left = eval(astNode->getLeft());
			right = eval(astNode->getRight());
			v = Primitive(left.value / right.value); 	
			v.type = getType(left, right);
			break;		
		break;
		case '%': 
			left = eval(astNode->getLeft());
			right = eval(astNode->getRight());
			v = Primitive(fmod(left.value, right.value)); 	
			v.type = getType(left, right);
			break;		
		case '^': 
			left = eval(astNode->getLeft());
			right = eval(astNode->getRight());
			v = Primitive(pow(left.value, right.value)); 	
			v.type = getType(left, right);
			break;		
		
		case 'M': v = Primitive(-1 * eval(astNode->getLeft()).value ); break;
		default: std::cout << "internal error: bad node "
			<< astNode->getNodetype() << std::endl;
	}
	return v;
}

void freeTree(Ast* temp) {
	if ( temp ) {
		Ast* left = temp->getLeft();
		Ast* right = temp->getRight();
		delete temp;
		freeTree(left);
		freeTree(right);
	}
}

void treefree(Ast *astNode) {
	switch(astNode->getNodetype()) {
	 // two subtrees
	case '+':
	case '-':
	case '*':
	case '/':
		delete(astNode->getRight());
		delete(astNode->getRight());
		break;
	 // one subtrees
	case 'M':	
	  delete (astNode->getLeft());

	 //no subtree
	case 'K':
	  delete astNode;
	  break;

	default: std::cout << "internal error: bad node " << astNode->getNodetype() << std::endl;
	}
	
	if(astNode->getRight() != NULL) {
		treefree(astNode->getRight());
	}
	if(astNode->getMiddle() != NULL) {
		treefree(astNode->getMiddle());
	}
	if(astNode->getLeft() != NULL) {
		treefree(astNode->getLeft());		
	}
}

