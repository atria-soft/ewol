/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ewol/ewol.hpp>
#include <ewol/widget/Manager.hpp>
#include <ewol/context/Context.hpp>
#include <gale/gale.hpp>

#include <gale/context/commandLine.hpp>
#include <etk/os/FSNode.hpp>
#include <gale/Dimension.hpp>

#ifndef EWOL_VERSION
#define EWOL_VERSION "0.0.0"
#endif

std::string ewol::getVersion() {
	return EWOL_VERSION;
}


int32_t ewol::run(ewol::context::Application* _application,
                  int32_t _argc,
                  const char* _argv[]) {
	return gale::run(new ewol::Context(_application), _argc, _argv);
}


