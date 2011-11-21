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
#include <widget/ewolButton.h>
#include <widget/ewolCheckBox.h>
#include <widget/ewolSizerHori.h>
#include <widget/ewolSizerVert.h>
#include <widget/ewolTest.h>

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
			// generate the display : 
			ewol::SizerHori * mySizer = new ewol::SizerHori();
			SetSubWidget(mySizer);
			
			ewol::Button * myButton = new ewol::Button("Mon Labell 1");
			mySizer->SubWidgetAdd(myButton);
			
			ewol::SizerVert * mySizerVert = new ewol::SizerVert();
			mySizer->SubWidgetAdd(mySizerVert);
			
			myButton = new ewol::Button("BT 3");
			myButton->SetExpendX(true);
			//myButton->SetExpendY(true);
			mySizerVert->SubWidgetAdd(myButton);
			
			ewol::Test * myTest = new ewol::Test();
			mySizerVert->SubWidgetAdd(myTest);
			
			ewol::CheckBox * myCheckBox = new ewol::CheckBox("mon label d'exemple de test");
			mySizerVert->SubWidgetAdd(myCheckBox);
			myCheckBox = new ewol::CheckBox("Exemple 2");
			mySizerVert->SubWidgetAdd(myCheckBox);
			myCheckBox = new ewol::CheckBox("Exemple 3 et plus si afinité");
			mySizerVert->SubWidgetAdd(myCheckBox);
			
			myTest = new ewol::Test();
			myTest->SetExpendY(true);
			mySizerVert->SubWidgetAdd(myTest);
			myButton = new ewol::Button("4 4 BT");
			myButton->SetExpendY(true);
			mySizerVert->SubWidgetAdd(myButton);
			
			myButton = new ewol::Button("Exemple 2");
			myButton->SetExpendX(true);
			mySizer->SubWidgetAdd(myButton);
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
	ewol::SetDefaultFont("freefont/FreeMono", 14);
	
	
	Plop * myWindowsExample = new Plop();
	
	
	// create the specific windows
	ewol::DisplayWindows(myWindowsExample);
	
	ewol::Run();
	
	ewol::DisplayWindows(NULL);
	
	delete(myWindowsExample);
	
	ewol::UnInit();
	
	
	return 0;
}
