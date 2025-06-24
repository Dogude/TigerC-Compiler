#include <stdlib.h>
#include <string.h>

int MY_alloc(lua_State *L) {
	
	int size = luaL_checkinteger(L, 1);
	
	void * buffer = malloc(size);
	memset(buffer,0,size);
	
	lua_pushlightuserdata(L,buffer);
	
	return 1;
}


int MY_release(lua_State *L) {
	
	void * buffer = lua_touserdata(L, 1);
		
	free(buffer);
			
	return 1;
	
}

int MY_fetch(lua_State *L) {
	
	void * buffer = lua_touserdata(L, 1);
	int offset = luaL_checkinteger(L, 2);
	const char *ptr = luaL_checkstring(L, 3);
										
	char *base = (char*)buffer + offset;	
	
	if(strcmp(ptr,"ptr") == 0) {
		void *p;
    	memcpy(&p, base, sizeof(void*));  // Safe pointer read
    	lua_pushlightuserdata(L, p);	
		return 1;
	} else if(strcmp(ptr,"int") == 0) {
		int data;
		memcpy(&data, base, sizeof(int));	
		lua_pushinteger(L,data);
		return 1;
	} else if(strcmp(ptr,"byte") == 0) {
		
		unsigned char data;
		memcpy(&data, base, sizeof(char));	
		lua_pushinteger(L,data);	
		return 1;				
	} else if(strcmp(ptr,"str") == 0) { 
		
		const char *str = (const char *)base;
		lua_pushstring(L,str);	
		return 1;
	} else if (strcmp(ptr, "double") == 0) {
    	double value;
    	memcpy(&value, base, sizeof(double));
    	lua_pushnumber(L,value);
		return 1;
	} else {
		lua_pushinteger(L,0);
		return 1;
	}
	
	return 1;
}

int MY_store(lua_State *L) {
	
	void * buffer = lua_touserdata(L, 1);
	int offset = luaL_checkinteger(L, 2);
	const char *ptr = luaL_checkstring(L, 3);
										
	char *base = (char*)buffer + offset;	
	
	if(strcmp(ptr,"ptr") == 0) {
		void *p = lua_touserdata(L, 4);
    	memcpy(base, &p, sizeof(void*));
		return 1;
	} else if(strcmp(ptr,"int") == 0) {
		int data = luaL_checkinteger(L, 4);
		memcpy(base, &data, sizeof(int));	
		return 1;
	} else if(strcmp(ptr,"byte") == 0) {
		
		unsigned char data = luaL_checkinteger(L, 4);
		memcpy(base, &data, sizeof(char));	
		return 1;				
	} else if(strcmp(ptr,"str") == 0) { 
		
		const char *data = luaL_checkstring(L, 4);
		memcpy(base, data , strlen(data) + 1);
		return 1;
	}	else if (strcmp(ptr, "double") == 0) {
    	double value = (double)luaL_checknumber(L, 4);
    	memcpy(base, &value, sizeof(double));
    	return 1;
	} else {
		lua_pushinteger(L,0);
		return 1;
	}
	
	return 1;
}









