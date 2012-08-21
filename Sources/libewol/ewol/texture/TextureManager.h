/**
 *******************************************************************************
 * @file ewol/texture/textureManager.h
 * @brief ewol Texture manager system (header)
 * @author Edouard DUPIN
 * @date 21/08/2012
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

#ifndef __EWOL_TEXTURE_MANAGER_H__
#define __EWOL_TEXTURE_MANAGER_H__

#include <etk/Types.h>
#include <ewol/Debug.h>
#include <etk/File.h>
#include <ewol/texture/Texture.h>

namespace ewol
{
	namespace textureManager {
		void     Init(void);
		void     UnInit(void);
		uint32_t Add(ewol::Texture *object); // note : Return the UniqueID ...
		void     Rm(ewol::Texture *object);
		void     Update(ewol::Texture *object);
		// Specific to load or update the data in the openGl context ==> system use only
		void     UpdateContext(void);
		void     OpenGlContextHasBeenDestroyed(void);
	};
};

#endif