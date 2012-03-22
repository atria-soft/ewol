/**
 *******************************************************************************
 * @file parserSVG/Path.h
 * @brief basic path parsing (Header)
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

#ifndef __SVG_PATH_H__
#define __SVG_PATH_H__

#include <parserSVG/Base.h>

namespace svg
{
	class Path : public svg::Base
	{
		private:
			
		public:
			Path(PaintState parentPaintState);
			~Path(void);
			virtual bool Parse(TiXmlNode * node);
			virtual void Display(int32_t spacing);
	};
};

#endif

