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
#include <ewol/renderer/EObject.h>

namespace ewol
{
	class EObjectManager
	{
		private:
			etk::Vector<ewol::EObject*> m_eObjectList; // all widget allocated ==> all time increment ... never removed ...
			etk::Vector<ewol::EObject*> m_eObjectAutoRemoveList; // all widget allocated
		public:
			EObjectManager(void);
			~EObjectManager(void);
			void Add(ewol::EObject* _object);
			void Rm(ewol::EObject* _object);
			int32_t GetNumberObject(void);
			
			void AutoRemove(ewol::EObject* _object);
			void RemoveAllAutoRemove(void);
			
			ewol::EObject* Get(const etk::UString& _name);
		private:
			void informOneObjectIsRemoved(ewol::EObject* _object);
	};
};

#endif
