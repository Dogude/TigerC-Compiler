#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

struct functions {
	
	char *name;
	int (*ptr)(lua_State*);
	
};

typedef struct functions Functions;

int MY_Box(lua_State *L) {
	const char *body = luaL_checkstring(L, 1);
	const char *title = luaL_checkstring(L, 2);
	int optional = luaL_optinteger(L, 3 , 0);
	int re = MessageBox(0,body,title,optional);
	lua_pushinteger(L,re);
	return 1;
}

typedef struct token Token; 
typedef struct File File;
    
struct token {
	
	int location;
	int precedence;			
	int (*op)(Token *);	
};

char * source; /* all functions should be able to acces to this*/
Token tokens[8192]; /* max tokens */
Token ops[8192];	
Token operands[8192];
int k;
int i; /* global index for source */

int op_include(Token * include) {
	
	int j = 0;	
	char filename[128];
	char * location = source + include->location + 2; /* +2 =' <' */
	static char buffer[8192];
	
	// find .h 
	while(*location != '>') {		
		filename[j++] = *location++;		
	}
	
	filename[j] = '\0';	
	FILE * file = fopen(filename,"r");	

	int read = fread(buffer,1,8192,file);
	fclose(file);
	
	char *include_start = source + i ;
	
	memcpy(include_start, buffer, read);	

	i += read;
	return 1;	
	
		
}


int tigerc_expr_eval () {
	
	int top = 0, out = 0;
	for(i = 0; i < k; i++) {
				
		if(tokens[i].precedence > 0 && tokens[i].precedence < 62) {

			while (top > 0 && ops[top - 1].precedence >= tokens[i].precedence) {
				
				top--;
				memcpy(operands + out, ops + top, sizeof(Token));
				out++;
			}
				
		memcpy(ops + top , tokens + i , sizeof(Token));
		top++;
			
		} else {
							
			memcpy(operands + out , tokens + i , sizeof(Token));
			out++;	
			
		}
	
	}
	
	while(top) {
		
		top--;
		memcpy(operands + out, ops + top, sizeof(Token));
		out++;
		
	}	
	
	return 1;
	
}

int token_collect(const char *lex , int precedence , int (*operation)(Token*)) {
	
	char * cmp = source + i;	                                    
	int len = strlen(lex);
	int test = strncmp(cmp, lex, len);                       
	
	if (test == 0) { 	
		tokens[k].precedence = precedence;                			
		tokens[k].op = operation;                                
		tokens[k].location = i;								                                          
		i += len;
		k++;		
		return 1;		
	} 

}

int tigerc_include_all_files(FILE *file , char **other) {
		
	source = malloc(2097152); /* allocate 2mb for all source files */
	int j = 0; /* only for traversing */
	int total = fread(source,1,2097152,file);		
	
	source[total] = '\0';

	
	include:
	
	while(source[i]) {		
		token_collect("#include", 10 ,NULL);	
		token_collect(" <", 9 , op_include);	
		token_collect(".h>", 8 , NULL);		
		i++;			
	}

	source[i] = '\0';

	for(j = 0; j < k; j++) 	
	switch(tokens[j].precedence) {
					
			case 9:				
			tokens[j].op(tokens+j);
			break;
			
			default:
			break;
			
	}
	
	source[i] = '\0';
	
	

	return 1;
		
}


int tigerc_all_tokens() {
	
	i = 0;
	k = 0;
	
	while(k < 8192 && source[i]) {
		token_collect("##",0,NULL);
		token_collect("define",0,NULL);
		token_collect("ifdef",0,NULL);
		token_collect("ifndef",0,NULL);
		token_collect("endif",0,NULL);
		token_collect("elif",0,NULL);
		token_collect("typedef",0,NULL);
		token_collect("static",0,NULL);
		token_collect("extern",0,NULL);
		token_collect("struct",0,NULL);
		token_collect("enum",0,NULL);
		token_collect("union",0,NULL);
		token_collect("int",0,NULL);
		token_collect("char",0,NULL);
		token_collect("void",0,NULL);
		token_collect("float",0,NULL);
		token_collect("double",0,NULL);
		token_collect("long double",0,NULL);
		token_collect("short",0,NULL);
		token_collect("const",0,NULL);
		token_collect("volatile",0,NULL);
		token_collect("inline",0,NULL);
		token_collect("break",0,NULL);
		token_collect("continue",0,NULL);
		token_collect("return",0,NULL);
		token_collect("goto",0,NULL);
		token_collect("if",0,NULL);
		token_collect("else",0,NULL);
		token_collect("else if",0,NULL);
		token_collect("while",0,NULL);
		token_collect("do",0,NULL);
		token_collect("for",0,NULL);
		token_collect("switch",0,NULL);
		token_collect("case",0,NULL);
		token_collect("default",0,NULL);
		token_collect("auto",0,NULL);
		token_collect("register",0,NULL);
		token_collect("signed",0,NULL);
		token_collect("unsigned",0,NULL);
		token_collect("ull",61,NULL);
		token_collect("ul",60,NULL);
		token_collect("f",59,NULL);
		token_collect("u",58,NULL);
		token_collect("L",57,NULL);
		token_collect("0x",56,NULL);
		token_collect("0b",55,NULL);
		token_collect("0o",54,NULL);
		token_collect(".",53,NULL);
		token_collect("->",52,NULL);
		token_collect("[",51,NULL);
		token_collect("]",51,NULL);   
		token_collect("(",49,NULL);
		token_collect(")",49,NULL);
		token_collect("++",47,NULL);
		token_collect("--",46,NULL);
		token_collect("!",45,NULL);
		token_collect("~",44,NULL);
		token_collect("sizeof",43,NULL);
		token_collect("*",42,NULL);
		token_collect("/",41,NULL);
		token_collect("%",40,NULL);
		token_collect("+",39,NULL);
		token_collect("-",38,NULL);
		token_collect("<<",37,NULL);
		token_collect(">>",36,NULL);
		token_collect("<",35,NULL);
		token_collect("<=",34,NULL);
		token_collect(">",33,NULL);
		token_collect(">=",32,NULL);
		token_collect("==",31,NULL);
		token_collect("!=",30,NULL);
		token_collect("&",29,NULL);
		token_collect("^",28,NULL);
		token_collect("|",27,NULL);
		token_collect("&&",26,NULL);
		token_collect("||",25,NULL);
		token_collect("?",24,NULL);
		token_collect(":",23,NULL);
		token_collect("=",22,NULL);
		token_collect("+=",21,NULL);
		token_collect("-=",20,NULL);
		token_collect("*=",19,NULL);
		token_collect("/=",18,NULL);
		token_collect("%=",17,NULL);
		token_collect(">>=",16,NULL);
		token_collect("<<=",15,NULL);
		token_collect("&=",14,NULL);
		token_collect("|=",13,NULL);
		token_collect("^=",12,NULL);
		token_collect(",",11,NULL);
		token_collect(";",0,NULL);
		token_collect("{",0,NULL);
		token_collect("}",0,NULL);
		token_collect("\"",0,NULL);
		token_collect("'",0,NULL);
		token_collect("\\",0,NULL);
		i++;
			
	}	
	
	tigerc_expr_eval();

}

char * process_source(char * filename) {
	
	FILE * file = fopen(filename,"r");
		
	tigerc_include_all_files(file,NULL);
		
	tigerc_all_tokens();

	fclose(file);
	
	return source;
	
} 

Token * getTokens() {

	return operands;

}



int MY_getTokens(lua_State *L) {
     
    void * tokens = getTokens();
    lua_pushlightuserdata(L, tokens); 
    return 1;

}

int MY_process_source(lua_State *L) {
  
    char *filename = (char *)luaL_checkstring(L, 1);
   	
    process_source(filename);
    
    lua_pushstring(L, source);  
    return 1;

}

int MY_tokenSize(lua_State *L) {
  
    lua_pushinteger(L, sizeof(Token));  
    return 1;

}

int MY_tokenCount(lua_State *L) {
  
    lua_pushinteger(L, k);  
    return 1;

}

Functions func[] = {
		
		{"MessageBoxA",MY_Box},
		{"getTokens",MY_getTokens},
		{"process_source",MY_process_source},
		{"tokenSize",MY_tokenSize},
		{"tokenCount",MY_tokenCount}
};