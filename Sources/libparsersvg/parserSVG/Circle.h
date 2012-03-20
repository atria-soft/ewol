/**
 *******************************************************************************
 * @file parserSVG/Circle.h
 * @brief basic circle parsing (Header)
 * @author Edouard DUPIN
 * @date 20/03/2012
 * @par Project
 * parserSVG
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

#ifndef __SVG_CIRCLE_H__
#define __SVG_CIRCLE_H__

#include <parserSVG/Base.h>

namespace svg
{
	class Circle : public svg::Base
	{
		private:
			coord2D_ts m_position;        //!< position of the Circle
			etkFloat_t m_ratio;           //!< Ratio of the Circle
		public:
			Circle(void);
			~Circle(void);
			virtual bool Parse(TiXmlNode * node);
	};
};

#endif

