/**
 *******************************************************************************
 * @file etkMemory.cpp
 * @brief Ewol Tool Kit : Memory implementation (Sources)
 * @author Edouard DUPIN
 * @date 19/10/2011
 * @par Project
 * Ewol TK
 *
 * @par Copyright
 * Copyright 2011 Edouard DUPIN, all right reserved
 *
 * This software is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY.
 *
 * Licence summary : 
 *    You can modify and redistribute the sources code and binaries.
 *    You can send me the bug-fix
 *
 * Term of the licence in in the file licence.txt.
 *
 *******************************************************************************
 */

#include <etkTypes.h>
#include <etkMemory.h>

// General 
#if ETK_MEMORY_CHECKER > 0

void etk::MemFree( void * pointerData, const char * variableName, const char * functionName, int32_t line, const char * fileName )
{
	TK_CRITICAL(" MEM FREE is not written ==> TODO...");
	if (NULL != pointerData) {
		free(pointerData);
	}
}

void * etk::MemMalloc( size_t num, size_t size, uint8_t init, const char * variableName, const char * functionName, int32_t line, const char * fileName )
{
	TK_CRITICAL(" MEM ALLOCATOR is not written ==> TODO...");
	return calloc(num, size);
}

void etk::MemShowLogs( void )
{
	TK_CRITICAL(" MEM DISPLAY is not written ==> TODO...");
}

#endif

