/**
 *******************************************************************************
 * @file ewol/theme/EolElementFrame.h
 * @brief basic ewol theme eol file element (specific frame) (Header)
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



#ifndef __EWOL_THEME_EOL_ELEMENT_FRAME_H__
#define __EWOL_THEME_EOL_ELEMENT_FRAME_H__

#include <etk/Types.h>
#include <etk/UString.h>
#include <etk/File.h>
#include <ewol/OObject.h>
#include <ewol/theme/EolColor.h>
#include <ewol/theme/EolBase.h>
#include <ewol/theme/EolBaseLine.h>
#include <ewol/theme/EolBaseRect.h>
#include <ewol/theme/EolBaseCircle.h>
#include <ewol/theme/EolBaseCirclePart.h>
#include <ewol/theme/EolBaseTriangle.h>
#include <ewol/theme/EolBasePolygone.h>


namespace ewol {
	namespace theme {
		class EolElementFrame {
			public:
				EolElementFrame(void);
				virtual ~EolElementFrame(void);
				
				void Generate(const ewol::theme::Theme * myTheme, const ewol::theme::EolElement * myElement, ewol::OObject2DColored & newObject, etkFloat_t posX, etkFloat_t posY, etkFloat_t sizeX, etkFloat_t sizeY);
				
				void Parse(TiXmlNode * pNode);
				etk::UString GetName(void);
				void SetName(etk::UString & newName);
				bool HasName(etk::UString & newName);
			private:
				void RemoveAll(void);
				etk::UString m_name;
				etk::VectorType<ewol::theme::EolBase*> m_description; // all element to draw the image ...
			public:
				// acces to manage and create object ==> drawing system 
				
		};
	};
};


#endif

