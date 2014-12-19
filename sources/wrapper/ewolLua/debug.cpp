/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#include "debug.h"

int32_t ewolLua::getLogId() {
	static int32_t g_val = etk::log::registerInstance("ewolLua");
	return g_val;
}
