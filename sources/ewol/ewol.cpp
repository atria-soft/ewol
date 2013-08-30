/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/ewol.h>
#include <ewol/widget/WidgetManager.h>
#include <ewol/renderer/eSystem.h>

#include <ewol/commandLine.h>
#include <etk/os/FSNode.h>
#include <ewol/Dimension.h>
#include <date/date.h>

#undef __class__
#define __class__	"ewol"

int32_t ewol::Run(int32_t _argc, const char* _argv[])
{
	if (NULL!=_argv) {
		etk::SetArgZero(_argv[0]);
	}
	// init display convertions:
	ewol::dimension::Init();
	
	EWOL_DEBUG("Store commangLine in the specific system");
	ewol::commandLine::Clean();
	for( int32_t i=1 ; i<_argc; i++) {
		EWOL_INFO("commandLine : \"" << _argv[i] << "\"" );
		if (0==strncmp("-l0", _argv[i], 256)) {
			GeneralDebugSetLevel(etk::LOG_LEVEL_NONE);
		} else if (0==strncmp("-l1", _argv[i], 256)) {
			GeneralDebugSetLevel(etk::LOG_LEVEL_CRITICAL);
		} else if (0==strncmp("-l2", _argv[i], 256)) {
			GeneralDebugSetLevel(etk::LOG_LEVEL_ERROR);
		} else if (0==strncmp("-l3", _argv[i], 256)) {
			GeneralDebugSetLevel(etk::LOG_LEVEL_WARNING);
		} else if (0==strncmp("-l4", _argv[i], 256)) {
			GeneralDebugSetLevel(etk::LOG_LEVEL_INFO);
		} else if (0==strncmp("-l5", _argv[i], 256)) {
			GeneralDebugSetLevel(etk::LOG_LEVEL_DEBUG);
		} else if(    0==strncmp("-l6", _argv[i], 256)
		           || 0==strncmp("-l7", _argv[i], 256)
		           || 0==strncmp("-l8", _argv[i], 256)
		           || 0==strncmp("-l9", _argv[i], 256)) {
			GeneralDebugSetLevel(etk::LOG_LEVEL_VERBOSE);
		} else {
			ewol::commandLine::Add(_argv[i]);
		}
	}
	// call standard RUN ...
	int32_t error = ewol::eSystem::main(_argc, _argv);
	
	ewol::commandLine::Clean();
	
	return error;
}

etk::UString ewol::GetCompilationMode(void)
{
	#ifdef MODE_RELEASE
		return "Release";
	#else
		return "Debug";
	#endif
}

etk::UString ewol::GetBoardType(void)
{
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

/*


void ewol::WindowsSet(ewol::Windows* _windows)
{
	// Remove current Focus :
	ewol::widgetManager::FocusSetDefault(NULL);
	ewol::widgetManager::FocusRelease();
	// set display of the windows :
	eSystem::SetCurrentWindows(_windows);
	// Set the new default Focus :
	ewol::widgetManager::FocusSetDefault(_windows);
}

void ewol::WindowsPopUpAdd(ewol::Widget* _tmpWidget)
{
	ewol::Windows* tmpWindows = eSystem::GetCurrentWindows();
	if (NULL != tmpWindows && NULL != _tmpWidget) {
		tmpWindows->PopUpWidgetPush(_tmpWidget);
	}
}

void ewol::ChangeSize(const ivec2& _size)
{
	guiInterface::ChangeSize(_size);
}

void ewol::ChangePos(const ivec2& _pos)
{
	guiInterface::ChangePos(_pos);
}

void ewol::ForceRedrawAll(void)
{
	eSystem::ForceRedrawAll();
}

void ewol::RequestUpdateSize(void)
{
	eSystem::RequestUpdateSize();
}

void ewol::Keyboard(bool _hide)
{
	if (true == _hide) {
		guiInterface::KeyboardHide();
	} else {
		guiInterface::KeyboardShow();
	}
}
void ewol::SetTitle(const etk::UString& _title)
{
	etk::UString title = _title;
	guiInterface::SetTitle(title);
}
*/
etk::UString ewol::GetVersion(void)
{
	#define FIRST_YEAR (2011)
	etk::UString tmpOutput = (date::GetYear()-FIRST_YEAR);
	tmpOutput += ".";
	tmpOutput += date::GetMonth();
	tmpOutput += ".";
	tmpOutput += date::GetDay();
	return tmpOutput;
}

int64_t ewol::GetTime(void)
{
	return ewol::eSystem::GetTime();
}
/*
void ewol::InputEventTransfertWidget(ewol::Widget* _source, ewol::Widget* _destination)
{
	eSystem::InputEventTransfertWidget(_source, _destination);
}

void ewol::ForceOrientation(ewol::orientation_te _orientation)
{
	guiInterface::ForceOrientation(_orientation);
}


void ewol::SetIcon(const etk::UString& _icon)
{
	guiInterface::SetIcon(_icon);
}
*/



