/**
 *******************************************************************************
 * @file parserSVG/Base.cpp
 * @brief basic Element parsing (Sources)
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
#include <parserSVG/Base.h>
#include <math.h>

svg::Base::Base(paintState_ts parentPaintState)
{
	// copy the parent painting properties ...
	m_paint = parentPaintState;
}

/**
 * @brief Parse the transform balise C String.
 * @param[in] inputString String data inside the transform attribute
 * @param[in,out] baseMatrice matrice that must be update
 * @return ---
 */
void svg::Base::ParseTransform(TiXmlNode *node)
{
	SVG_CHECK_INOUT(node);
	const char * inputString = (char*)node->ToElement()->Attribute("transform");
	if (NULL == inputString) {
		return;
	}
	char tmpData[2048];
	for (int32_t iii=0; inputString[iii]=='\0' && iii<2047; iii++) {
		if (inputString[iii] == ',') {
			tmpData[iii] = ' ';
		} else {
			tmpData[iii] = inputString[iii];
		}
		// end of the string ...
		tmpData[iii+1] = '\0';
	}
	etkFloat_t base[6];
	etkFloat_t zzz[6];
	etkFloat_t angle, xxx, yyy;
	int32_t n;
	char * pointerOnData = tmpData;
	while (*pointerOnData) {
		base[0] = 1.0;
		base[1] = 0.0;
		base[2] = 0.0;
		base[3] = 1.0;
		base[4] = 0.0;
		base[5] = 0.0;
		if (sscanf(pointerOnData, "matrix (%f %f %f %f %f %f) %n", &base[0], &base[1], &base[2], &base[3], &base[4], &base[5], &n) == 6) {
			// nothing to do ...
		} else if (sscanf(pointerOnData, "translate (%f %f) %n", &base[4], &base[5], &n) == 2) {
			// nothing to do ...
		} else if (sscanf(pointerOnData, "translate (%f) %n", &base[4], &n) == 1) {
			// nothing to do ...
		} else if (sscanf(pointerOnData, "scale (%f %f) %n", &base[0], &base[3], &n) == 2) {
			// nothing to do ...
		} else if (sscanf(pointerOnData, "scale (%f) %n", &base[0], &n) == 1) {
			base[3] = base[0];
		} else if (sscanf(pointerOnData, "rotate (%f %f %f) %n", &angle, &xxx, &yyy, &n) == 3) {
			angle = angle / 180 * M_PI;
			base[0] = cos(angle);
			base[1] = sin(angle);
			base[2] = -sin(angle);
			base[3] = cos(angle);
			base[4] = -xxx * cos(angle) + yyy * sin(angle) + xxx;
			base[5] = -xxx * sin(angle) - yyy * cos(angle) + yyy;
		} else if (sscanf(pointerOnData, "rotate (%f) %n", &angle, &n) == 1) {
			angle = angle / 180 * M_PI;
			base[0] = cos(angle);
			base[1] = sin(angle);
			base[2] = -sin(angle);
			base[3] = cos(angle);
		} else if (sscanf(pointerOnData, "skewX (%f) %n", &angle, &n) == 1) {
			angle = angle / 180 * M_PI;
			base[2] = tan(angle);
		} else if (sscanf(pointerOnData, "skewY (%f) %n", &angle, &n) == 1) {
			angle = angle / 180 * M_PI;
			base[1] = tan(angle);
		} else {
			break;
		}
		zzz[0] = m_paint.matrix[0]*base[0] + m_paint.matrix[2]*base[1];
		zzz[1] = m_paint.matrix[1]*base[0] + m_paint.matrix[3]*base[1];
		zzz[2] = m_paint.matrix[0]*base[2] + m_paint.matrix[2]*base[3];
		zzz[3] = m_paint.matrix[1]*base[2] + m_paint.matrix[3]*base[3];
		zzz[4] = m_paint.matrix[0]*base[4] + m_paint.matrix[2]*base[5] + m_paint.matrix[4];
		zzz[5] = m_paint.matrix[1]*base[4] + m_paint.matrix[3]*base[5] + m_paint.matrix[5];
		memcpy(m_paint.matrix, zzz, sizeof(etkFloat_t) * 6 );
		pointerOnData += n;
	}
}


/**
 * @brief Parse x, y, width, height attribute of the xml node
 * @param[in] node XML node
 * @param[out] pos parsed position
 * @param[out] size parsed dimention
 * @return ---
 */
void svg::Base::ParsePosition(const TiXmlNode *node, coord2D_ts &pos, coord2D_ts &size)
{
	pos.x = 0;
	pos.y = 0;
	size.x = m_paint.viewPort.x;
	size.y = m_paint.viewPort.y;

	const char * content = node->ToElement()->Attribute("x");
	if (NULL != content) {
		pos.x = ParseLength(content);
	}
	content = node->ToElement()->Attribute("y");
	if (NULL != content) {
		pos.y = ParseLength(content);
	}
	content = node->ToElement()->Attribute("width");
	if (NULL != content) {
		size.x = ParseLength(content);
	}
	content = node->ToElement()->Attribute("height");
	if (NULL != content) {
		size.y = ParseLength(content);
	}
}


/**
 * @brief Parse a lenght of the xml element
 * @param[in] dataInput Data C String with the printed lenght
 * @return standart number of pixels
 */
etkFloat_t svg::Base::ParseLength(const char *dataInput)
{
	int32_t numLength = strspn(dataInput, "0123456789+-.");
	const char *unit = dataInput + numLength;
	etkFloat_t n = atof(dataInput);
	etkFloat_t font_size = 20.0;

	if (unit[0] == '\0') {
		return n;
	} else if (unit[0] == '%') {                   // xxx %
		return n / 100.0 * m_paint.viewPort.x;
	} else if (unit[0] == 'e' && unit[1] == 'm') { // xxx em
		return n * font_size;
	} else if (unit[0] == 'e' && unit[1] == 'x') { // xxx ex
		return n / 2.0 * font_size;
	} else if (unit[0] == 'p' && unit[1] == 'x') { // xxx px
		return n;
	} else if (unit[0] == 'p' && unit[1] == 't') { // xxx pt
		return n * 1.25;
	} else if (unit[0] == 'p' && unit[1] == 'c') { // xxx pc
		return n * 15.0;
	} else if (unit[0] == 'm' && unit[1] == 'm') { // xxx mm
		return n * 3.543307;
	} else if (unit[0] == 'c' && unit[1] == 'm') { // xxx cm
		return n * 35.43307;
	} else if (unit[0] == 'i' && unit[1] == 'n') { // xxx in
		return n * 90;
	}
	return 0;
}


/**
 * @brief Parse a Painting attribute of a specific node
 * @param[in] node : basic node of the XML that might be parsed
 * @return ---
 */
void svg::Base::ParsePaintAttr(const TiXmlNode *node)
{
	const char * content = node->ToElement()->Attribute("fill");
	if (NULL != content) {
		m_paint.fill = ParseColor(content);
	}
	content = node->ToElement()->Attribute("stroke");
	if (NULL != content) {
		m_paint.stroke = ParseColor(content);
	}
	content = node->ToElement()->Attribute("stroke-width");
	if (NULL != content) {
		m_paint.strokeWidth = ParseLength(content);
	}
	content = node->ToElement()->Attribute("style");
	if (NULL != content) {
		const char *sss;
		if ((sss = strstr(content, "fill:"))) {
			sss += 5;
			while(    sss[0] ==' '
			       && sss[0]!='\0' ) {
				sss++;
			}
			m_paint.fill = ParseColor(sss);
		}
		if ((sss = strstr(content, "stroke:"))) {
			sss += 7;
			while(    sss[0] ==' '
			       && sss[0]!='\0' ) {
				sss++;
			}
			m_paint.stroke = ParseColor(sss);
		}
		if ((sss = strstr(content, "stroke-width:"))) {
			sss += 13;
			while(    sss[0] ==' '
			       && sss[0]!='\0' ) {
				sss++;
			}
			m_paint.strokeWidth = ParseLength(sss);
		}
	}
}

/**
 * @brief Parse a color specification from the svg file
 * @param[in] inputData Data C String with the xml definition
 * @return the parsed color
 */
color_ts svg::Base::ParseColor(const char *inputData)
{
	color_ts localColor;
	localColor.red  =1.0;
	localColor.green=1.0;
	localColor.blue =1.0;
	localColor.alpha=1.0;
	
	uint32_t red, green, blue;
	size_t len = strlen(inputData);

	if(    len == 4
	    && inputData[0] == '#') {
		if (sscanf(inputData + 1, "%1x%1x%1x", &red, &green, &blue) == 3) {
			localColor.red  = (etkFloat_t)(red | red << 4) / 256.0;
			localColor.green= (etkFloat_t)(green | green << 4) / 256.0;
			localColor.blue = (etkFloat_t)(blue | blue << 4) / 256.0;
		}
	} else if(    len == 7
	           && inputData[0] == '#') {
		if (sscanf(inputData + 1, "%2x%2x%2x", &red, &green, &blue) == 3) {
			localColor.red  = (etkFloat_t)(red) / 256.0;
			localColor.green= (etkFloat_t)(green) / 256.0;
			localColor.blue = (etkFloat_t)(blue) / 256.0;
		}
	} else if(    10 <= len
	           && inputData[0] == 'r'
	           && inputData[1] == 'g'
	           && inputData[2] == 'b'
	           && inputData[3] == '('
	           && inputData[len - 1] == ')') {
		if (sscanf(inputData + 4, "%u,%u,%u", &red, &green, &blue) == 3) {
			localColor.red  = (etkFloat_t)(red) / 256.0;
			localColor.green= (etkFloat_t)(green) / 256.0;
			localColor.blue = (etkFloat_t)(blue) / 256.0;
		} else if (sscanf(inputData + 4, "%f%%,%f%%,%f%%", &localColor.red, &localColor.green, &localColor.blue) == 3) {
			// nothing to do ...
		}
	} else if(    len == 4
	           && strcmp(inputData, "none") == 0) {
		localColor.alpha=0.0;
	} else if(    5 < len
	           && inputData[0] == 'u'
	           && inputData[1] == 'r'
	           && inputData[2] == 'l'
	           && inputData[3] == '(') {
		if (inputData[4] == '#') {
			// TODO : parse gradient ...
		}
	} else {
		// TODO : Might be a named color ... need to find it ...
	}
	return localColor;
}


/**
 * @brief Parse all the element needed in the basic node
 * @param[in] node standart XML node
 * @return true if no problem arrived
 */
bool svg::Base::Parse(TiXmlNode * node)
{
	return false;
}


const char * svg::Base::SpacingDist(int32_t spacing)
{
	static const char *tmpValue = "                                                                                ";
	if (spacing>20) {
		spacing = 20;
	}
	return tmpValue + 20*4 - spacing*4;
}