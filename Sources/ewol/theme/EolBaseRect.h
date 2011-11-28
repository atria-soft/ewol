/**
 *******************************************************************************
 * @file ewol/theme/EolBaseRect.h
 * @brief basic ewol theme eol file basic element type=rectangle (Header)
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

#ifndef __EWOL_THEME_EOL_BASE_RECT_H__
#define __EWOL_THEME_EOL_BASE_RECT_H__

namespace ewol {
	namespace theme {
		class EolBaseRect : public EolBase {
			public:
				EolBaseRect(void);
				virtual ~EolBaseRect(void);
				/*
					<rect color="..."
					      position="0.53;0.56"
					      size="0.22;0.11"
					/>
				*/
				virtual void Parse(TiXmlNode * pNode);
			private:
				etk::String     m_color;
				coord2D_ts      m_position;
				coord2D_ts      m_size;
		};
	};
};

#endif

