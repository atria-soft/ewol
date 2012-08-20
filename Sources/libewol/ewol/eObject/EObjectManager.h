/**
 *******************************************************************************
 * @file ewol/eObject/EObjectManager.h
 * @brief basic ewol Object Manager (Header)
 * @author Edouard DUPIN
 * @date 27/02/2012
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

#ifndef __EWOL_E_OBJECT_MANAGER_H__
#define __EWOL_E_OBJECT_MANAGER_H__

#include <etk/Types.h>
#include <ewol/Debug.h>
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

