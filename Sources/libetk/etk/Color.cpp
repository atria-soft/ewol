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


const color_ts etk::color::color_none = {0x00, 0x00, 0x00, 0x00};
const color_ts etk::color::color_AliceBlue = {0xF0, 0xF8, 0xFF, 0xFF};
const color_ts etk::color::color_AntiqueWhite = {0xFA, 0xEB, 0xD7, 0xFF};
const color_ts etk::color::color_Aqua = {0x00, 0xFF, 0xFF, 0xFF};
const color_ts etk::color::color_Aquamarine = {0x7F, 0xFF, 0xD4, 0xFF};
const color_ts etk::color::color_Azure = {0xF0, 0xFF, 0xFF, 0xFF};
const color_ts etk::color::color_Beige = {0xF5, 0xF5, 0xDC, 0xFF};
const color_ts etk::color::color_Bisque = {0xFF, 0xE4, 0xC4, 0xFF};
const color_ts etk::color::color_Black = {0x00, 0x00, 0x00, 0xFF};
const color_ts etk::color::color_BlanchedAlmond = {0xFF, 0xEB, 0xCD, 0xFF};
const color_ts etk::color::color_Blue = {0x00, 0x00, 0xFF, 0xFF};
const color_ts etk::color::color_BlueViolet = {0x8A, 0x2B, 0xE2, 0xFF};
const color_ts etk::color::color_Brown = {0xA5, 0x2A, 0x2A, 0xFF};
const color_ts etk::color::color_BurlyWood = {0xDE, 0xB8, 0x87, 0xFF};
const color_ts etk::color::color_CadetBlue = {0x5F, 0x9E, 0xA0, 0xFF};
const color_ts etk::color::color_Chartreuse = {0x7F, 0xFF, 0x00, 0xFF};
const color_ts etk::color::color_Chocolate = {0xD2, 0x69, 0x1E, 0xFF};
const color_ts etk::color::color_Coral = {0xFF, 0x7F, 0x50, 0xFF};
const color_ts etk::color::color_CornflowerBlue = {0x64, 0x95, 0xED, 0xFF};
const color_ts etk::color::color_Cornsilk = {0xFF, 0xF8, 0xDC, 0xFF};
const color_ts etk::color::color_Crimson = {0xDC, 0x14, 0x3C, 0xFF};
const color_ts etk::color::color_Cyan = {0x00, 0xFF, 0xFF, 0xFF};
const color_ts etk::color::color_DarkBlue = {0x00, 0x00, 0x8B, 0xFF};
const color_ts etk::color::color_DarkCyan = {0x00, 0x8B, 0x8B, 0xFF};
const color_ts etk::color::color_DarkGoldenRod = {0xB8, 0x86, 0x0B, 0xFF};
const color_ts etk::color::color_DarkGray = {0xA9, 0xA9, 0xA9, 0xFF};
const color_ts etk::color::color_DarkGrey = {0xA9, 0xA9, 0xA9, 0xFF};
const color_ts etk::color::color_DarkGreen = {0x00, 0x64, 0x00, 0xFF};
const color_ts etk::color::color_DarkKhaki = {0xBD, 0xB7, 0x6B, 0xFF};
const color_ts etk::color::color_DarkMagenta = {0x8B, 0x00, 0x8B, 0xFF};
const color_ts etk::color::color_DarkOliveGreen = {0x55, 0x6B, 0x2F, 0xFF};
const color_ts etk::color::color_Darkorange = {0xFF, 0x8C, 0x00, 0xFF};
const color_ts etk::color::color_DarkOrchid = {0x99, 0x32, 0xCC, 0xFF};
const color_ts etk::color::color_DarkRed = {0x8B, 0x00, 0x00, 0xFF};
const color_ts etk::color::color_DarkSalmon = {0xE9, 0x96, 0x7A, 0xFF};
const color_ts etk::color::color_DarkSeaGreen = {0x8F, 0xBC, 0x8F, 0xFF};
const color_ts etk::color::color_DarkSlateBlue = {0x48, 0x3D, 0x8B, 0xFF};
const color_ts etk::color::color_DarkSlateGray = {0x2F, 0x4F, 0x4F, 0xFF};
const color_ts etk::color::color_DarkSlateGrey = {0x2F, 0x4F, 0x4F, 0xFF};
const color_ts etk::color::color_DarkTurquoise = {0x00, 0xCE, 0xD1, 0xFF};
const color_ts etk::color::color_DarkViolet = {0x94, 0x00, 0xD3, 0xFF};
const color_ts etk::color::color_DeepPink = {0xFF, 0x14, 0x93, 0xFF};
const color_ts etk::color::color_DeepSkyBlue = {0x00, 0xBF, 0xFF, 0xFF};
const color_ts etk::color::color_DimGray = {0x69, 0x69, 0x69, 0xFF};
const color_ts etk::color::color_DimGrey = {0x69, 0x69, 0x69, 0xFF};
const color_ts etk::color::color_DodgerBlue = {0x1E, 0x90, 0xFF, 0xFF};
const color_ts etk::color::color_FireBrick = {0xB2, 0x22, 0x22, 0xFF};
const color_ts etk::color::color_FloralWhite = {0xFF, 0xFA, 0xF0, 0xFF};
const color_ts etk::color::color_ForestGreen = {0x22, 0x8B, 0x22, 0xFF};
const color_ts etk::color::color_Fuchsia = {0xFF, 0x00, 0xFF, 0xFF};
const color_ts etk::color::color_Gainsboro = {0xDC, 0xDC, 0xDC, 0xFF};
const color_ts etk::color::color_GhostWhite = {0xF8, 0xF8, 0xFF, 0xFF};
const color_ts etk::color::color_Gold = {0xFF, 0xD7, 0x00, 0xFF};
const color_ts etk::color::color_GoldenRod = {0xDA, 0xA5, 0x20, 0xFF};
const color_ts etk::color::color_Gray = {0x80, 0x80, 0x80, 0xFF};
const color_ts etk::color::color_Grey = {0x80, 0x80, 0x80, 0xFF};
const color_ts etk::color::color_Green = {0x00, 0x80, 0x00, 0xFF};
const color_ts etk::color::color_GreenYellow = {0xAD, 0xFF, 0x2F, 0xFF};
const color_ts etk::color::color_HoneyDew = {0xF0, 0xFF, 0xF0, 0xFF};
const color_ts etk::color::color_HotPink = {0xFF, 0x69, 0xB4, 0xFF};
const color_ts etk::color::color_IndianRed  = {0xCD, 0x5C, 0x5C, 0xFF};
const color_ts etk::color::color_Indigo  = {0x4B, 0x00, 0x82, 0xFF};
const color_ts etk::color::color_Ivory = {0xFF, 0xFF, 0xF0, 0xFF};
const color_ts etk::color::color_Khaki = {0xF0, 0xE6, 0x8C, 0xFF};
const color_ts etk::color::color_Lavender = {0xE6, 0xE6, 0xFA, 0xFF};
const color_ts etk::color::color_LavenderBlush = {0xFF, 0xF0, 0xF5, 0xFF};
const color_ts etk::color::color_LawnGreen = {0x7C, 0xFC, 0x00, 0xFF};
const color_ts etk::color::color_LemonChiffon = {0xFF, 0xFA, 0xCD, 0xFF};
const color_ts etk::color::color_LightBlue = {0xAD, 0xD8, 0xE6, 0xFF};
const color_ts etk::color::color_LightCoral = {0xF0, 0x80, 0x80, 0xFF};
const color_ts etk::color::color_LightCyan = {0xE0, 0xFF, 0xFF, 0xFF};
const color_ts etk::color::color_LightGoldenRodYellow = {0xFA, 0xFA, 0xD2, 0xFF};
const color_ts etk::color::color_LightGray = {0xD3, 0xD3, 0xD3, 0xFF};
const color_ts etk::color::color_LightGrey = {0xD3, 0xD3, 0xD3, 0xFF};
const color_ts etk::color::color_LightGreen = {0x90, 0xEE, 0x90, 0xFF};
const color_ts etk::color::color_LightPink = {0xFF, 0xB6, 0xC1, 0xFF};
const color_ts etk::color::color_LightSalmon = {0xFF, 0xA0, 0x7A, 0xFF};
const color_ts etk::color::color_LightSeaGreen = {0x20, 0xB2, 0xAA, 0xFF};
const color_ts etk::color::color_LightSkyBlue = {0x87, 0xCE, 0xFA, 0xFF};
const color_ts etk::color::color_LightSlateGray = {0x77, 0x88, 0x99, 0xFF};
const color_ts etk::color::color_LightSlateGrey = {0x77, 0x88, 0x99, 0xFF};
const color_ts etk::color::color_LightSteelBlue = {0xB0, 0xC4, 0xDE, 0xFF};
const color_ts etk::color::color_LightYellow = {0xFF, 0xFF, 0xE0, 0xFF};
const color_ts etk::color::color_Lime = {0x00, 0xFF, 0x00, 0xFF};
const color_ts etk::color::color_LimeGreen = {0x32, 0xCD, 0x32, 0xFF};
const color_ts etk::color::color_Linen = {0xFA, 0xF0, 0xE6, 0xFF};
const color_ts etk::color::color_Magenta = {0xFF, 0x00, 0xFF, 0xFF};
const color_ts etk::color::color_Maroon = {0x80, 0x00, 0x00, 0xFF};
const color_ts etk::color::color_MediumAquaMarine = {0x66, 0xCD, 0xAA, 0xFF};
const color_ts etk::color::color_MediumBlue = {0x00, 0x00, 0xCD, 0xFF};
const color_ts etk::color::color_MediumOrchid = {0xBA, 0x55, 0xD3, 0xFF};
const color_ts etk::color::color_MediumPurple = {0x93, 0x70, 0xD8, 0xFF};
const color_ts etk::color::color_MediumSeaGreen = {0x3C, 0xB3, 0x71, 0xFF};
const color_ts etk::color::color_MediumSlateBlue = {0x7B, 0x68, 0xEE, 0xFF};
const color_ts etk::color::color_MediumSpringGreen = {0x00, 0xFA, 0x9A, 0xFF};
const color_ts etk::color::color_MediumTurquoise = {0x48, 0xD1, 0xCC, 0xFF};
const color_ts etk::color::color_MediumVioletRed = {0xC7, 0x15, 0x85, 0xFF};
const color_ts etk::color::color_MidnightBlue = {0x19, 0x19, 0x70, 0xFF};
const color_ts etk::color::color_MintCream = {0xF5, 0xFF, 0xFA, 0xFF};
const color_ts etk::color::color_MistyRose = {0xFF, 0xE4, 0xE1, 0xFF};
const color_ts etk::color::color_Moccasin = {0xFF, 0xE4, 0xB5, 0xFF};
const color_ts etk::color::color_NavajoWhite = {0xFF, 0xDE, 0xAD, 0xFF};
const color_ts etk::color::color_Navy = {0x00, 0x00, 0x80, 0xFF};
const color_ts etk::color::color_OldLace = {0xFD, 0xF5, 0xE6, 0xFF};
const color_ts etk::color::color_Olive = {0x80, 0x80, 0x00, 0xFF};
const color_ts etk::color::color_OliveDrab = {0x6B, 0x8E, 0x23, 0xFF};
const color_ts etk::color::color_Orange = {0xFF, 0xA5, 0x00, 0xFF};
const color_ts etk::color::color_OrangeRed = {0xFF, 0x45, 0x00, 0xFF};
const color_ts etk::color::color_Orchid = {0xDA, 0x70, 0xD6, 0xFF};
const color_ts etk::color::color_PaleGoldenRod = {0xEE, 0xE8, 0xAA, 0xFF};
const color_ts etk::color::color_PaleGreen = {0x98, 0xFB, 0x98, 0xFF};
const color_ts etk::color::color_PaleTurquoise = {0xAF, 0xEE, 0xEE, 0xFF};
const color_ts etk::color::color_PaleVioletRed = {0xD8, 0x70, 0x93, 0xFF};
const color_ts etk::color::color_PapayaWhip = {0xFF, 0xEF, 0xD5, 0xFF};
const color_ts etk::color::color_PeachPuff = {0xFF, 0xDA, 0xB9, 0xFF};
const color_ts etk::color::color_Peru = {0xCD, 0x85, 0x3F, 0xFF};
const color_ts etk::color::color_Pink = {0xFF, 0xC0, 0xCB, 0xFF};
const color_ts etk::color::color_Plum = {0xDD, 0xA0, 0xDD, 0xFF};
const color_ts etk::color::color_PowderBlue = {0xB0, 0xE0, 0xE6, 0xFF};
const color_ts etk::color::color_Purple = {0x80, 0x00, 0x80, 0xFF};
const color_ts etk::color::color_Red = {0xFF, 0x00, 0x00, 0xFF};
const color_ts etk::color::color_RosyBrown = {0xBC, 0x8F, 0x8F, 0xFF};
const color_ts etk::color::color_RoyalBlue = {0x41, 0x69, 0xE1, 0xFF};
const color_ts etk::color::color_SaddleBrown = {0x8B, 0x45, 0x13, 0xFF};
const color_ts etk::color::color_Salmon = {0xFA, 0x80, 0x72, 0xFF};
const color_ts etk::color::color_SandyBrown = {0xF4, 0xA4, 0x60, 0xFF};
const color_ts etk::color::color_SeaGreen = {0x2E, 0x8B, 0x57, 0xFF};
const color_ts etk::color::color_SeaShell = {0xFF, 0xF5, 0xEE, 0xFF};
const color_ts etk::color::color_Sienna = {0xA0, 0x52, 0x2D, 0xFF};
const color_ts etk::color::color_Silver = {0xC0, 0xC0, 0xC0, 0xFF};
const color_ts etk::color::color_SkyBlue = {0x87, 0xCE, 0xEB, 0xFF};
const color_ts etk::color::color_SlateBlue = {0x6A, 0x5A, 0xCD, 0xFF};
const color_ts etk::color::color_SlateGray = {0x70, 0x80, 0x90, 0xFF};
const color_ts etk::color::color_SlateGrey = {0x70, 0x80, 0x90, 0xFF};
const color_ts etk::color::color_Snow = {0xFF, 0xFA, 0xFA, 0xFF};
const color_ts etk::color::color_SpringGreen = {0x00, 0xFF, 0x7F, 0xFF};
const color_ts etk::color::color_SteelBlue = {0x46, 0x82, 0xB4, 0xFF};
const color_ts etk::color::color_Tan = {0xD2, 0xB4, 0x8C, 0xFF};
const color_ts etk::color::color_Teal = {0x00, 0x80, 0x80, 0xFF};
const color_ts etk::color::color_Thistle = {0xD8, 0xBF, 0xD8, 0xFF};
const color_ts etk::color::color_Tomato = {0xFF, 0x63, 0x47, 0xFF};
const color_ts etk::color::color_Turquoise = {0x40, 0xE0, 0xD0, 0xFF};
const color_ts etk::color::color_Violet = {0xEE, 0x82, 0xEE, 0xFF};
const color_ts etk::color::color_Wheat = {0xF5, 0xDE, 0xB3, 0xFF};
const color_ts etk::color::color_White = {0xFF, 0xFF, 0xFF, 0xFF};
const color_ts etk::color::color_WhiteSmoke = {0xF5, 0xF5, 0xF5, 0xFF};
const color_ts etk::color::color_Yellow = {0xFF, 0xFF, 0x00, 0xFF};
const color_ts etk::color::color_YellowGreen = {0x9A, 0xCD, 0x32, 0xFF};


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

