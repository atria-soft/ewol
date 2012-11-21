/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_E_OBJECT_MANAGER_H__
#define __EWOL_E_OBJECT_MANAGER_H__

#include <etk/types.h>
#include <ewol/eObject/EObject.h>

namespace ewol {
	namespace EObjectManager {
		void  Init(         void);
		void  UnInit(       void);
		void  Add(          ewol::EObject* object);
		void  Rm(           ewol::EObject* object);
		
		void  AutoRemove(   ewol::EObject* object);
		void  RemoveAllAutoRemove(void);
	};
};

#endif

