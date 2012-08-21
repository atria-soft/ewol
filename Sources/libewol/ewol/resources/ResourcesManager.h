/**
 *******************************************************************************
 * @file ewol/resources/ResourcesManager.h
 * @brief ewol Resources manager system (header)
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

#ifndef __EWOL_RESOURCES_MANAGER_H__
#define __EWOL_RESOURCES_MANAGER_H__

#include <etk/Types.h>
#include <ewol/Debug.h>
#include <etk/File.h>
#include <ewol/resources/ResourcesImage.h>

namespace ewol
{
	namespace resourcesManager {
		void     Init(void);
		void     UnInit(void);
		uint32_t Add(ewol::ResourcesImage* object);
		void     Rm(ewol::ResourcesImage* object);
		//uint32_t Add(audio::Track* object);
		//void     Rm(audio::Track* object);
		
		ewol::ResourcesImage*  ImageKeep(etk::UString fileName, Vector2D<int32_t> size);
		void                   ImageRelease(ewol::ResourcesImage* object);
		//audio::Track* AudioKeep(etk::UString fileName, Vector2D<int32_t> size);
		//void          AudioRelease(audio::Track* object);
	};
};

#endif