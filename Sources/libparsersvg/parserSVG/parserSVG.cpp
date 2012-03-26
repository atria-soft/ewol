/**
 *******************************************************************************
 * @file parserSVG/parserSVG.cpp
 * @brief parserSVG : basic header of the SVG parser (Sources)
 * @author Edouard DUPIN
 * @date 18/03/2012
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
#include <parserSVG/parserSVG.h>
#include <tinyXML/tinyxml.h>
#include <parserSVG/Base.h>
#include <parserSVG/Circle.h>
#include <parserSVG/Ellipse.h>
#include <parserSVG/Line.h>
#include <parserSVG/Path.h>
#include <parserSVG/Polygon.h>
#include <parserSVG/Polyline.h>
#include <parserSVG/Rectangle.h>
#include <parserSVG/Text.h>
#include <parserSVG/Group.h>

#include <agg-2.4/agg_basics.h>
#include <agg-2.4/agg_rendering_buffer.h>
#include <agg-2.4/agg_rasterizer_scanline_aa.h>
#include <agg-2.4/agg_scanline_p.h>
#include <agg-2.4/agg_renderer_scanline.h>
#include <agg-2.4/agg_path_storage.h>
#include <agg-2.4/agg_conv_transform.h>
#include <agg-2.4/agg_bounding_rect.h>
#include <agg-2.4/agg_color_rgba.h>
#include <agg-2.4/agg_pixfmt_rgba.h>

svg::Parser::Parser(etk::File fileName)
{
	m_fileName = fileName;
	m_version = "0.0";
	m_loadOK = false;
	m_paint.fill.red = 0xFF;
	m_paint.fill.green = 0;
	m_paint.fill.blue = 0;
	m_paint.fill.alpha = 0xFF;
	
	m_paint.stroke.red = 0xFF;
	m_paint.stroke.green = 0xFF;
	m_paint.stroke.blue = 0xFF;
	m_paint.stroke.alpha = 0;
	
	m_paint.strokeWidth = 0xFF;
	m_paint.viewPort.x = 0xFF;
	m_paint.viewPort.y = 0xFF;
	m_paint.flagEvenOdd = false;
	
	
	// Start loading the XML : 
	SVG_DEBUG("open file (SVG) \"" << m_fileName << "\"");

	// allocate the document in the stack
	TiXmlDocument XmlDocument;
	if (false == m_fileName.Exist()) {
		SVG_ERROR("File Does not exist : " << m_fileName);
		return;
	}
	int32_t fileSize = m_fileName.Size();
	if (0==fileSize) {
		SVG_ERROR("This file is empty : " << m_fileName);
		return;
	}
	if (false == m_fileName.fOpenRead()) {
		SVG_ERROR("Can not open the file : " << m_fileName);
		return;
	}
	// allocate data
	char * fileBuffer = new char[fileSize+5];
	if (NULL == fileBuffer) {
		SVG_ERROR("Error Memory allocation size=" << fileSize);
		return;
	}
	memset(fileBuffer, 0, (fileSize+5)*sizeof(char));
	// load data from the file :
	m_fileName.fRead(fileBuffer, 1, fileSize);
	// close the file:
	m_fileName.fClose();
	// load the XML from the memory
	XmlDocument.Parse((const char*)fileBuffer, 0, TIXML_ENCODING_UTF8);

	TiXmlElement* root = XmlDocument.FirstChildElement( "svg" );
	if (NULL == root ) {
		SVG_ERROR("(l ?) main node not find: \"svg\" in \"" << m_fileName << "\"");
	} else {
		// get the svg version :
		const char *version = root->ToElement()->Attribute("version");
		if (NULL != version) {
			m_version = version;
		}
		// parse ...
		coord2D_ts pos;
		coord2D_ts size;
		ParseTransform(root);
		ParsePosition(root, pos, size);
		ParsePaintAttr(root);
		SVG_VERBOSE("parsed .ROOT trans : (" << m_transformMatrix.sx << "," << m_transformMatrix.shy << "," << m_transformMatrix.shx << "," << m_transformMatrix.sy << "," << m_transformMatrix.tx << "," << m_transformMatrix.ty << ")");
		
		
		// parse all sub node :
		for(TiXmlNode * child = root->FirstChild(); NULL != child; child = child->NextSibling() ) {
			svg::Base *elementParser = NULL;
			if (child->Type()==TiXmlNode::TINYXML_COMMENT) {
				// nothing to do, just proceed to next step
			} else {
				etk::UString localValue = child->Value();
				if (localValue == "g") {
					elementParser = new svg::Group(m_paint);
				} else if (localValue == "a") {
					// TODO ...
				} else if (localValue == "path") {
					elementParser = new svg::Path(m_paint);
				} else if (localValue == "rect") {
					elementParser = new svg::Rectangle(m_paint);
				} else if (localValue == "circle") {
					elementParser = new svg::Circle(m_paint);
				} else if (localValue == "ellipse") {
					elementParser = new svg::Ellipse(m_paint);
				} else if (localValue == "line") {
					elementParser = new svg::Line(m_paint);
				} else if (localValue == "polyline") {
					elementParser = new svg::Polyline(m_paint);
				} else if (localValue == "polygon") {
					elementParser = new svg::Polygon(m_paint);
				} else if (localValue == "text") {
					elementParser = new svg::Text(m_paint);
				} else {
					SVG_ERROR("(l "<<child->Row()<<") node not suported : \""<<localValue<<"\" must be [g,a,path,rect,circle,ellipse,line,polyline,polygon,text]");
				}
				if (NULL == elementParser) {
					SVG_ERROR("(l "<<child->Row()<<") error on node: \""<<localValue<<"\" allocation error or not supported ...");
				} else {
					if (false == elementParser->Parse(child, m_transformMatrix)) {
						SVG_ERROR("(l "<<child->Row()<<") error on node: \""<<localValue<<"\" Sub Parsing ERROR");
						delete(elementParser);
						elementParser = NULL;
					} else {
						// add element in the system
						m_subElementList.PushBack(elementParser);
					}
				}
			}
		}
	}
	
	if (NULL != fileBuffer) {
		delete[] fileBuffer;
	}
}

svg::Parser::~Parser(void)
{
	
}



void svg::Parser::DisplayDebug(void)
{
	SVG_DEBUG("Main SVG node : ");
	for (int32_t iii=0; iii<m_subElementList.Size(); iii++) {
		if (NULL != m_subElementList[iii]) {
			m_subElementList[iii]->Display(1);
		}
	}
}


void svg::Parser::AggDraw(svg::Renderer& myRenderer, agg::trans_affine& basicTrans)
{
	for (int32_t iii=0; iii<m_subElementList.Size(); iii++) {
		if (NULL != m_subElementList[iii]) {
			m_subElementList[iii]->AggDraw(myRenderer, basicTrans);
		}
	}
}


void svg::Parser::GenerateTestFile(void)
{
	float coefmult = 2;
	int width = 800*coefmult;
	int height = 600*coefmult;
	
	svg::Renderer * myRenderer = new svg::Renderer(width, height);
	// create the first element matrix modification ...
	agg::trans_affine basicTrans;
	//basicTrans *= agg::trans_affine_translation(-g_base_dx, -g_base_dy);
	basicTrans *= agg::trans_affine_scaling(2, 2);
	//basicTrans *= agg::trans_affine_rotation(g_angle);// + agg::pi);
	//basicTrans *= agg::trans_affine_skewing(2.0, 5.0);
	//basicTrans *= agg::trans_affine_translation(width*0.3, height/2);
	basicTrans *= agg::trans_affine_translation(width/2, height/2);
	
	
	AggDraw(*myRenderer, basicTrans);
	etk::UString tmpFileOut = "yyy_out_";
	tmpFileOut += m_fileName.GetShortFilename();
	tmpFileOut += ".ppm";
	myRenderer->WritePpm(tmpFileOut);
	
}



