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

svg::Parser::Parser(etk::File fileName)
{
	m_fileName = fileName;
	m_version = "0.0";
	m_loadOK = false;
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
	fileName.fRead(fileBuffer, 1, fileSize);
	// close the file:
	fileName.fClose();
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
		// parse all sub node :
		for(TiXmlNode * child = root->FirstChild(); NULL != child; child = child->NextSibling() ) {
			svg::Base *elementParser = NULL;
			if (child->Type()==TiXmlNode::TINYXML_COMMENT) {
				// nothing to do, just proceed to next step
			} else {
				etk::UString localValue = child->Value();
				if (localValue == "g") {
					// group ... not supported now ...
				} else if (localValue == "a") {
					// TODO ...
				} else if (localValue == "path") {
					elementParser = new svg::Path();
				} else if (localValue == "rect") {
					elementParser = new svg::Rectangle();
				} else if (localValue == "circle") {
					elementParser = new svg::Circle();
				} else if (localValue == "ellipse") {
					elementParser = new svg::Ellipse();
				} else if (localValue == "line") {
					elementParser = new svg::Line();
				} else if (localValue == "polyline") {
					elementParser = new svg::Polyline();
				} else if (localValue == "polygon") {
					elementParser = new svg::Polygon();
				} else if (localValue == "text") {
					elementParser = new svg::Text();
				} else {
					SVG_ERROR("(l "<<child->Row()<<") node not suported : \""<<localValue<<"\" must be [g,a,path,rect,circle,ellipse,line,polyline,polygon,text]");
				}
				if (NULL == elementParser) {
					SVG_ERROR("(l "<<child->Row()<<") error on node: \""<<localValue<<"\" allocation error or not supported ...");
				} else {
					if (false == elementParser->Parse(child)) {
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


