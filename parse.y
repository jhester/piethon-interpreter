%{
#include <iostream>
#include <map>
#include <string>
#include "ast.h"

	extern int yylex();
	void yyerror(const char * msg) { std::cout << msg << std::endl; }

	// Map of all the functions which maps string name to AstNode
	std::stack< std::map<std::string, Primitive> > executionstack;
	std::map<std::string, Ast*> functions;
	%}

	%union {
		Ast* ast;
		int ival;
		float fval;
		char *id;
		std::list<Parameter> *params;
	}

	%token CR
	%token ASSIGN
	%token PRINT
	%token<id> IDENT 
	%token<id> STRING
	%token<ival> NUMBER
	%token<fval> FLOAT
	%token LPAREN
	%token RPAREN
	%token DEF
	%token COLON
	%token COMMA
	%token PASS
	%token IF
	%token END
	%token RETURN
	%token LBRACKET
	%token RBRACKET
	%token PERIOD
	%token LEN
	%token WHILE
	
	%left  PLUS MINUS
	%left  MULT DIV MOD
	%left  DOUBLE_MULT
	%left  LESS LESSEQ GREAT GREATEQ EQ NOTEQ

	%nonassoc UMINUS
	%nonassoc LOWER_THAN_ELSE
	%nonassoc ELSE
	
	%type<params> parm_list 
	%type<ast> expr line selection stmt

	%%

	start   : start funcdef
	| { ; }
	;

	funcdef : DEF IDENT LPAREN parm_list RPAREN COLON stmt END { 
		// AST for function, add to function table
		FunctionNode* mainFunc = new FunctionNode('F', $7, NULL, std::string($2), $4);
		functions[std::string($2)] = mainFunc; 
		delete [] $2;
	}
	| CR
	;

	parm_list 
		: IDENT 	{	
			$$ = new std::list<Parameter>();
			$$->push_back(Parameter(std::string($1), Primitive()));
		}
		| IDENT COMMA parm_list {
			$3->push_back(Parameter(std::string($1), Primitive()));
			$$ = $3;
		}
		| NUMBER {
			$$ = new std::list<Parameter>();
			$$->push_back(Parameter("", Primitive($1)));
		}
		| NUMBER COMMA parm_list {
			$3->push_back(Parameter("", Primitive($1)));
			$$ = $3;
		}
		| FLOAT   {
			$$ = new std::list<Parameter>();
			$$->push_back(Parameter("", Primitive($1)));
		}
		| FLOAT COMMA parm_list {
			$3->push_back(Parameter("", Primitive($1)));
			$$ = $3;
		}
		| { $$ = new std::list<Parameter>(); }
	;

	stmt    
		: stmt line CR	{
			$$ = new AstNode('S', $1, $2);
		}
		| stmt selection CR	{
			$$ = new AstNode('S', $1, $2);
		}
		
		| { $$ = new AstNode('n', NULL, NULL); } // No-op
	;
	
	selection 
		: IF expr COLON stmt %prec LOWER_THAN_ELSE END	{
			$$ = new AstNode('?', $2,$4);
		}
		| IF expr COLON stmt ELSE COLON stmt END	{
			$$ = new AstIfElse(':', $2, $4, $7);		
		}
		
		| WHILE expr COLON stmt END{
			$$ = new AstNode('w', $2,$4);
		}
		; 

	line    
		: line IDENT ASSIGN expr	{
			// Identifier assigment	
			AstLeaf* lf= new AstLeaf('I', $2);
			$$ = new AstNode('=', lf,$4);
			//$$ = new AstNode('L', linend, NULL);
		}
		
		| line IDENT LBRACKET expr RBRACKET ASSIGN expr { 
			// Assignment of an array index
			AstArrayIndex* lf = new AstArrayIndex('a', $2, $4);
			$$ = new AstNode('=', lf,$7);
		}

		| line IDENT LPAREN parm_list RPAREN {
			// Function call
			$$ = new AstCallLeaf('C', $2, $4);
		}
	
		| line IDENT ASSIGN IDENT LPAREN parm_list RPAREN {
			// Function call and assign
			AstLeaf* lf= new AstLeaf('I', $2);			
			AstCallLeaf* call = new AstCallLeaf('C', $4, $6);
			$$ = new AstNode('=', lf,call);
		}
		
		| line IDENT ASSIGN LBRACKET parm_list RBRACKET {
			// Array creation and assignment
			AstLeaf* lf= new AstLeaf('I', $2);
			std::vector<Parameter> *vec = new std::vector<Parameter>();
			for (std::list<Parameter>::iterator paramitr = $5->begin(); paramitr != $5->end(); ++paramitr) {
				vec->push_back(*paramitr);
			}
		 	std::reverse(vec->begin(),vec->end()); 
			AstArray* arr = new AstArray('A', vec);
			$$ = new AstNode('=', lf,arr);
		}
		
		| line IDENT PERIOD IDENT LPAREN expr RPAREN { 
			// Method dispatch, with single argument
			$$ = new AstMethod('m', std::string($4), $6, $2);
		}
		
		| line IDENT PERIOD IDENT LPAREN RPAREN { 
			// No argument method dispatch
			$$ = new AstMethod('m', std::string($4), NULL, $2);
		}
		
		| line PRINT expr {
			// Print statement
			$$ = new AstNode('P', $3, NULL);			
		}

		| line RETURN expr {
			// Return statement
			$$ = new AstNode('R', $3, NULL);
		}
		
		| { $$ = new AstNode('n', NULL, NULL); } // No-op
	;

	expr    
		: expr PLUS expr 	{ $$ = new AstNode('+', $1,$3); }
		| expr MINUS expr 	{ $$ = new AstNode('-', $1,$3);}
		| expr MULT expr 	{ $$ = new AstNode('*', $1,$3);}
		| expr DOUBLE_MULT expr		{ $$ = new AstNode('^', $1,$3);}
		| expr DIV expr		{ $$ = new AstNode('/', $1,$3);}
		| expr MOD expr		{ $$ = new AstNode('%', $1,$3);}
		| expr LESS expr	{ $$ = new AstNode('<', $1,$3);}
		| expr LESSEQ expr	{ $$ = new AstNode(',', $1,$3);}
		| expr GREAT expr	{ $$ = new AstNode('>', $1,$3);}
		| expr GREATEQ expr	{ $$ = new AstNode('.', $1,$3);}
		| expr NOTEQ expr	{ $$ = new AstNode('!', $1,$3);}
		| expr EQ expr		{ $$ = new AstNode('~', $1,$3);}
		| MINUS expr %prec UMINUS	{ $$ = new AstNode('M', $2, NULL); }
		| NUMBER	{ $$ = new AstNumber('K', Primitive($1)); }
		| FLOAT		{ $$ = new AstNumber('K', Primitive($1)); }
		| IDENT		{ $$ = new AstLeaf('I', $1)	}
		| IDENT LBRACKET expr RBRACKET { $$ = new AstArrayIndex('a', $1, $3); }
		| LEN LPAREN IDENT RPAREN {$$ = new AstLeaf('l', $3);}
		;
	%%
