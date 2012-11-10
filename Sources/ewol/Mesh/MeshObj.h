/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
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

