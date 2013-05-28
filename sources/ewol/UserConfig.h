/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_USER_CONFIG_H__
#define __EWOL_USER_CONFIG_H__

#include <etk/types.h>
#include <etk/Stream.h>
#include <ewol/eObject/EObject.h>

namespace ewol
{
	namespace userConfig
	{
		void Init(void);
		void UnInit(void);
		const ewol::EObject* GetUserConfig(const etk::UString& _name);
		void AddUserConfig(ewol::EObject* _newConfig);
		//void RmUserConfig(ewol::EObject* _newConfig); // note : To remove user config ==> just destroy it ==> simple ..
		void SetConfigName(const etk::UString& _fileName="USERDATA:generalConfig.xml");
		bool Load(void);
		bool Save(void);
	};
};

#endif

