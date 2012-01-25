/**
 *******************************************************************************
 * @file etk/Debug.h
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

#include <etk/Debug.h>
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
	etk::cout << tmpName;
}

void TOOLS_DisplayTime(void)
{
	char tmpdata[50];
#ifdef __PLATFORM__Android
	struct timeval  now;
	gettimeofday(&now, NULL);
	sprintf(tmpdata, " %2dh %2dmin %2ds | ", (int32_t)(now.tv_sec/3600)%24, (int32_t)(now.tv_sec/60)%60, (int32_t)(now.tv_sec%60));
#else
	time_t rawtime;
	struct tm * timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	sprintf(tmpdata, " %2dh %2dmin %2ds | ", (timeinfo->tm_hour)%24, timeinfo->tm_min, timeinfo->tm_sec);
#endif
	etk::cout << tmpdata ;
}

