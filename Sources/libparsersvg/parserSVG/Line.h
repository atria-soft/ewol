/**
 *******************************************************************************
 * @file parserSVG/Line.h
 * @brief basic line parsing (Header)
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

#ifndef __SVG_LINE_H__
#define __SVG_LINE_H__

#include <parserSVG/Base.h>

namespace svg
{
	class Line : public svg::Base
	{
		private:
			coord2D_ts m_startPos;       //!< Start line position
			coord2D_ts m_stopPos;        //!< Stop line position
		public:
			Line(PaintState parentPaintState);
			~Line(void);
			virtual bool Parse(TiXmlNode * node);
			virtual void Display(int32_t spacing);
	};
};

#endif

