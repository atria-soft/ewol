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

#ifndef __ETK_COLOR_H__
#define __ETK_COLOR_H__

#include <etk/UString.h>

#ifndef __ETK_USTRING_H__
	namespace etk {
		class UString;
	};
#endif

namespace etk {

	class Color {
		public:
			uint8_t red;
			uint8_t green;
			uint8_t blue;
			uint8_t alpha;
			// constructor :
			Color(void);
			Color(const etk::Color& data);
			Color(const uint32_t data);
			Color(uint8_t _red, uint8_t _green, uint8_t _blue, uint8_t _alpha = 255);
			Color(float _red, float _green, float _blue, float _alpha = 1.0);
			Color(etk::UString data);
			Color(const char * data);
			// operator :
			etk::Color& operator=(const uint32_t _input);
			etk::Color& operator=(const etk::Color& _input);
			bool operator==(const etk::Color& _input) const;
			bool operator!=(const etk::Color& _input) const;
			// access :
			void Set(etk::UString _input);
			uint32_t Get(void) const;
			/*****************************************************
			 *    Cout << operator
			 *****************************************************/
			friend etk::CCout& operator <<( etk::CCout &os,const etk::Color &obj);
	};
	
	etk::CCout& operator <<(etk::CCout &os, const etk::Color &obj);
	
	namespace color {
		extern const Color none;
		extern const Color aliceBlue;
		extern const Color antiqueWhite;
		extern const Color aqua;
		extern const Color aquamarine;
		extern const Color azure;
		extern const Color beige;
		extern const Color bisque;
		extern const Color black;
		extern const Color blanchedAlmond;
		extern const Color blue;
		extern const Color blueViolet;
		extern const Color brown;
		extern const Color burlyWood;
		extern const Color cadetBlue;
		extern const Color chartreuse;
		extern const Color chocolate;
		extern const Color coral;
		extern const Color cornflowerBlue;
		extern const Color cornsilk;
		extern const Color crimson;
		extern const Color cyan;
		extern const Color darkBlue;
		extern const Color darkCyan;
		extern const Color darkGoldenRod;
		extern const Color darkGray;
		extern const Color darkGrey;
		extern const Color darkGreen;
		extern const Color darkKhaki;
		extern const Color darkMagenta;
		extern const Color darkOliveGreen;
		extern const Color darkorange;
		extern const Color darkOrchid;
		extern const Color darkRed;
		extern const Color darkSalmon;
		extern const Color darkSeaGreen;
		extern const Color darkSlateBlue;
		extern const Color darkSlateGray;
		extern const Color darkSlateGrey;
		extern const Color darkTurquoise;
		extern const Color darkViolet;
		extern const Color deepPink;
		extern const Color deepSkyBlue;
		extern const Color dimGray;
		extern const Color dimGrey;
		extern const Color dodgerBlue;
		extern const Color fireBrick;
		extern const Color floralWhite;
		extern const Color forestGreen;
		extern const Color fuchsia;
		extern const Color gainsboro;
		extern const Color ghostWhite;
		extern const Color gold;
		extern const Color goldenRod;
		extern const Color gray;
		extern const Color grey;
		extern const Color green;
		extern const Color greenYellow;
		extern const Color honeyDew;
		extern const Color hotPink;
		extern const Color indianRed;
		extern const Color indigo;
		extern const Color ivory;
		extern const Color khaki;
		extern const Color lavender;
		extern const Color lavenderBlush;
		extern const Color lawnGreen;
		extern const Color lemonChiffon;
		extern const Color lightBlue;
		extern const Color lightCoral;
		extern const Color lightCyan;
		extern const Color lightGoldenRodYellow;
		extern const Color lightGray;
		extern const Color lightGrey;
		extern const Color lightGreen;
		extern const Color lightPink;
		extern const Color lightSalmon;
		extern const Color lightSeaGreen;
		extern const Color lightSkyBlue;
		extern const Color lightSlateGray;
		extern const Color lightSlateGrey;
		extern const Color lightSteelBlue;
		extern const Color lightYellow;
		extern const Color lime;
		extern const Color limeGreen;
		extern const Color linen;
		extern const Color magenta;
		extern const Color maroon;
		extern const Color mediumAquaMarine;
		extern const Color mediumBlue;
		extern const Color mediumOrchid;
		extern const Color mediumPurple;
		extern const Color mediumSeaGreen;
		extern const Color mediumSlateBlue;
		extern const Color mediumSpringGreen;
		extern const Color mediumTurquoise;
		extern const Color mediumVioletRed;
		extern const Color midnightBlue;
		extern const Color mintCream;
		extern const Color mistyRose;
		extern const Color moccasin;
		extern const Color navajoWhite;
		extern const Color navy;
		extern const Color oldLace;
		extern const Color olive;
		extern const Color oliveDrab;
		extern const Color orange;
		extern const Color orangeRed;
		extern const Color orchid;
		extern const Color paleGoldenRod;
		extern const Color paleGreen;
		extern const Color paleTurquoise;
		extern const Color paleVioletRed;
		extern const Color papayaWhip;
		extern const Color peachPuff;
		extern const Color peru;
		extern const Color pink;
		extern const Color plum;
		extern const Color powderBlue;
		extern const Color purple;
		extern const Color red;
		extern const Color rosyBrown;
		extern const Color royalBlue;
		extern const Color saddleBrown;
		extern const Color salmon;
		extern const Color sandyBrown;
		extern const Color seaGreen;
		extern const Color seaShell;
		extern const Color sienna;
		extern const Color silver;
		extern const Color skyBlue;
		extern const Color slateBlue;
		extern const Color slateGray;
		extern const Color slateGrey;
		extern const Color snow;
		extern const Color springGreen;
		extern const Color steelBlue;
		extern const Color tan;
		extern const Color teal;
		extern const Color thistle;
		extern const Color tomato;
		extern const Color turquoise;
		extern const Color violet;
		extern const Color wheat;
		extern const Color white;
		extern const Color whiteSmoke;
		extern const Color yellow;
		extern const Color yellowGreen;
	};
};

#endif

