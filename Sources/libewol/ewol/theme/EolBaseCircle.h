/**
 *******************************************************************************
 * @file ewol/theme/EolBasicCircle.h
 * @brief basic ewol theme eol file basic element type=Circle (Header)
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

#ifndef __EWOL_THEME_EOL_BASE_CIRCLE_H__
#define __EWOL_THEME_EOL_BASE_CIRCLE_H__

namespace ewol {
	namespace theme {
		class EolBaseCircle : public EolBase {
			public:
				EolBaseCircle(void);
				virtual ~EolBaseCircle(void);
				/*
					<circle colorBG="..."    ==> generate condition to display background ...
					        colorBorder="..."    ==> generate condition to display border ...
					        position="0.53;0.56"
					        radius="0.2"
					        thickness="0.05"  ==> only if neededd by border, default 0.01
					/>
				*/
				virtual void Parse(TiXmlNode * pNode);
				virtual void Generate(const ewol::theme::Theme * myTheme, const ewol::theme::EolElement * myElement, ewol::OObject2DColored & newObject, etkFloat_t posX, etkFloat_t posY, etkFloat_t sizeX, etkFloat_t sizeY);
			private:
				etk::UString    m_colorBG;
				etk::UString    m_colorBorder;
				coord2D_ts      m_posCenter;
				etkFloat_t      m_radius;
				etkFloat_t      m_thickness;
		};
	};
};
#endif

