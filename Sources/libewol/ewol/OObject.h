/**
 *******************************************************************************
 * @file ewol/OObject.h
 * @brief ewol OpenGl Object system (header)
 * @author Edouard DUPIN
 * @date 24/10/2011
 * @par Project
 * ewol
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

#ifndef __EWOL_O_OBJECT_H__
#define __EWOL_O_OBJECT_H__

#include <etk/Types.h>
#include <etk/File.h>
#include <ewol/Debug.h>
#include <ewol/Font.h>
#include <etk/VectorType.h>

namespace ewol {
	extern "C" {
		typedef struct {
			int32_t f;
			int32_t s;
			int32_t t;
		}linkCoord_ts;
	};
	
	class OObject
	{
		public:
			OObject(void);
			virtual ~OObject(void);
		public:
			virtual void Draw(void) = 0;
		public:
			// use to crop element outside the display
			virtual void UpdateSize(etkFloat_t sizeX, etkFloat_t sizeY) { };
			// Move to the correct position display
			virtual void UpdateOrigin(etkFloat_t x, etkFloat_t y) { };
	};
};

#endif


#include <ewol/OObject/2DTextured.h>
#include <ewol/OObject/2DColored.h>
#include <ewol/OObject/2DText.h>
#include <ewol/OObject/2DTextColored.h>

