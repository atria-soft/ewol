/**
 *******************************************************************************
 * @file etk/Color.h
 * @brief Ewol Tool Kit : basic colors
 * @author Edouard DUPIN
 * @date 23/04/2012
 * @par Project
 * Ewol TK
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

#include <etk/Types.h>
#include <etk/Color.h>
#include <etk/DebugInternal.h>


const color_ts etk::color::color_none(0x00000000);
const color_ts etk::color::color_AliceBlue(0xF0F8FFFF);
const color_ts etk::color::color_AntiqueWhite(0xFAEBD7FF);
const color_ts etk::color::color_Aqua(0x00FFFFFF);
const color_ts etk::color::color_Aquamarine(0x7FFFD4FF);
const color_ts etk::color::color_Azure(0xF0FFFFFF);
const color_ts etk::color::color_Beige(0xF5F5DCFF);
const color_ts etk::color::color_Bisque(0xFFE4C4FF);
const color_ts etk::color::color_Black(0x000000FF);
const color_ts etk::color::color_BlanchedAlmond(0xFFEBCDFF);
const color_ts etk::color::color_Blue(0x0000FFFF);
const color_ts etk::color::color_BlueViolet(0x8A2BE2FF);
const color_ts etk::color::color_Brown(0xA52A2AFF);
const color_ts etk::color::color_BurlyWood(0xDEB887FF);
const color_ts etk::color::color_CadetBlue(0x5F9EA0FF);
const color_ts etk::color::color_Chartreuse(0x7FFF00FF);
const color_ts etk::color::color_Chocolate(0xD2691EFF);
const color_ts etk::color::color_Coral(0xFF7F50FF);
const color_ts etk::color::color_CornflowerBlue(0x6495EDFF);
const color_ts etk::color::color_Cornsilk(0xFFF8DCFF);
const color_ts etk::color::color_Crimson(0xDC143CFF);
const color_ts etk::color::color_Cyan(0x00FFFFFF);
const color_ts etk::color::color_DarkBlue(0x00008BFF);
const color_ts etk::color::color_DarkCyan(0x008B8BFF);
const color_ts etk::color::color_DarkGoldenRod(0xB8860BFF);
const color_ts etk::color::color_DarkGray(0xA9A9A9FF);
const color_ts etk::color::color_DarkGrey(0xA9A9A9FF);
const color_ts etk::color::color_DarkGreen(0x006400FF);
const color_ts etk::color::color_DarkKhaki(0xBDB76BFF);
const color_ts etk::color::color_DarkMagenta(0x8B008BFF);
const color_ts etk::color::color_DarkOliveGreen(0x556B2FFF);
const color_ts etk::color::color_Darkorange(0xFF8C00FF);
const color_ts etk::color::color_DarkOrchid(0x9932CCFF);
const color_ts etk::color::color_DarkRed(0x8B0000FF);
const color_ts etk::color::color_DarkSalmon(0xE9967AFF);
const color_ts etk::color::color_DarkSeaGreen(0x8FBC8FFF);
const color_ts etk::color::color_DarkSlateBlue(0x483D8BFF);
const color_ts etk::color::color_DarkSlateGray(0x2F4F4FFF);
const color_ts etk::color::color_DarkSlateGrey(0x2F4F4FFF);
const color_ts etk::color::color_DarkTurquoise(0x00CED1FF);
const color_ts etk::color::color_DarkViolet(0x9400D3FF);
const color_ts etk::color::color_DeepPink(0xFF1493FF);
const color_ts etk::color::color_DeepSkyBlue(0x00BFFFFF);
const color_ts etk::color::color_DimGray(0x696969FF);
const color_ts etk::color::color_DimGrey(0x696969FF);
const color_ts etk::color::color_DodgerBlue(0x1E90FFFF);
const color_ts etk::color::color_FireBrick(0xB22222FF);
const color_ts etk::color::color_FloralWhite(0xFFFAF0FF);
const color_ts etk::color::color_ForestGreen(0x228B22FF);
const color_ts etk::color::color_Fuchsia(0xFF00FFFF);
const color_ts etk::color::color_Gainsboro(0xDCDCDCFF);
const color_ts etk::color::color_GhostWhite(0xF8F8FFFF);
const color_ts etk::color::color_Gold(0xFFD700FF);
const color_ts etk::color::color_GoldenRod(0xDAA520FF);
const color_ts etk::color::color_Gray(0x808080FF);
const color_ts etk::color::color_Grey(0x808080FF);
const color_ts etk::color::color_Green(0x008000FF);
const color_ts etk::color::color_GreenYellow(0xADFF2FFF);
const color_ts etk::color::color_HoneyDew(0xF0FFF0FF);
const color_ts etk::color::color_HotPink(0xFF69B4FF);
const color_ts etk::color::color_IndianRed (0xCD5C5CFF);
const color_ts etk::color::color_Indigo (0x4B0082FF);
const color_ts etk::color::color_Ivory(0xFFFFF0FF);
const color_ts etk::color::color_Khaki(0xF0E68CFF);
const color_ts etk::color::color_Lavender(0xE6E6FAFF);
const color_ts etk::color::color_LavenderBlush(0xFFF0F5FF);
const color_ts etk::color::color_LawnGreen(0x7CFC00FF);
const color_ts etk::color::color_LemonChiffon(0xFFFACDFF);
const color_ts etk::color::color_LightBlue(0xADD8E6FF);
const color_ts etk::color::color_LightCoral(0xF08080FF);
const color_ts etk::color::color_LightCyan(0xE0FFFFFF);
const color_ts etk::color::color_LightGoldenRodYellow(0xFAFAD2FF);
const color_ts etk::color::color_LightGray(0xD3D3D3FF);
const color_ts etk::color::color_LightGrey(0xD3D3D3FF);
const color_ts etk::color::color_LightGreen(0x90EE90FF);
const color_ts etk::color::color_LightPink(0xFFB6C1FF);
const color_ts etk::color::color_LightSalmon(0xFFA07AFF);
const color_ts etk::color::color_LightSeaGreen(0x20B2AAFF);
const color_ts etk::color::color_LightSkyBlue(0x87CEFAFF);
const color_ts etk::color::color_LightSlateGray(0x778899FF);
const color_ts etk::color::color_LightSlateGrey(0x778899FF);
const color_ts etk::color::color_LightSteelBlue(0xB0C4DEFF);
const color_ts etk::color::color_LightYellow(0xFFFFE0FF);
const color_ts etk::color::color_Lime(0x00FF00FF);
const color_ts etk::color::color_LimeGreen(0x32CD32FF);
const color_ts etk::color::color_Linen(0xFAF0E6FF);
const color_ts etk::color::color_Magenta(0xFF00FFFF);
const color_ts etk::color::color_Maroon(0x800000FF);
const color_ts etk::color::color_MediumAquaMarine(0x66CDAAFF);
const color_ts etk::color::color_MediumBlue(0x0000CDFF);
const color_ts etk::color::color_MediumOrchid(0xBA55D3FF);
const color_ts etk::color::color_MediumPurple(0x9370D8FF);
const color_ts etk::color::color_MediumSeaGreen(0x3CB371FF);
const color_ts etk::color::color_MediumSlateBlue(0x7B68EEFF);
const color_ts etk::color::color_MediumSpringGreen(0x00FA9AFF);
const color_ts etk::color::color_MediumTurquoise(0x48D1CCFF);
const color_ts etk::color::color_MediumVioletRed(0xC71585FF);
const color_ts etk::color::color_MidnightBlue(0x191970FF);
const color_ts etk::color::color_MintCream(0xF5FFFAFF);
const color_ts etk::color::color_MistyRose(0xFFE4E1FF);
const color_ts etk::color::color_Moccasin(0xFFE4B5FF);
const color_ts etk::color::color_NavajoWhite(0xFFDEADFF);
const color_ts etk::color::color_Navy(0x000080FF);
const color_ts etk::color::color_OldLace(0xFDF5E6FF);
const color_ts etk::color::color_Olive(0x808000FF);
const color_ts etk::color::color_OliveDrab(0x6B8E23FF);
const color_ts etk::color::color_Orange(0xFFA500FF);
const color_ts etk::color::color_OrangeRed(0xFF4500FF);
const color_ts etk::color::color_Orchid(0xDA70D6FF);
const color_ts etk::color::color_PaleGoldenRod(0xEEE8AAFF);
const color_ts etk::color::color_PaleGreen(0x98FB98FF);
const color_ts etk::color::color_PaleTurquoise(0xAFEEEEFF);
const color_ts etk::color::color_PaleVioletRed(0xD87093FF);
const color_ts etk::color::color_PapayaWhip(0xFFEFD5FF);
const color_ts etk::color::color_PeachPuff(0xFFDAB9FF);
const color_ts etk::color::color_Peru(0xCD853FFF);
const color_ts etk::color::color_Pink(0xFFC0CBFF);
const color_ts etk::color::color_Plum(0xDDA0DDFF);
const color_ts etk::color::color_PowderBlue(0xB0E0E6FF);
const color_ts etk::color::color_Purple(0x800080FF);
const color_ts etk::color::color_Red(0xFF0000FF);
const color_ts etk::color::color_RosyBrown(0xBC8F8FFF);
const color_ts etk::color::color_RoyalBlue(0x4169E1FF);
const color_ts etk::color::color_SaddleBrown(0x8B4513FF);
const color_ts etk::color::color_Salmon(0xFA8072FF);
const color_ts etk::color::color_SandyBrown(0xF4A460FF);
const color_ts etk::color::color_SeaGreen(0x2E8B57FF);
const color_ts etk::color::color_SeaShell(0xFFF5EEFF);
const color_ts etk::color::color_Sienna(0xA0522DFF);
const color_ts etk::color::color_Silver(0xC0C0C0FF);
const color_ts etk::color::color_SkyBlue(0x87CEEBFF);
const color_ts etk::color::color_SlateBlue(0x6A5ACDFF);
const color_ts etk::color::color_SlateGray(0x708090FF);
const color_ts etk::color::color_SlateGrey(0x708090FF);
const color_ts etk::color::color_Snow(0xFFFAFAFF);
const color_ts etk::color::color_SpringGreen(0x00FF7FFF);
const color_ts etk::color::color_SteelBlue(0x4682B4FF);
const color_ts etk::color::color_Tan(0xD2B48CFF);
const color_ts etk::color::color_Teal(0x008080FF);
const color_ts etk::color::color_Thistle(0xD8BFD8FF);
const color_ts etk::color::color_Tomato(0xFF6347FF);
const color_ts etk::color::color_Turquoise(0x40E0D0FF);
const color_ts etk::color::color_Violet(0xEE82EEFF);
const color_ts etk::color::color_Wheat(0xF5DEB3FF);
const color_ts etk::color::color_White(0xFFFFFFFF);
const color_ts etk::color::color_WhiteSmoke(0xF5F5F5FF);
const color_ts etk::color::color_Yellow(0xFFFF00FF);
const color_ts etk::color::color_YellowGreen(0x9ACD32FF);


typedef struct {
	const char * colorName;
	color_ts color;
} colorList_ts;

static const colorList_ts listOfColor[] = {
	{ "none",				etk::color::color_none},
	{ "AliceBlue",			etk::color::color_AliceBlue},
	{ "AntiqueWhite",		etk::color::color_AntiqueWhite},
	{ "Aqua",				etk::color::color_Aqua},
	{ "Aquamarine",			etk::color::color_Aquamarine},
	{ "Azure",				etk::color::color_Azure},
	{ "Beige",				etk::color::color_Beige},
	{ "Bisque",				etk::color::color_Bisque},
	{ "Black",				etk::color::color_Black},
	{ "BlanchedAlmond",		etk::color::color_BlanchedAlmond},
	{ "Blue",				etk::color::color_Blue},
	{ "BlueViolet",			etk::color::color_BlueViolet},
	{ "Brown",				etk::color::color_Brown},
	{ "BurlyWood",			etk::color::color_BurlyWood},
	{ "CadetBlue",			etk::color::color_CadetBlue},
	{ "Chartreuse",			etk::color::color_Chartreuse},
	{ "Chocolate",			etk::color::color_Chocolate},
	{ "Coral",				etk::color::color_Coral},
	{ "CornflowerBlue",		etk::color::color_CornflowerBlue},
	{ "Cornsilk",			etk::color::color_Cornsilk},
	{ "Crimson",			etk::color::color_Crimson},
	{ "Cyan",				etk::color::color_Cyan},
	{ "DarkBlue",			etk::color::color_DarkBlue},
	{ "DarkCyan",			etk::color::color_DarkCyan},
	{ "DarkGoldenRod",		etk::color::color_DarkGoldenRod},
	{ "DarkGray",			etk::color::color_DarkGray},
	{ "DarkGrey",			etk::color::color_DarkGrey},
	{ "DarkGreen",			etk::color::color_DarkGreen},
	{ "DarkKhaki",			etk::color::color_DarkKhaki},
	{ "DarkMagenta",		etk::color::color_DarkMagenta},
	{ "DarkOliveGreen",		etk::color::color_DarkOliveGreen},
	{ "Darkorange",			etk::color::color_Darkorange},
	{ "DarkOrchid",			etk::color::color_DarkOrchid},
	{ "DarkRed",			etk::color::color_DarkRed},
	{ "DarkSalmon",			etk::color::color_DarkSalmon},
	{ "DarkSeaGreen",		etk::color::color_DarkSeaGreen},
	{ "DarkSlateBlue",		etk::color::color_DarkSlateBlue},
	{ "DarkSlateGray",		etk::color::color_DarkSlateGray},
	{ "DarkSlateGrey",		etk::color::color_DarkSlateGrey},
	{ "DarkTurquoise",		etk::color::color_DarkTurquoise},
	{ "DarkViolet",			etk::color::color_DarkViolet},
	{ "DeepPink",			etk::color::color_DeepPink},
	{ "DeepSkyBlue",		etk::color::color_DeepSkyBlue},
	{ "DimGray",			etk::color::color_DimGray},
	{ "DimGrey",			etk::color::color_DimGrey},
	{ "DodgerBlue",			etk::color::color_DodgerBlue},
	{ "FireBrick",			etk::color::color_FireBrick},
	{ "FloralWhite",		etk::color::color_FloralWhite},
	{ "ForestGreen",		etk::color::color_ForestGreen},
	{ "Fuchsia",			etk::color::color_Fuchsia},
	{ "Gainsboro",			etk::color::color_Gainsboro},
	{ "GhostWhite",			etk::color::color_GhostWhite},
	{ "Gold",				etk::color::color_Gold},
	{ "GoldenRod",			etk::color::color_GoldenRod},
	{ "Gray",				etk::color::color_Gray},
	{ "Grey",				etk::color::color_Grey},
	{ "Green",				etk::color::color_Green},
	{ "GreenYellow",		etk::color::color_GreenYellow},
	{ "HoneyDew",			etk::color::color_HoneyDew},
	{ "HotPink",			etk::color::color_HotPink},
	{ "IndianRed",			etk::color::color_IndianRed},
	{ "Indigo",				etk::color::color_Indigo},
	{ "Ivory",				etk::color::color_Ivory},
	{ "Khaki",				etk::color::color_Khaki},
	{ "Lavender",			etk::color::color_Lavender},
	{ "LavenderBlush",		etk::color::color_LavenderBlush},
	{ "LawnGreen",			etk::color::color_LawnGreen},
	{ "LemonChiffon",		etk::color::color_LemonChiffon},
	{ "LightBlue",			etk::color::color_LightBlue},
	{ "LightCoral",			etk::color::color_LightCoral},
	{ "LightCyan",			etk::color::color_LightCyan},
	{ "LightGoldenRodYellow",	etk::color::color_LightGoldenRodYellow},
	{ "LightGray",			etk::color::color_LightGray},
	{ "LightGrey",			etk::color::color_LightGrey},
	{ "LightGreen",			etk::color::color_LightGreen},
	{ "LightPink",			etk::color::color_LightPink},
	{ "LightSalmon",		etk::color::color_LightSalmon},
	{ "LightSeaGreen",		etk::color::color_LightSeaGreen},
	{ "LightSkyBlue",		etk::color::color_LightSkyBlue},
	{ "LightSlateGray",		etk::color::color_LightSlateGray},
	{ "LightSlateGrey",		etk::color::color_LightSlateGrey},
	{ "LightSteelBlue",		etk::color::color_LightSteelBlue},
	{ "LightYellow",		etk::color::color_LightYellow},
	{ "Lime",				etk::color::color_Lime},
	{ "LimeGreen",			etk::color::color_LimeGreen},
	{ "Linen",				etk::color::color_Linen},
	{ "Magenta",			etk::color::color_Magenta},
	{ "Maroon",				etk::color::color_Maroon},
	{ "MediumAquaMarine",	etk::color::color_MediumAquaMarine},
	{ "MediumBlue",			etk::color::color_MediumBlue},
	{ "MediumOrchid",		etk::color::color_MediumOrchid},
	{ "MediumPurple",		etk::color::color_MediumPurple},
	{ "MediumSeaGreen",		etk::color::color_MediumSeaGreen},
	{ "MediumSlateBlue",	etk::color::color_MediumSlateBlue},
	{ "MediumSpringGreen",	etk::color::color_MediumSpringGreen},
	{ "MediumTurquoise",	etk::color::color_MediumTurquoise},
	{ "MediumVioletRed",	etk::color::color_MediumVioletRed},
	{ "MidnightBlue",		etk::color::color_MidnightBlue},
	{ "MintCream",			etk::color::color_MintCream},
	{ "MistyRose",			etk::color::color_MistyRose},
	{ "Moccasin",			etk::color::color_Moccasin},
	{ "NavajoWhite",		etk::color::color_NavajoWhite},
	{ "Navy",				etk::color::color_Navy},
	{ "OldLace",			etk::color::color_OldLace},
	{ "Olive",				etk::color::color_Olive},
	{ "OliveDrab",			etk::color::color_OliveDrab},
	{ "Orange",				etk::color::color_Orange},
	{ "OrangeRed",			etk::color::color_OrangeRed},
	{ "Orchid",				etk::color::color_Orchid},
	{ "PaleGoldenRod",		etk::color::color_PaleGoldenRod},
	{ "PaleGreen",			etk::color::color_PaleGreen},
	{ "PaleTurquoise",		etk::color::color_PaleTurquoise},
	{ "PaleVioletRed",		etk::color::color_PaleVioletRed},
	{ "PapayaWhip",			etk::color::color_PapayaWhip},
	{ "PeachPuff",			etk::color::color_PeachPuff},
	{ "Peru",				etk::color::color_Peru},
	{ "Pink",				etk::color::color_Pink},
	{ "Plum",				etk::color::color_Plum},
	{ "PowderBlue",			etk::color::color_PowderBlue},
	{ "Purple",				etk::color::color_Purple},
	{ "Red",				etk::color::color_Red},
	{ "RosyBrown",			etk::color::color_RosyBrown},
	{ "RoyalBlue",			etk::color::color_RoyalBlue},
	{ "SaddleBrown",		etk::color::color_SaddleBrown},
	{ "Salmon",				etk::color::color_Salmon},
	{ "SandyBrown",			etk::color::color_SandyBrown},
	{ "SeaGreen",			etk::color::color_SeaGreen},
	{ "SeaShell",			etk::color::color_SeaShell},
	{ "Sienna",				etk::color::color_Sienna},
	{ "Silver",				etk::color::color_Silver},
	{ "SkyBlue",			etk::color::color_SkyBlue},
	{ "SlateBlue",			etk::color::color_SlateBlue},
	{ "SlateGray",			etk::color::color_SlateGray},
	{ "SlateGrey",			etk::color::color_SlateGrey},
	{ "Snow",				etk::color::color_Snow},
	{ "SpringGreen",		etk::color::color_SpringGreen},
	{ "SteelBlue",			etk::color::color_SteelBlue},
	{ "Tan",				etk::color::color_Tan},
	{ "Teal",				etk::color::color_Teal},
	{ "Thistle",			etk::color::color_Thistle},
	{ "Tomato",				etk::color::color_Tomato},
	{ "Turquoise",			etk::color::color_Turquoise},
	{ "Violet",				etk::color::color_Violet},
	{ "Wheat",				etk::color::color_Wheat},
	{ "White",				etk::color::color_White},
	{ "WhiteSmoke",			etk::color::color_WhiteSmoke},
	{ "Yellow",				etk::color::color_Yellow},
	{ "YellowGreen",		etk::color::color_YellowGreen}
};
static const int32_t listOfColorSize = sizeof(listOfColor) / sizeof(colorList_ts);


static bool strnCmpNoCase(const char * input1, const char * input2, int32_t maxLen)
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
color_ts etk::color::Parse(const char *inputData)
{
	color_ts localColor = etk::color::color_White;
	
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
				TK_ERROR(" pb in parsing the color : \"" << inputData << "\"");
			}
		} else if (len==5) {
			if (sscanf(inputData + 1, "%1x%1x%1x%1x", &red, &green, &blue, &alpha) == 4) {
				localColor.red   = (red | red << 4);
				localColor.green = (green | green << 4);
				localColor.blue  = (blue | blue << 4);
				localColor.alpha = (alpha | alpha << 4);
			} else {
				TK_ERROR(" pb in parsing the color : \"" << inputData << "\"");
			}
		} else if (len == 7) {
			if (sscanf(inputData + 1, "%2x%2x%2x", &red, &green, &blue) == 3) {
				localColor.red  = red;
				localColor.green= green;
				localColor.blue = blue;
			} else {
				TK_ERROR(" pb in parsing the color : \"" << inputData << "\"");
			}
		} else if (len == 9) {
			if (sscanf(inputData + 1, "%2x%2x%2x%2x", &red, &green, &blue, &alpha) == 4) {
				localColor.red  = red;
				localColor.green= green;
				localColor.blue = blue;
				localColor.alpha = alpha;
			} else {
				TK_ERROR(" pb in parsing the color : \"" << inputData << "\"");
			}
		} else {
			TK_ERROR(" pb in parsing the color : \"" << inputData << "\" ==> unknown methode ...");
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
			fred  = etk_avg(0.0, fred, 1.0);
			fgreen= etk_avg(0.0, fgreen, 1.0);
			fblue = etk_avg(0.0, fblue, 1.0);
			falpha = etk_avg(0.0, falpha, 1.0);
			localColor.red  = (uint8_t)(fred * 255.);
			localColor.green= (uint8_t)(fgreen * 255.);
			localColor.blue = (uint8_t)(fblue * 255.);
			localColor.alpha = (uint8_t)(falpha * 255.);
		} else if (sscanf(inputData + 4, "%f%%,%f%%,%f%%", &fred, &fgreen, &fblue) == 3) {
			fred  = etk_avg(0.0, fred, 1.0);
			fgreen= etk_avg(0.0, fgreen, 1.0);
			fblue = etk_avg(0.0, fblue, 1.0);
			localColor.red  = (uint8_t)(fred * 255.);
			localColor.green= (uint8_t)(fgreen * 255.);
			localColor.blue = (uint8_t)(fblue * 255.);
		} else {
			TK_ERROR(" pb in parsing the color : \"" << inputData << "\" ==> unknown methode ...");
		}
	} else {
		bool findIt = false;
		// direct named color ...
		for (int32_t iii=0; iii<listOfColorSize; iii++) {
			if (strnCmpNoCase(listOfColor[iii].colorName, inputData, strlen(listOfColor[iii].colorName)) == true) {
				findIt = true;
				localColor = listOfColor[iii].color;
				// stop searching
				break;
			}
		}
		// not find color ...
		if (findIt == false) {
			TK_ERROR(" pb in parsing the color : \"" << inputData << "\" not find ...");
		}
	}
	TK_VERBOSE("Parse color : \"" << inputData << "\" ==> " << localColor);
	return localColor;
}


color_ts etk::color::Create(uint32_t val)
{
	color_ts tmpColor;
	tmpColor.red=(uint8_t)((val>>24)&0xFF);
	tmpColor.green=(uint8_t)((val>>16)&0xFF);
	tmpColor.blue=(uint8_t)((val>>8)&0xFF);
	tmpColor.alpha=(uint8_t)((val)&0xFF);
	return tmpColor;
}

color_ts etk::color::Create(uint8_t _red, uint8_t _green, uint8_t _blue, uint8_t _alpha)
{
	color_ts tmpColor;
	tmpColor.red  =_red;
	tmpColor.green=_green;
	tmpColor.blue =_blue;
	tmpColor.alpha=_alpha;
	return tmpColor;
}

color_ts etk::color::Create(float _red, float _green, float _blue, float _alpha)
{
	color_ts tmpColor;
	_red   = etk_avg((float)0, (float)_red,   (float)1);
	_green = etk_avg((float)0, (float)_green, (float)1);
	_blue  = etk_avg((float)0, (float)_blue,  (float)1);
	_alpha = etk_avg((float)0, (float)_alpha, (float)1);
	tmpColor.red  =(uint8_t)(_red*255);
	tmpColor.green=(uint8_t)(_green*255);
	tmpColor.blue =(uint8_t)(_blue*255);
	tmpColor.alpha=(uint8_t)(_alpha*255);
	return tmpColor;
}

