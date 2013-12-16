//  Declarations for an AST calculator
//  From "flex & bison", fb3-1, by John Levine
//  Adapted by Brian Malloy

#include <string>
#include <stdio.h>
#include <map>
#include <stack>
#include <list>
#include <vector>

extern void yyerror(const char*);
extern void yyerror(const char*, const char);

enum PrimitiveType { FLOATING, INTEGER, BOOLEAN, ARRAY };

class Primitive {	
public:
	Primitive() {}
	~Primitive() {
		//delete[] array;	
		//delete array;
		// This has to be handled externally since these go on the call stack
	}
	Primitive(float value) : type(FLOATING), value(value) {}
	Primitive(double value) : type(FLOATING), value(value) {}	
	Primitive(int value) : type(INTEGER), value(value) {}
	Primitive(std::vector<Primitive> *arr) : type(INTEGER), array(arr) {}
	
	// Should use union maybe? Hacky hack
	PrimitiveType type;
	double value;
	std::vector<Primitive> *array;
};

class Parameter {
public:
	Parameter(std::string n, Primitive v) : name(n), value(v) {}
	std::string name;
	Primitive value;
};

class Ast {
public:
	Ast(int n) : nodetype(n), identifier(ID_INCR++) {}
	virtual ~Ast() {}
	char getNodetype() const { return nodetype; }
	char getIdentifier() const { return identifier; }
	
	virtual Ast* getLeft() const { return NULL; }
	virtual Ast* getMiddle() const  { return NULL; }
	virtual Ast* getRight() const { return NULL; }
	
	virtual Primitive getNumber() const { throw std::string("No Number"); }
	virtual std::string getMethod() const { throw std::string("No name"); }
	virtual std::string getName() const { return ""; }
	virtual std::list<Parameter>* getParams() const { throw std::string("No params"); }
	virtual std::vector<Parameter>* getMembers() const { throw std::string("No members"); }	
	virtual int getIndex() { throw std::string("No index"); }
	virtual void printInfo() const {
		printf("(id='%d',type='%c')\n", this->getIdentifier(), this->getNodetype());
	}	
	virtual void printStructure() const {
		if(this->getRight() != NULL) {
			printf("	%d-R::",  this->getIdentifier());
			this->getRight()->printInfo();
			printf("	");
			this->getRight()->printStructure();			
		}
		if(this->getLeft() != NULL) {
			printf("	%d-L::",  this->getIdentifier());
			this->getLeft()->printInfo();
			printf("	");			
			this->getLeft()->printStructure();						
		}
	}
	static int ID_INCR;
private:
	char nodetype;
	int identifier;
};

class AstNode : public Ast {
public:
	AstNode(char nodetype, Ast* l, Ast* r) : 
	  Ast(nodetype), left(l), right(r) 
	{}
	virtual ~AstNode() {}
	virtual Ast* getLeft() const  { return left; }
	virtual Ast* getRight() const { return right; }
private:
	Ast *left;
	Ast *right;
};

class AstMethod : public AstNode {
public:
	AstMethod(char nodetype, std::string m, Ast* expr, std::string n) : 
	  AstNode(nodetype, expr, NULL), method(m), name(n)
	{}
	virtual std::string getName() const { return name; }
	virtual std::string getMethod() const { return method; }
private:
	std::string method;
	std::string name;	
};

class FunctionNode : public AstNode {
public:
	FunctionNode(char nodetype, Ast* l, Ast* r, std::string n, std::list<Parameter> *p) : 
	  AstNode(nodetype, l, r), name(n), params(p)
	{}
	virtual ~FunctionNode() {delete[] params;}
	virtual std::string getName() const { return name; }
	virtual std::list<Parameter>* getParams() const { return params; }
private:
	std::string name;
	std::list<Parameter> *params;
};

class AstNumber : public Ast {
public:
  AstNumber(char nodetype, Primitive n) : Ast(nodetype), number(n) {} 
  virtual ~AstNumber() {}
  virtual Primitive getNumber() const { return number; }
private:
  Primitive number;
	
};

class AstIfElse : public AstNode {
public:
	AstIfElse(char nodetype, Ast* expr, Ast* ifstmt, Ast* elsestmt) : 
	  AstNode(nodetype, expr, elsestmt), middle(ifstmt)
	{}
	virtual Ast* getMiddle() const  { return middle; }
private:
	Ast *middle;		
};

class AstLeaf : public Ast {
public:
  AstLeaf(char nodetype, std::string n) : Ast(nodetype), name(n) {} 
  virtual ~AstLeaf() {}
  virtual std::string getName() const { return name; }
private:
  std::string name;
};

class AstArray : public Ast {
public:
  	AstArray(char nodetype, std::vector<Parameter>* n) : Ast(nodetype), members(n) {} 
  	virtual ~AstArray() {
		delete[] members;
	}
	virtual std::vector<Parameter>* getMembers() const { return members; }	
private:
	std::vector<Parameter>* members;
};

class AstArrayIndex : public AstLeaf {
public:
	AstArrayIndex(char nodetype, std::string n, Ast *i) : AstLeaf(nodetype, n), index(i)  {}
	virtual Ast* getLeft() const  { return index; }
private:
	Ast *index;	
};

class AstCallLeaf : public AstLeaf {
public:
	AstCallLeaf(char nodetype, std::string n, std::list<Parameter> *p) : AstLeaf(nodetype, n), params(p) {} 
	virtual ~AstCallLeaf() {delete[] params;}
	virtual std::list<Parameter>* getParams() const { return params; }
private:
	std::list<Parameter> *params;
};

Primitive eval(Ast*);   // Evaluate an AST
void treefree(Ast*); // delete and free an AST 
void freeTree(Ast*);
void makeGraph(Ast*, FILE*); // Print a graph of the node in dot format