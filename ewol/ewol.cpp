/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */

#include <ewol/ewol.h>
#include <ewol/widget/Manager.h>
#include <ewol/context/Context.h>
#include <gale/gale.h>

#include <gale/context/commandLine.h>
#include <etk/os/FSNode.h>
#include <gale/Dimension.h>

#ifndef EWOL_VERSION
#define EWOL_VERSION "0.0.0"
#endif

std::string ewol::getCompilationMode() {
	#ifdef MODE_RELEASE
		return "Release";
	#else
		return "Debug";
	#endif
}

std::string ewol::getBoardType() {
	#ifdef __TARGET_OS__Linux
		return "Linux";
	#elif defined(__TARGET_OS__Android)
		return "Android";
	#elif defined(__TARGET_OS__Windows)
		return "Windows";
	#elif defined(__TARGET_OS__IOs)
		return "IOs";
	#elif defined(__TARGET_OS__MacOs)
		return "MacOs";
	#else
		return "Unknown";
	#endif
}

std::string ewol::getVersion() {
	return EWOL_VERSION;
}
int64_t ewol::getTime() {
	return gale::getTime();
}

int32_t ewol::run(ewol::context::Application* _application,
                  int32_t _argc,
                  const char* _argv[]) {
	return gale::run(new ewol::Context(_application), _argc, _argv);
}


