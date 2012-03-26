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


// return the next char position ... (after 'X' or NULL)
const char * extractCmd(const char * input, char& cmd, etk::VectorType<float>& outputList)
{
	if (*input == '\0') {
		return NULL;
	}
	outputList.Clear();
	cmd = '\0';
	const char * outputPointer = NULL;
	if (!( (input[0] <= 'Z' && input[0] >= 'A') || (input[0] <= 'z' && input[0] >= 'a') ) ) {
		SVG_ERROR("Error in the SVG Path : \"" << input << "\"");
		return NULL;
	}
	cmd = input[0];
	SVG_DEBUG("Find command : " << cmd);
	if (input[1] == '\0') {
		return &input[1];
	}
	int32_t iii=1;
	// extract every float separated by a ' ' or a ','
	float element;
	char spacer[10];
	int32_t nbElementRead;
	while(sscanf(&input[iii], "%1[, ]%f%n", spacer, &element, &nbElementRead) == 2) {
		SVG_DEBUG("Find element : " << element);
		outputList.PushBack(element);
		iii += nbElementRead;
	}
	outputPointer = &input[iii];
	while(*outputPointer!= '\0' && *outputPointer == ' ') {
		outputPointer++;
	}
	//outputPointer++;
	return outputPointer;
}

bool svg::Path::Parse(TiXmlNode * node, agg::trans_affine& parentTrans)
{
	ParseTransform(node);
	ParsePaintAttr(node);
	
	// add the property of the parrent modifications ...
	m_transformMatrix *= parentTrans;
	
	//m_listElement
	
	const char *elementXML = node->ToElement()->Attribute("d");
	if (NULL == elementXML) {
		SVG_ERROR("(l "<<node->Row()<<") path: missing 'p' attribute");
		return false;
	}
	SVG_DEBUG("Parse Path : \"" << elementXML << "\"");
	
	coord2D_ts currentPos;
	currentPos.x = 0;
	currentPos.y = 0;
	
	char command;
	etk::VectorType<float> listDot;
	
	for( const char *sss=extractCmd(elementXML, command, listDot);
	     NULL != sss;
	     sss=extractCmd(sss, command, listDot) ) {
		
		// later ...
	}
	
	return true;
}

void svg::Path::Display(int32_t spacing)
{
	SVG_DEBUG(SpacingDist(spacing) << "Path");
}


