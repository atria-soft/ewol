/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */


#include <appl/debug.h>

int32_t appl::getLogId() {
	static int32_t g_val = etk::log::registerInstance("test_ewol");
	return g_val;
}
