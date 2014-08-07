/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD 3 clauses (see license file)
 */

#ifndef __EWOL_OBJECT_REMOVE_EVENT_H__
#define __EWOL_OBJECT_REMOVE_EVENT_H__

#include <etk/types.h>
#include <ewol/object/Object.h>

namespace ewol {
	namespace object {
		class RemoveEvent {
			public:
				virtual void onObjectRemove(const std::shared_ptr<ewol::Object>& _object) = 0;
			public:
				RemoveEvent();
				virtual ~RemoveEvent();
		};
	}
};

#endif
