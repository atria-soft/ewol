/**
 *******************************************************************************
 * @file etkDebug.h
 * @brief Ewol Tool Kit : log implementation
 * @author Edouard DUPIN
 * @date 08/06/2010
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

#include <etkDebug.h>
#include <time.h>

// Max string size : (wide screan console nb caractere)
#define EDN_LOG_MAX_LENGTH 250


#define FUNCTION_NAME_SIZE	(70)

void TOOLS_DisplayFuncName(int32_t ligne, const char* className, const char* funcName, const char* libName)
{
	char tmpName[FUNCTION_NAME_SIZE] = "";
	
	if (NULL == className) {
		if (NULL == libName) {
			snprintf(tmpName, FUNCTION_NAME_SIZE, "???????? | (l=%5d) %s                                             ",ligne, funcName);
		} else {
			snprintf(tmpName, FUNCTION_NAME_SIZE, "%s | (l=%5d) %s                                             ",libName, ligne, funcName);
		}
	} else {
		if (NULL == libName) {
			snprintf(tmpName, FUNCTION_NAME_SIZE, "???????? | (l=%5d) %s::%s                                             ",ligne, className, funcName);
		} else {
			snprintf(tmpName, FUNCTION_NAME_SIZE, "%s | (l=%5d) %s::%s                                             ", libName, ligne, className, funcName);
		}
		
	}
	tmpName[FUNCTION_NAME_SIZE-4] = ' ';
	tmpName[FUNCTION_NAME_SIZE-3] = '|';
	tmpName[FUNCTION_NAME_SIZE-2] = ' ';
	tmpName[FUNCTION_NAME_SIZE-1] = '\0';
	std::cout << tmpName;
}


void TOOLS_DisplayTime(void)
{
	time_t rawtime;
	struct tm * timeinfo;
	char tmpdata[50];
	
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	sprintf(tmpdata, " %2dh %2dmin %2ds | ", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
	std::cout << tmpdata ;
}

