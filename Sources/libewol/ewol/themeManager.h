/**
 *******************************************************************************
 * @file ewol/themeManager.h
 * @brief basic ewol theme Manager (Header)
 * @author Edouard DUPIN
 * @date 23/11/2011
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



#ifndef __EWOL_THEME_MANAGER_H__
#define __EWOL_THEME_MANAGER_H__

#include <etk/Types.h>
#include <etk/UString.h>
#include <etk/File.h>
#include <ewol/OObject.h>

namespace ewol {
	namespace theme {
		void Init(void);
		void UnInit(void);
		void LoadDefault(etk::File filename); // default system theme ==> when an element in not find in the user theme, it is search in this one ... not needed
		void Load(etk::File filename); // add a user theme at the list ==> this remove previous declaration by the user...
		int32_t GetObjectId(etk::UString name);
		// ???? GetObjectType(int32_t id);
		void Generate(int32_t id, int32_t frameId, ewol::OObject2DColored & newObject, etkFloat_t posX, etkFloat_t posY, etkFloat_t sizeX, etkFloat_t sizeY);
		int32_t GetNbFrame(int32_t id);
		int32_t GetFrameId(int32_t id, etk::UString & frameName);
		
	};
};


#endif

