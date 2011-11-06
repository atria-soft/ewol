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


#include <ewol.h>
#include <ewolFont.h>

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// need to run xcompmgr to have transparency

class Plop :public ewol::Windows
{
	public:
		Plop(void)
		{
			
		};
		
		~Plop(void)
		{
			
		};
};

/**
 * @brief main input fonction
 */
int main(int argc, char *argv[])
{
	
	ewol::Init(argc, argv);
	/*
	if (true == ewol::AddFont("dataTest/TextMonospace.ebt", true, true, true) ) {
		//fontID = GetFontIdWithFileName("dataTest/TextMonospace.ebt");
	}
	*/
	ewol::SetFontFolder("Font");
	ewol::SetDefaultFont("freefont/FreeMono", 16);
	
	
	Plop * myWindowsExample = new Plop();
	
	
	// create the specific windows
	ewol::DisplayWindows(myWindowsExample);
	
	ewol::Run();
	
	ewol::DisplayWindows(NULL);
	
	delete(myWindowsExample);
	
	ewol::UnInit();
	
	
	return 0;
}
