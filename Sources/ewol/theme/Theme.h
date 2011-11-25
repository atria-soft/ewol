/**
 *******************************************************************************
 * @file ewol/theme/Theme.h
 * @brief basic ewol Theme basic class (Header)
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



#ifndef __EWOL_EOL_THEME_H__
#define __EWOL_EOL_THEME_H__

#include <etk/Types.h>
#include <etk/String.h>
#include <etk/File.h>
#include <etk/VectorType.h>
#include <ewol/OObject.h>
#include <ewol/theme/EolColor.h>
#include <ewol/theme/EolBase.h>
#include <ewol/theme/EolElement.h>
#include <ewol/theme/EolElementFrame.h>

namespace ewol {
	namespace theme {
		class Theme {
			public:
				Theme(void);
				virtual ~Theme(void);
				void Load(etk::File & newFile, bool defaultTheme=false);
				void Generate(int32_t id, int32_t frameId, OObject2DTextured & newObject, etkFloat_t posX, etkFloat_t posY, etkFloat_t sizeX, etkFloat_t sizeY);
				int32_t GetNbFrame(int32_t id);
				int32_t GetFrameId(int32_t id, etk::String & frameName);
				int32_t GetObjectId(etk::String name);
			private:
				etk::VectorType<ewol::theme::EolColor*>    m_listColor;
				etk::VectorType<ewol::theme::EolBase*>     m_listGroup; // TODO : we need really to star a frame here ...
				etk::VectorType<ewol::theme::EolElement*>  m_listElement;
			public:
				// acces to manage and create object ==> drawing system 
				
		};
	};
};

#endif

