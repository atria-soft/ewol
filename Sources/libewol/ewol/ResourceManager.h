/**
 *******************************************************************************
 * @file ewol/ResourcesManager.h
 * @brief ewol resources manager template (header)
 * @author Edouard DUPIN
 * @date 24/08/2012
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

#ifndef __RESOURCES_MANAGER_H__
#define __RESOURCES_MANAGER_H__

#include <etk/Types.h>
#include <ewol/Debug.h>
#include <ewol/Resource.h>
#include <ewol/openGL/Shader.h>
#include <ewol/openGL/Program.h>
#include <ewol/font/Font.h>
#include <ewol/font/TexturedFont.h>

namespace ewol
{
	namespace resource {
		void Init(void);
		void UnInit(void);
		
		// return the type of the resource ...
		bool Keep(etk::UString& filename, ewol::TexturedFont*& object);
		bool Keep(etk::UString& filename, ewol::Font*& object);
		bool Keep(etk::UString& filename, ewol::Program*& object);
		bool Keep(etk::UString& filename, ewol::Shader*& object);
		
		void Release(ewol::Resource*& object);
		void Release(ewol::TexturedFont*& object);
		void Release(ewol::Font*& object);
		void Release(ewol::Program*& object);
		void Release(ewol::Shader*& object);
	}
};


#endif

