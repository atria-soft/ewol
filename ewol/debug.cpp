/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */

#include <ewol/debug.hpp>

int32_t ewol::getLogId() {
	static int32_t g_val = elog::registerInstance("ewol");
	return g_val;
}
