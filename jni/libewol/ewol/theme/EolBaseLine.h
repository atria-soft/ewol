/**
 *******************************************************************************
 * @file ewol/theme/EolBaseLine.h
 * @brief basic ewol theme eol file basic element type=Line (Header)
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


#include <ewol/theme/EolBase.h>
#include <ewol/theme/EolColor.h>

#ifndef __EWOL_THEME_EOL_BASE_LINE_H__
#define __EWOL_THEME_EOL_BASE_LINE_H__

namespace ewol {
	namespace theme {
		class EolBaseLine : public EolBase {
			public:
				EolBaseLine(void);
				virtual ~EolBaseLine(void);
				/*
					<line color="..."
					      positionStart="0.53;0.56"
					      positionStop="0.22;0.11"
					      thickness="0.05"
					/>
				*/
				virtual void Parse(TiXmlNode * pNode);
				virtual void Generate(const ewol::theme::Theme * myTheme, const ewol::theme::EolElement * myElement, ewol::OObject2DColored & newObject, etkFloat_t posX, etkFloat_t posY, etkFloat_t sizeX, etkFloat_t sizeY);
			private:
				etk::String     m_color;
				coord2D_ts      m_posStart;
				coord2D_ts      m_posStop;
				etkFloat_t      m_thickness;
		};
	};
};


#endif

