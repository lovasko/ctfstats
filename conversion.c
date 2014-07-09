#include "conversion.h"

const char*
kind_to_string (ctf_kind kind)
{
	static const char *translation_table[] = 
	{
		"none", 
		"int", 
		"float", 
		"pointer",
		"array",
		"function", 
		"struct", 
		"union", 
		"enum", 
		"forward declaration", 
		"typedef",
		"volatile",
		"const", 
		"restrict"
	};

	if (kind <= CTF_KIND_MAX && kind >= 0)
		return translation_table[kind];
	else
		return "unresolvable";
}

