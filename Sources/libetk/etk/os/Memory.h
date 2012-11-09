/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __ETK_TOOLS_MEMORY_H__
#define __ETK_TOOLS_MEMORY_H__

#ifndef ETK_MEMORY_CHECKER
#define ETK_MEMORY_CHECKER 0
#endif

// General 
#if ETK_MEMORY_CHECKER > 0
namespace etk {
    void MemFree( void * pointerData, const char * variableName, const char * functionName, int32_t line, const char * fileName );
    void * MemMalloc( size_t num, size_t size, uint8_t init, const char * variableName, const char * functionName, int32_t line, const char * fileName );
    void MemShowLogs( void );
};
#   define ETK_MALLOC(pointerData, nbElements, dataType)              do {                                                                                                                                \
                                                                            pointerData = (dataType *)etk::MemMalloc( (nbElements), sizeof(dataType), 0, #pointerData, __func__, __LINE__, __FILE__);    \
                                                                        }while(0)
#   define ETK_MALLOC_CAST(pointerData, nbElements, dataType, cast)   do {                                                                                                                                \
                                                                            pointerData = (cast)etk::MemMalloc( (nbElements), sizeof(dataType), 0, #pointerData, __func__, __LINE__, __FILE__);            \
                                                                        }while(0)
#   define ETK_CALLOC(pointerData, nbElements, dataType)              do {                                                                                                                                \
                                                                            pointerData = (dataType *)etk::MemMalloc( (nbElements), sizeof(dataType), 1, #pointerData, __func__, __LINE__, __FILE__);    \
                                                                        }while(0)
#   define ETK_CALLOC_CAST(pointerData, nbElements, dataType, cast)   do {                                                                                                                                \
                                                                            pointerData = (cast)etk::MemMalloc( (nbElements), sizeof(dataType), 1, #pointerData, __func__, __LINE__, __FILE__);            \
                                                                        }while(0)
#   define ETK_FREE(pointerData)                                      do { \
                                                                            etk::MemFree( (pointerData) , #pointerData, __func__, __LINE__, __FILE__); \
                                                                            (pointerData) = NULL; \
                                                                        }while(0)
#   define ETK_MEM_SHOW_LOG()                                         do { \
                                                                            etk::MemShowLogs(); \
                                                                        }while(0)
#else

#	define ETK_MALLOC(pointerData, nbElements, dataType)					do { \
																				(pointerData) = (dataType *)malloc( (nbElements) * sizeof(dataType) ); \
																			}while(0)

#	define ETK_MALLOC_CAST(pointerData, nbElements, dataType, cast)		do { \
																				(pointerData) = (cast)malloc( (nbElements) * sizeof(dataType) ); \
																			}while(0)

#	define ETK_CALLOC(pointerData, nbElements, dataType)					do { \
																				(pointerData) = (dataType *)calloc( (nbElements), sizeof(dataType) ); \
																			}while(0)

#	define ETK_CALLOC_CAST(pointerData, nbElements, dataType, cast)		do { \
																				(pointerData) = (cast)calloc( (nbElements), sizeof(dataType) ); \
																			}while(0)

#	define ETK_REALLOC(pointerData, nbElements, dataType)					do { \
																				(pointerData) = (dataType *)realloc( (pointerData), (nbElements)* sizeof(dataType) ); \
																			}while(0)

#	define ETK_REALLOC_CAST(pointerData, nbElements, dataType, cast)		do { \
																				(pointerData) = (cast)realloc( (pointerData), (nbElements) * sizeof(dataType) ); \
																			}while(0)

#	define ETK_FREE(pointerData)	do { \
										free( pointerData ); \
										(pointerData) = NULL; \
									}while(0)

#	define ETK_MEM_SHOW_LOG()		do { \
										TK_DEBUG("No Memory check availlable"); \
									}while(0)
#endif


#endif

