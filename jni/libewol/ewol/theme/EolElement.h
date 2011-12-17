/**
 *******************************************************************************
 * @file ewol/theme/EolElement.h
 * @brief basic ewol theme eol file element (Header)
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



#ifndef __EWOL_THEME_EOL_ELEMENT_H__
#define __EWOL_THEME_EOL_ELEMENT_H__

#include <etk/Types.h>
#include <etk/String.h>
#include <etk/File.h>
#include <ewol/OObject.h>
#include <ewol/theme/EolColor.h>
#include <ewol/theme/EolElementFrame.h>


namespace ewol {
	namespace theme {
		class EolElement {
			public:
				EolElement(void);
				virtual ~EolElement(void);
				void Parse(TiXmlNode * pNode);
				etk::String GetName(void) const;
				void SetName(etk::String & newName);
				bool HasName(etk::String & newName) const;
				
				void Generate(const ewol::theme::Theme * myTheme, int32_t frameId, ewol::OObject2DColored & newObject, etkFloat_t posX, etkFloat_t posY, etkFloat_t sizeX, etkFloat_t sizeY);
				bool GenerateGroup(const ewol::theme::Theme * myTheme, etk::String groupName, ewol::OObject2DColored & newObject, etkFloat_t posX, etkFloat_t posY, etkFloat_t sizeX, etkFloat_t sizeY);
				bool GetColor(etk::String colorName, color_ts & selectedColor) const;
				
				int32_t GetNbFrame(void) const;
				int32_t GetFrameId(etk::String & frameName) const;
			private:
				void RemoveAll(void);
				etk::String m_name;
				/*
				void Load(etk::File & newFile) { };
				void Generate(int32_t id, int32_t frameId, OObject2DTextured & newObject, etkFloat_t posX, etkFloat_t posY, etkFloat_t sizeX, etkFloat_t sizeY) {};
				int32_t GetNbFrame(int32_t id) {return 0;};
				int32_t GetFrameId(int32_t id, etk::String & frameName) {return 0;};
				int32_t GetObjectId(etk::String name) { return -1; };
				*/
			private:
				etk::VectorType<ewol::theme::EolColor*>         m_listColor;
				etk::VectorType<ewol::theme::EolElementFrame*>  m_listGroup;
				etk::VectorType<ewol::theme::EolElementFrame*>  m_listElement;
				etkFloat_t    m_ratio;
				bool          m_clipX;
				bool          m_clipY;
				coord2D_ts    m_internalElemStart;
				coord2D_ts    m_internalElemStop;
			public:
				// acces to manage and create object ==> drawing system 
				
		};
	};
};
#endif

