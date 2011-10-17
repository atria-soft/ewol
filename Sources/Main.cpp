/**
 *******************************************************************************
 * @file Main.cpp
 * @brief main fonction
 * @author Edouard DUPIN
 * @date 13/10/2011
 * @par Project
 * ewol 
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


#include "ewol.h"

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * @brief main input fonction
 */
int main(int argc, char *argv[])
{
	printf("==================================================\n");
	printf("== ewol:: Init()\n");
	printf("==================================================\n");
	ewol::Init(argc, argv);
	
	// create the windows
	//ewol::Windows myWindows(ewol::WINDOWS_MAIN);
	
	
	printf("==================================================\n");
	printf("== ewol:: Run()\n");
	printf("==================================================\n");
	ewol::Run();
	
	printf("==================================================\n");
	printf("== ewol:: UnInit()\n");
	printf("==================================================\n");
	ewol::UnInit();
	
	return 0;
}
