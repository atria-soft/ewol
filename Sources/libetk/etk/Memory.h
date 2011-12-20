/**
 *******************************************************************************
 * @file etk/Memory.h
 * @brief Ewol Tool Kit : Memory implementation (headers)
 * @author Edouard DUPIN
 * @date 12/01/2011
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
 
#ifndef __TOOLS_MEMORY_H__
#define __TOOLS_MEMORY_H__

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

