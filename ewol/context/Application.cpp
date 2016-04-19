/** @file
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#include <etk/types.h>
#include <ewol/context/Application.h>
#include <ewol/context/Context.h>


ewol::context::Application::Application() {
	
}

ewol::context::Application::~Application() {
	
}

void ewol::context::Application::onCreate(ewol::Context& _context) {
	
}

void ewol::context::Application::onStart(ewol::Context& _context) {
	
}

void ewol::context::Application::onResume(ewol::Context& _context) {
	
}

void ewol::context::Application::onPause(ewol::Context& _context) {
	
}

void ewol::context::Application::onStop(ewol::Context& _context) {
	
}

void ewol::context::Application::onDestroy(ewol::Context& _context) {
	
}

void ewol::context::Application::onKillDemand(ewol::Context& _context) {
	_context.exit(0);
}

