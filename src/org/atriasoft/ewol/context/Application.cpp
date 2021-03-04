/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <etk/types.hpp>
#include <ewol/context/Application.hpp>
#include <ewol/context/Context.hpp>

#include <etk/typeInfo.hpp>
ETK_DECLARE_TYPE(EwolApplication);

EwolApplication::Application() {
	
}

EwolApplication::~Application() {
	
}

void EwolApplication::onCreate(EwolContext _context) {
	
}

void EwolApplication::onStart(EwolContext _context) {
	
}

void EwolApplication::onResume(EwolContext _context) {
	
}

void EwolApplication::onPause(EwolContext _context) {
	
}

void EwolApplication::onStop(EwolContext _context) {
	
}

void EwolApplication::onDestroy(EwolContext _context) {
	
}

void EwolApplication::onKillDemand(EwolContext _context) {
	_context.exit(0);
}

