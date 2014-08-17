/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#ifndef __EWOL_OBJECT_REMOVE_EVENT_H__
#define __EWOL_OBJECT_REMOVE_EVENT_H__

#include <etk/types.h>
#include <ewol/object/Object.h>

namespace ewol {
	namespace object {
		class RemoveEvent {
			public:
				RemoveEvent();
				virtual ~RemoveEvent();
		};
	}
};

#endif
