/**
 *******************************************************************************
 * @file ewol/Mesh/MeshObj.h
 * @brief ewol Mesh ;obj loader system (header)
 * @author Edouard DUPIN
 * @date 30/10/2012
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

#ifndef __MESH_OBJ_H__
#define __MESH_OBJ_H__

#include <etk/Types.h>
#include <etk/UString.h>
#include <ewol/Mesh/Mesh.h>

namespace ewol
{
	class MeshObj : public ewol::Mesh
	{
		public:
			MeshObj(etk::UString fileName);
			~MeshObj(void) { };
			virtual const char* GetType(void) { return "ewol::MeshObj"; };
	};
};





#endif

