/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <etk/Types.h>
#include <etk/os/Memory.h>

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

