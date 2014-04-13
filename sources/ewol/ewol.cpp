/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/ewol.h>
#include <ewol/widget/Manager.h>
#include <ewol/context/Context.h>

#include <ewol/context/commandLine.h>
#include <etk/os/FSNode.h>
#include <ewol/Dimension.h>
#include <date/date.h>

#undef __class__
#define __class__ "ewol"
#ifndef EWOL_VERSION
#define EWOL_VERSION "0.0.0"
#endif

std::string ewol::getCompilationMode(void) {
	#ifdef MODE_RELEASE
		return "Release";
	#else
		return "Debug";
	#endif
}

std::string ewol::getBoardType(void) {
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

std::string ewol::getVersion(void) {
	return EWOL_VERSION;
}

