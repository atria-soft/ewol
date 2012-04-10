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

svg::Base::Base(PaintState parentPaintState)
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
	SVG_VERBOSE("find transform : \"" << inputString << "\"");
	char tmpData[2048];
	for (int32_t iii=0; inputString[iii]!='\0' && iii<2047; iii++) {
		if (inputString[iii] == ',') {
			tmpData[iii] = ' ';
		} else {
			tmpData[iii] = inputString[iii];
		}
		// end of the string ...
		tmpData[iii+1] = '\0';
	}
	SVG_VERBOSE("find transform : \"" << tmpData << "\"");
	double matrix[6];
	float angle, xxx, yyy;
	int32_t n;
	char * pointerOnData = tmpData;
	while (*pointerOnData) {
		if (sscanf(pointerOnData, "matrix (%lf %lf %lf %lf %lf %lf) %n", &matrix[0], &matrix[1], &matrix[2], &matrix[3], &matrix[4], &matrix[5], &n) == 6) {
			m_transformMatrix.load_from(matrix);
		} else if (sscanf(pointerOnData, "translate (%f %f) %n", &xxx, &yyy, &n) == 2) {
			m_transformMatrix *= agg::trans_affine_translation(xxx, yyy);
			SVG_VERBOSE("Translate : " << xxx << ", " << yyy);
		} else if (sscanf(pointerOnData, "translate (%f) %n", &xxx, &n) == 1) {
			m_transformMatrix *= agg::trans_affine_translation(xxx, 0);
			SVG_VERBOSE("Translate : " << xxx << ", " << 0);
		} else if (sscanf(pointerOnData, "scale (%f %f) %n", &xxx, &yyy, &n) == 2) {
			m_transformMatrix *= agg::trans_affine_scaling(xxx, yyy);
			SVG_VERBOSE("Scale : " << xxx << ", " << yyy);
		} else if (sscanf(pointerOnData, "scale (%f) %n", &xxx, &n) == 1) {
			m_transformMatrix *= agg::trans_affine_scaling(xxx, xxx);
			SVG_VERBOSE("Scale : " << xxx << ", " << xxx);
		} else if (sscanf(pointerOnData, "rotate (%f %f %f) %n", &angle, &xxx, &yyy, &n) == 3) {
			angle = angle / 180 * M_PI;
			m_transformMatrix *= agg::trans_affine_translation(-xxx, -yyy);
			m_transformMatrix *= agg::trans_affine_rotation(angle);
			m_transformMatrix *= agg::trans_affine_translation(xxx, yyy);
		} else if (sscanf(pointerOnData, "rotate (%f) %n", &angle, &n) == 1) {
			angle = angle / 180 * M_PI;
			SVG_VERBOSE("rotate : " << angle << "rad, " << (angle/M_PI*180) << "°");
			m_transformMatrix *= agg::trans_affine_rotation(angle);
		} else if (sscanf(pointerOnData, "skewX (%f) %n", &angle, &n) == 1) {
			angle = angle / 180 * M_PI;
			SVG_VERBOSE("skewX : " << angle << "rad, " << (angle/M_PI*180) << "\Uffffffff");
			m_transformMatrix *= agg::trans_affine_skewing(angle, 0.0);
		} else if (sscanf(pointerOnData, "skewY (%f) %n", &angle, &n) == 1) {
			angle = angle / 180 * M_PI;
			SVG_VERBOSE("skewY : " << angle << "rad, " << (angle/M_PI*180) << "\Uffffffff");
			m_transformMatrix *= agg::trans_affine_skewing(0.0, angle);
		} else {
			break;
		}
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
	size.x = 0;
	size.y = 0;

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
	//SVG_INFO("          ==> \"" << dataInput << "\"");
	etkFloat_t n = atof(dataInput);
	//SVG_INFO("          ==> ?? = " << n );
	etkFloat_t font_size = 20.0;
	
	// note : ";" is for the parsing of the style elements ...
	if (unit[0] == '\0' || unit[0] == ';' ) {
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

// return the next char position ... (after ';' or NULL)
const char * extractPartOfStyle(const char * input, char * outputType, char * outputData, int32_t maxLen)
{
	if (*input == '\0') {
		return NULL;
	}
	int32_t jjj = 0;
	const char * outputPointer = NULL;
	outputType[maxLen-1] = '\0';
	outputType[0] = '\0';
	outputData[maxLen-1] = '\0';
	outputData[0] = '\0';
	char * output = outputType;
	for( int32_t iii=0; iii<maxLen-1 && input[iii]!='\0'; iii++) {
		outputPointer = &input[iii];
		if (input[iii] != ';') {
			if (input[iii] == ' ') {
				// nothing to do ... we do not copy espaces ...
			} else if (input[iii] == ':') {
				// change the output ...
				output = outputData;
				jjj = 0;
			} else {
				output[jjj] = input[iii];
				output[jjj+1] = '\0';
				jjj++;
			}
		} else {
			break;
		}
	}
	outputPointer++;
	return outputPointer;
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
	content = node->ToElement()->Attribute("opacity");
	if (NULL != content) {
		etkFloat_t opacity = ParseLength(content);
		opacity  = etk_max(0.0, etk_min(1.0, opacity));
		m_paint.fill.alpha = opacity*0xFF;
		m_paint.stroke.alpha = opacity*0xFF;
	}
	content = node->ToElement()->Attribute("fill-opacity");
	if (NULL != content) {
		etkFloat_t opacity = ParseLength(content);
		opacity  = etk_max(0.0, etk_min(1.0, opacity));
		m_paint.fill.alpha = opacity*0xFF;
	}
	content = node->ToElement()->Attribute("stroke-opacity");
	if (NULL != content) {
		etkFloat_t opacity = ParseLength(content);
		opacity  = etk_max(0.0, etk_min(1.0, opacity));
		m_paint.stroke.alpha = opacity*0xFF;
	}
	content = node->ToElement()->Attribute("fill-rule");
	if (NULL != content) {
		if (0 == strcmp(content, "nonzero") ) {
			m_paint.flagEvenOdd = false;
		} else if (0 == strcmp(content, "evenodd") ) {
			m_paint.flagEvenOdd = true;
		} else {
			SVG_ERROR("not know fill-rule value : \"" << content << "\", not in [nonzero,evenodd]");
		}
	}
	content = node->ToElement()->Attribute("stroke-linecap");
	if (NULL != content) {
		if (0 == strcmp(content, "butt") ) {
			m_paint.lineCap = svg::LINECAP_BUTT;
		} else if (0 == strcmp(content, "round") ) {
			m_paint.lineCap = svg::LINECAP_ROUND;
		} else if (0 == strcmp(content, "square") ) {
			m_paint.lineCap = svg::LINECAP_SQUARE;
		} else {
			m_paint.lineCap = svg::LINECAP_BUTT;
			SVG_ERROR("not know stroke-linecap value : \"" << content << "\", not in [butt,round,square]");
		}
	}
	content = node->ToElement()->Attribute("stroke-linejoin");
	if (NULL != content) {
		if (0 == strcmp(content, "miter") ) {
			m_paint.lineJoin = svg::LINEJOIN_MITER;
		} else if (0 == strcmp(content, "round") ) {
			m_paint.lineJoin = svg::LINEJOIN_ROUND;
		} else if (0 == strcmp(content, "bevel") ) {
			m_paint.lineJoin = svg::LINEJOIN_BEVEL;
		} else {
			m_paint.lineJoin = svg::LINEJOIN_MITER;
			SVG_ERROR("not know stroke-linejoin value : \"" << content << "\", not in [miter,round,bevel]");
		}
	}
	content = node->ToElement()->Attribute("style");
	if (NULL != content) {
		char outputType[1024] = "";
		char outputValue[1024] = "";
		
		for( const char *sss=extractPartOfStyle(content, outputType, outputValue, 1024);
		     NULL != sss;
		     sss=extractPartOfStyle(sss, outputType, outputValue, 1024) ) {
			//SVG_INFO(" style parse : \"" << outputType << "\" with value : \"" << outputValue << "\"");
			if (0 == strcmp(outputType, "fill") ) {
				m_paint.fill = ParseColor(outputValue);
			} else if (0 == strcmp(outputType, "stroke") ) {
				m_paint.stroke = ParseColor(outputValue);
			} else if (0 == strcmp(outputType, "stroke-width") ) {
				m_paint.strokeWidth = ParseLength(outputValue);
				//SVG_ERROR(" input : \"" << outputValue << "\" ==> " <<m_paint.strokeWidth);
			} else if (0 == strcmp(outputType, "opacity") ) {
				etkFloat_t opacity = ParseLength(outputValue);
				opacity  = etk_max(0.0, etk_min(1.0, opacity));
				m_paint.fill.alpha = opacity*0xFF;
				m_paint.stroke.alpha = opacity*0xFF;
			} else if (0 == strcmp(outputType, "fill-opacity") ) {
				etkFloat_t opacity = ParseLength(outputValue);
				opacity  = etk_max(0.0, etk_min(1.0, opacity));
				m_paint.fill.alpha = opacity*0xFF;
			} else if (0 == strcmp(outputType, "stroke-opacity") ) {
				etkFloat_t opacity = ParseLength(outputValue);
				opacity  = etk_max(0.0, etk_min(1.0, opacity));
				m_paint.stroke.alpha = opacity*0xFF;
			} else if (0 == strcmp(outputType, "fill-rule") ) {
				if (0 == strcmp(outputValue, "nonzero") ) {
					m_paint.flagEvenOdd = false;
				} else if (0 == strcmp(outputValue, "evenodd") ) {
					m_paint.flagEvenOdd = true;
				} else {
					SVG_ERROR("not know  " << outputType << " value : \"" << outputValue << "\", not in [nonzero,evenodd]");
				}
			} else if (0 == strcmp(outputType, "stroke-linecap") ) {
				if (0 == strcmp(outputValue, "butt") ) {
					m_paint.lineCap = svg::LINECAP_BUTT;
				} else if (0 == strcmp(outputValue, "round") ) {
					m_paint.lineCap = svg::LINECAP_ROUND;
				} else if (0 == strcmp(outputValue, "square") ) {
					m_paint.lineCap = svg::LINECAP_SQUARE;
				} else {
					m_paint.lineCap = svg::LINECAP_BUTT;
					SVG_ERROR("not know  " << outputType << " value : \"" << outputValue << "\", not in [butt,round,square]");
				}
			} else if (0 == strcmp(outputType, "stroke-linejoin") ) {
				if (0 == strcmp(outputValue, "miter") ) {
					m_paint.lineJoin = svg::LINEJOIN_MITER;
				} else if (0 == strcmp(outputValue, "round") ) {
					m_paint.lineJoin = svg::LINEJOIN_ROUND;
				} else if (0 == strcmp(outputValue, "bevel") ) {
					m_paint.lineJoin = svg::LINEJOIN_BEVEL;
				} else {
					m_paint.lineJoin = svg::LINEJOIN_MITER;
					SVG_ERROR("not know  " << outputType << " value : \"" << outputValue << "\", not in [miter,round,bevel]");
				}
			} else if (0 == strcmp(outputType, "marker-start") ) {
				// TODO : ...
			} else {
				SVG_ERROR("not know painting element in style balise : \"" << outputType << "\" with value : \"" << outputValue << "\"");
			}
		}
	}
}
typedef struct {
	const char * colorName;
	uint32_t color;
} colorList_ts;

const colorList_ts listOfColor[] = {
	{ "none",				0x00000000},
	{ "AliceBlue",			0xF0F8FFFF},
	{ "AntiqueWhite",		0xFAEBD7FF},
	{ "Aqua",				0x00FFFFFF},
	{ "Aquamarine",			0x7FFFD4FF},
	{ "Azure",				0xF0FFFFFF},
	{ "Beige",				0xF5F5DCFF},
	{ "Bisque",				0xFFE4C4FF},
	{ "Black",				0x000000FF},
	{ "BlanchedAlmond",		0xFFEBCDFF},
	{ "Blue",				0x0000FFFF},
	{ "BlueViolet",			0x8A2BE2FF},
	{ "Brown",				0xA52A2AFF},
	{ "BurlyWood",			0xDEB887FF},
	{ "CadetBlue",			0x5F9EA0FF},
	{ "Chartreuse",			0x7FFF00FF},
	{ "Chocolate",			0xD2691EFF},
	{ "Coral",				0xFF7F50FF},
	{ "CornflowerBlue",		0x6495EDFF},
	{ "Cornsilk",			0xFFF8DCFF},
	{ "Crimson",			0xDC143CFF},
	{ "Cyan",				0x00FFFFFF},
	{ "DarkBlue",			0x00008BFF},
	{ "DarkCyan",			0x008B8BFF},
	{ "DarkGoldenRod",		0xB8860BFF},
	{ "DarkGray",			0xA9A9A9FF},
	{ "DarkGrey",			0xA9A9A9FF},
	{ "DarkGreen",			0x006400FF},
	{ "DarkKhaki",			0xBDB76BFF},
	{ "DarkMagenta",		0x8B008BFF},
	{ "DarkOliveGreen",		0x556B2FFF},
	{ "Darkorange",			0xFF8C00FF},
	{ "DarkOrchid",			0x9932CCFF},
	{ "DarkRed",			0x8B0000FF},
	{ "DarkSalmon",			0xE9967AFF},
	{ "DarkSeaGreen",		0x8FBC8FFF},
	{ "DarkSlateBlue",		0x483D8BFF},
	{ "DarkSlateGray",		0x2F4F4FFF},
	{ "DarkSlateGrey",		0x2F4F4FFF},
	{ "DarkTurquoise",		0x00CED1FF},
	{ "DarkViolet",			0x9400D3FF},
	{ "DeepPink",			0xFF1493FF},
	{ "DeepSkyBlue",		0x00BFFFFF},
	{ "DimGray",			0x696969FF},
	{ "DimGrey",			0x696969FF},
	{ "DodgerBlue",			0x1E90FFFF},
	{ "FireBrick",			0xB22222FF},
	{ "FloralWhite",		0xFFFAF0FF},
	{ "ForestGreen",		0x228B22FF},
	{ "Fuchsia",			0xFF00FFFF},
	{ "Gainsboro",			0xDCDCDCFF},
	{ "GhostWhite",			0xF8F8FFFF},
	{ "Gold",				0xFFD700FF},
	{ "GoldenRod",			0xDAA520FF},
	{ "Gray",				0x808080FF},
	{ "Grey",				0x808080FF},
	{ "Green",				0x008000FF},
	{ "GreenYellow",		0xADFF2FFF},
	{ "HoneyDew",			0xF0FFF0FF},
	{ "HotPink",			0xFF69B4FF},
	{ "IndianRed ",			0xCD5C5CFF},
	{ "Indigo ",			0x4B0082FF},
	{ "Ivory",				0xFFFFF0FF},
	{ "Khaki",				0xF0E68CFF},
	{ "Lavender",			0xE6E6FAFF},
	{ "LavenderBlush",		0xFFF0F5FF},
	{ "LawnGreen",			0x7CFC00FF},
	{ "LemonChiffon",			0xFFFACDFF},
	{ "LightBlue",				0xADD8E6FF},
	{ "LightCoral",				0xF08080FF},
	{ "LightCyan",				0xE0FFFFFF},
	{ "LightGoldenRodYellow",	0xFAFAD2FF},
	{ "LightGray",				0xD3D3D3FF},
	{ "LightGrey",				0xD3D3D3FF},
	{ "LightGreen",				0x90EE90FF},
	{ "LightPink",				0xFFB6C1FF},
	{ "LightSalmon",			0xFFA07AFF},
	{ "LightSeaGreen",			0x20B2AAFF},
	{ "LightSkyBlue",			0x87CEFAFF},
	{ "LightSlateGray",			0x778899FF},
	{ "LightSlateGrey",			0x778899FF},
	{ "LightSteelBlue",			0xB0C4DEFF},
	{ "LightYellow",			0xFFFFE0FF},
	{ "Lime",				0x00FF00FF},
	{ "LimeGreen",			0x32CD32FF},
	{ "Linen",				0xFAF0E6FF},
	{ "Magenta",			0xFF00FFFF},
	{ "Maroon",				0x800000FF},
	{ "MediumAquaMarine",	0x66CDAAFF},
	{ "MediumBlue",			0x0000CDFF},
	{ "MediumOrchid",		0xBA55D3FF},
	{ "MediumPurple",		0x9370D8FF},
	{ "MediumSeaGreen",		0x3CB371FF},
	{ "MediumSlateBlue",	0x7B68EEFF},
	{ "MediumSpringGreen",	0x00FA9AFF},
	{ "MediumTurquoise",	0x48D1CCFF},
	{ "MediumVioletRed",	0xC71585FF},
	{ "MidnightBlue",		0x191970FF},
	{ "MintCream",			0xF5FFFAFF},
	{ "MistyRose",			0xFFE4E1FF},
	{ "Moccasin",			0xFFE4B5FF},
	{ "NavajoWhite",		0xFFDEADFF},
	{ "Navy",				0x000080FF},
	{ "OldLace",			0xFDF5E6FF},
	{ "Olive",				0x808000FF},
	{ "OliveDrab",			0x6B8E23FF},
	{ "Orange",				0xFFA500FF},
	{ "OrangeRed",			0xFF4500FF},
	{ "Orchid",				0xDA70D6FF},
	{ "PaleGoldenRod",		0xEEE8AAFF},
	{ "PaleGreen",			0x98FB98FF},
	{ "PaleTurquoise",		0xAFEEEEFF},
	{ "PaleVioletRed",		0xD87093FF},
	{ "PapayaWhip",			0xFFEFD5FF},
	{ "PeachPuff",			0xFFDAB9FF},
	{ "Peru",				0xCD853FFF},
	{ "Pink",				0xFFC0CBFF},
	{ "Plum",				0xDDA0DDFF},
	{ "PowderBlue",			0xB0E0E6FF},
	{ "Purple",				0x800080FF},
	{ "Red",				0xFF0000FF},
	{ "RosyBrown",			0xBC8F8FFF},
	{ "RoyalBlue",			0x4169E1FF},
	{ "SaddleBrown",		0x8B4513FF},
	{ "Salmon",				0xFA8072FF},
	{ "SandyBrown",			0xF4A460FF},
	{ "SeaGreen",			0x2E8B57FF},
	{ "SeaShell",			0xFFF5EEFF},
	{ "Sienna",				0xA0522DFF},
	{ "Silver",				0xC0C0C0FF},
	{ "SkyBlue",			0x87CEEBFF},
	{ "SlateBlue",			0x6A5ACDFF},
	{ "SlateGray",			0x708090FF},
	{ "SlateGrey",			0x708090FF},
	{ "Snow",				0xFFFAFAFF},
	{ "SpringGreen",		0x00FF7FFF},
	{ "SteelBlue",			0x4682B4FF},
	{ "Tan",				0xD2B48CFF},
	{ "Teal",				0x008080FF},
	{ "Thistle",			0xD8BFD8FF},
	{ "Tomato",				0xFF6347FF},
	{ "Turquoise",			0x40E0D0FF},
	{ "Violet",				0xEE82EEFF},
	{ "Wheat",				0xF5DEB3FF},
	{ "White",				0xFFFFFFFF},
	{ "WhiteSmoke",			0xF5F5F5FF},
	{ "Yellow",				0xFFFF00FF},
	{ "YellowGreen",		0x9ACD32FF}
};
const int32_t listOfColorSize = sizeof(listOfColor) / sizeof(colorList_ts);

bool strnCmpNoCase(const char * input1, const char * input2, int32_t maxLen)
{
	int32_t iii=0;
	while ('\0' != *input1 && '\0' != *input2 && iii < maxLen) {
		char in1 = *input1;
		char in2 = *input2;
		if (in1 != in2) {
			if (in1 <= 'Z' && in1 >= 'A') {
				in1 = in1 - 'A' + 'a';
			}
			if (in2 <= 'Z' && in2 >= 'A') {
				in2 = in2 - 'A' + 'a';
			}
			if (in1 != in2) {
				return false;
			}
		}
		iii++;
		input1++;
		input2++;
	}
	return true;
}


/**
 * @brief Parse a color specification from the svg file
 * @param[in] inputData Data C String with the xml definition
 * @return the parsed color
 */
color8_ts svg::Base::ParseColor(const char *inputData)
{
	color8_ts localColor;
	localColor.red  =0xFF;
	localColor.green=0xFF;
	localColor.blue =0xFF;
	localColor.alpha=0xFF;
	
	uint32_t red, green, blue, alpha;
	float fred, fgreen, fblue, falpha;
	size_t len = strlen(inputData);
	
	if(    len >=1
	    && inputData[0] == '#') {
		if(len == 4) {
			if (sscanf(inputData + 1, "%1x%1x%1x", &red, &green, &blue) == 3) {
				localColor.red  = (red | red << 4);
				localColor.green= (green | green << 4);
				localColor.blue = (blue | blue << 4);
			} else {
				SVG_ERROR(" pb in parsing the color : \"" << inputData << "\"");
			}
		} else if (len==5) {
			if (sscanf(inputData + 1, "%1x%1x%1x%1x", &red, &green, &blue, &alpha) == 4) {
				localColor.red   = (red | red << 4);
				localColor.green = (green | green << 4);
				localColor.blue  = (blue | blue << 4);
				localColor.alpha = (alpha | alpha << 4);
			} else {
				SVG_ERROR(" pb in parsing the color : \"" << inputData << "\"");
			}
		} else if (len == 7) {
			if (sscanf(inputData + 1, "%2x%2x%2x", &red, &green, &blue) == 3) {
				localColor.red  = red;
				localColor.green= green;
				localColor.blue = blue;
			} else {
				SVG_ERROR(" pb in parsing the color : \"" << inputData << "\"");
			}
		} else if (len == 9) {
			if (sscanf(inputData + 1, "%2x%2x%2x%2x", &red, &green, &blue, &alpha) == 4) {
				localColor.red  = red;
				localColor.green= green;
				localColor.blue = blue;
				localColor.alpha = alpha;
			} else {
				SVG_ERROR(" pb in parsing the color : \"" << inputData << "\"");
			}
		} else {
			SVG_ERROR(" pb in parsing the color : \"" << inputData << "\" ==> unknown methode ...");
		}
	} else if(    4 <= len
	           && inputData[0] == 'r'
	           && inputData[1] == 'g'
	           && inputData[2] == 'b'
	           && inputData[3] == '(' ) {
		if (sscanf(inputData + 4, "%u,%u,%u,%u", &red, &green, &blue, &alpha) == 4) {
			localColor.red  = etk_min(0xFF, red);
			localColor.green= etk_min(0xFF, green);
			localColor.blue = etk_min(0xFF, blue);
			localColor.alpha = etk_min(0xFF, alpha);
		} else if (sscanf(inputData + 4, "%u,%u,%u", &red, &green, &blue) == 3) {
			localColor.red  = etk_min(0xFF, red);
			localColor.green= etk_min(0xFF, green);
			localColor.blue = etk_min(0xFF, blue);
		} else if (sscanf(inputData + 4, "%f%%,%f%%,%f%%,%f%%", &fred, &fgreen, &fblue, &falpha) == 4) {
			fred  = etk_max(0.0, etk_min(1.0, fred));
			fgreen= etk_max(0.0, etk_min(1.0, fgreen));
			fblue = etk_max(0.0, etk_min(1.0, fblue));
			falpha = etk_max(0.0, etk_min(1.0, falpha));
			localColor.red  = (uint8_t)(fred * 255.);
			localColor.green= (uint8_t)(fgreen * 255.);
			localColor.blue = (uint8_t)(fblue * 255.);
			localColor.alpha = (uint8_t)(falpha * 255.);
		} else if (sscanf(inputData + 4, "%f%%,%f%%,%f%%", &fred, &fgreen, &fblue) == 3) {
			fred  = etk_max(0.0, etk_min(1.0, fred));
			fgreen= etk_max(0.0, etk_min(1.0, fgreen));
			fblue = etk_max(0.0, etk_min(1.0, fblue));
			localColor.red  = (uint8_t)(fred * 255.);
			localColor.green= (uint8_t)(fgreen * 255.);
			localColor.blue = (uint8_t)(fblue * 255.);
		} else {
			SVG_ERROR(" pb in parsing the color : \"" << inputData << "\" ==> unknown methode ...");
		}
	} else if(    4 < len
	           && inputData[0] == 'u'
	           && inputData[1] == 'r'
	           && inputData[2] == 'l'
	           && inputData[3] == '(') {
		if (inputData[4] == '#') {
			// TODO : parse gradient ...
		}
		SVG_ERROR(" pb in parsing the color : \"" << inputData << "\" ==> url(XXX) is not supported now ...");
	} else {
		bool findIt = false;
		// direct named color ...
		for (int32_t iii=0; iii<listOfColorSize; iii++) {
			if (strnCmpNoCase(listOfColor[iii].colorName, inputData, strlen(listOfColor[iii].colorName)) == true) {
				findIt = true;
				localColor.red  = (uint8_t)((0xFF000000 & listOfColor[iii].color) >> 24) ;
				localColor.green= (uint8_t)((0x00FF0000 & listOfColor[iii].color) >> 16) ;
				localColor.blue = (uint8_t)((0x0000FF00 & listOfColor[iii].color) >> 8) ;
				localColor.alpha = (uint8_t)(0x000000FF & listOfColor[iii].color) ;
				// stop searching
				break;
			}
		}
		// or color in reference ...
		if (findIt == false) {
			// TODO : Might be a named color ... need to find it ...
			SVG_ERROR(" pb in parsing the color : \"" << inputData << "\" ==> refs color not supported now ...");
		}
	}
	SVG_VERBOSE("Parse color : \"" << inputData << "\" ==> " << localColor);
	return localColor;
}


/**
 * @brief Parse all the element needed in the basic node
 * @param[in] node standart XML node
 * @return true if no problem arrived
 */
bool svg::Base::Parse(TiXmlNode * node, agg::trans_affine& parentTrans, coord2D_ts& sizeMax)
{
	SVG_ERROR("NOT IMPLEMENTED");
	sizeMax.x = 0;
	sizeMax.y = 0;
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


/*
void svg::Base::AggCheckChange(agg::path_storage& path, etk::VectorType<agg::rgba8> &colors, etk::VectorType<uint32_t> &pathIdx, PaintState &curentPaintProp)
{
	if (curentPaintProp != m_paint) {
		SVG_INFO("add path color = " << m_paint.fill);
		// New color. Every new color creates new path in the path object.
		colors.PushBack(agg::rgba8(m_paint.fill.red, m_paint.fill.green, m_paint.fill.blue, m_paint.fill.alpha));
		uint32_t tmpPathNew = path.start_new_path();
		pathIdx.PushBack(tmpPathNew);
		curentPaintProp = m_paint;
	}
}
*/
