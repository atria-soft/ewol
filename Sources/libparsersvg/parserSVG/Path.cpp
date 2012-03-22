/**
 *******************************************************************************
 * @file parserSVG/Path.cpp
 * @brief basic path parsing (Sources)
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

#include <parserSVG/Debug.h>
#include <parserSVG/Path.h>

svg::Path::Path(PaintState parentPaintState) : svg::Base(parentPaintState)
{
	
}

svg::Path::~Path(void)
{
	
}

bool svg::Path::Parse(TiXmlNode * node)
{
	ParseTransform(node);
	ParsePaintAttr(node);
	return false;
}

void svg::Path::Display(int32_t spacing)
{
	SVG_DEBUG(SpacingDist(spacing) << "Path");
}


