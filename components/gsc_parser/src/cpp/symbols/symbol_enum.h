#ifndef __SYMBOL_ENUM_H__

#if SYMBOL_TYPE_ENUM_STRING
	#undef SYMBOL_TYPE_ENUM_STRING
	#include "symbol_enum.h"
	#define SYMBOL_TYPE_ENUM_STRING 1
	
	#define REGISTER_SYMBOL_TYPE(X) #X
#else
	#define REGISTER_SYMBOL_TYPE(X) X
#endif

#if SYMBOL_TYPE_ENUM_STRING
	static const char* SYMBOL_TYPE_STRINGS[] = {
#else
	enum SYMBOL_TYPE {
#endif
		REGISTER_SYMBOL_TYPE( S_TYPE_NONE ),
		
		REGISTER_SYMBOL_TYPE( S_TYPE_GROUP ),
		
		REGISTER_SYMBOL_TYPE( S_TYPE_LITERAL_UNDEFINED ),
		REGISTER_SYMBOL_TYPE( S_TYPE_LITERAL_INT ),
		REGISTER_SYMBOL_TYPE( S_TYPE_LITERAL_FLOAT ),
		REGISTER_SYMBOL_TYPE( S_TYPE_LITERAL_STRING ),
		REGISTER_SYMBOL_TYPE( S_TYPE_LITERAL_FILEPATH ),
		
		REGISTER_SYMBOL_TYPE( S_TYPE_IDENTIFIER ),
		
		REGISTER_SYMBOL_TYPE( S_TYPE_INCLUDE ),
		REGISTER_SYMBOL_TYPE( S_TYPE_ANIMTREE ),
		
		REGISTER_SYMBOL_TYPE( S_TYPE_FUNCTION_DECL ),
		REGISTER_SYMBOL_TYPE( S_TYPE_FUNCTION_CALL ),
		
		REGISTER_SYMBOL_TYPE( S_TYPE_STATEMENT ),
		REGISTER_SYMBOL_TYPE( S_TYPE_STATEMENT_BLOCK ),
		REGISTER_SYMBOL_TYPE( S_TYPE_STATEMENT_WAIT ),
		REGISTER_SYMBOL_TYPE( S_TYPE_STATEMENT_EXPRESSION ),
		REGISTER_SYMBOL_TYPE( S_TYPE_STATEMENT_IF ),
		REGISTER_SYMBOL_TYPE( S_TYPE_STATEMENT_SWITCH ),
		REGISTER_SYMBOL_TYPE( S_TYPE_STATEMENT_CASE ),
		REGISTER_SYMBOL_TYPE( S_TYPE_STATEMENT_LOOP ),
		REGISTER_SYMBOL_TYPE( S_TYPE_STATEMENT_RETURN ),
		REGISTER_SYMBOL_TYPE( S_TYPE_STATEMENT_BREAK ),
		REGISTER_SYMBOL_TYPE( S_TYPE_STATEMENT_CONTINUE ),
		REGISTER_SYMBOL_TYPE( S_TYPE_STATEMENT_EMPTY ),
		
		REGISTER_SYMBOL_TYPE( S_TYPE_CONDITIONAL_IF ),
		REGISTER_SYMBOL_TYPE( S_TYPE_CONDITIONAL_ELSE ),
		REGISTER_SYMBOL_TYPE( S_TYPE_CONDITIONAL_WHILE ),
		REGISTER_SYMBOL_TYPE( S_TYPE_CONDITIONAL_FOR ),
		REGISTER_SYMBOL_TYPE( S_TYPE_CONDITIONAL_SWITCH ),
		REGISTER_SYMBOL_TYPE( S_TYPE_CONDITIONAL_CASE ),
		
		REGISTER_SYMBOL_TYPE( S_TYPE_MEMBER_NULL ),
		REGISTER_SYMBOL_TYPE( S_TYPE_MEMBER_OBJECT_PROPERTY ),
		REGISTER_SYMBOL_TYPE( S_TYPE_MEMBER_ARRAY_ELEMENT ),
		REGISTER_SYMBOL_TYPE( S_TYPE_MEMBER_ARRAY_EMPTY ),
		
		REGISTER_SYMBOL_TYPE( S_TYPE_EXPRESSION ),
		REGISTER_SYMBOL_TYPE( S_TYPE_REFERENCE ),
		REGISTER_SYMBOL_TYPE( S_TYPE_POINTER )
		};

	#undef REGISTER_SYMBOL_TYPE

const char* SYMBOL_TYPE_STRING(SYMBOL_TYPE type);

#define __SYMBOL_ENUM_H__
#endif