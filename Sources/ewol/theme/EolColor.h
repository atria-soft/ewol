/**
 *******************************************************************************
 * @file ewol/theme/EolColor.h
 * @brief basic ewol theme eol file color (Header)
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


#ifndef __EWOL_THEME_EOL_COLOR_H__
#define __EWOL_THEME_EOL_COLOR_H__


#include <etk/Types.h>
#include <etk/String.h>
#include <etk/File.h>
#include <ewol/OObject.h>
#include <tinyXML/tinyxml.h>


namespace ewol {
	namespace theme {
		class EolColor {
			public:
				EolColor(void);
				virtual ~EolColor(void);
				
				void Parse(TiXmlNode * pNode);
				etk::String GetName(void);
				void SetName(etk::String & newName);
				bool HasName(etk::String & newName);
				color_ts Get(void);
				void Set(color_ts newColor);
				void Set(etkFloat_t red, etkFloat_t green, etkFloat_t blue, etkFloat_t alpha = 1);
			private:
				color_ts    m_color;
				etk::String m_name;
		};
	};
};


#endif

