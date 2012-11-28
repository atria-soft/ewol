/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/ewol.h>
#include <ewol/widget/WidgetManager.h>
#include <ewol/renderer/os/eSystem.h>

#include <ewol/renderer/os/gui.h>
#include <ewol/commandLine.h>
#include <etk/os/FSNode.h>

#undef __class__
#define __class__	"ewol"


int32_t ewol::Run(int32_t argc, const char* argv[])
{
	
	EWOL_DEBUG("Store commangLine in the specific system");
	ewol::commandLine::Clean();
	for( int32_t i=1 ; i<argc; i++) {
		EWOL_INFO("commandLine : \"" << argv[i] << "\"" );
		if (0==strncmp("-l0", argv[i], 256)) {
			GeneralDebugSetLevel(etk::LOG_LEVEL_NONE);
		} else if (0==strncmp("-l1", argv[i], 256)) {
			GeneralDebugSetLevel(etk::LOG_LEVEL_CRITICAL);
		} else if (0==strncmp("-l2", argv[i], 256)) {
			GeneralDebugSetLevel(etk::LOG_LEVEL_ERROR);
		} else if (0==strncmp("-l3", argv[i], 256)) {
			GeneralDebugSetLevel(etk::LOG_LEVEL_WARNING);
		} else if (0==strncmp("-l4", argv[i], 256)) {
			GeneralDebugSetLevel(etk::LOG_LEVEL_INFO);
		} else if (0==strncmp("-l5", argv[i], 256)) {
			GeneralDebugSetLevel(etk::LOG_LEVEL_DEBUG);
		} else if(    0==strncmp("-l6", argv[i], 256)
		           || 0==strncmp("-l7", argv[i], 256)
		           || 0==strncmp("-l8", argv[i], 256)
		           || 0==strncmp("-l9", argv[i], 256)) {
			GeneralDebugSetLevel(etk::LOG_LEVEL_VERBOSE);
		} else {
			etk::UString tmpString(argv[i]);
			ewol::commandLine::Add(tmpString);
		}
	}
	// call standard RUN ...
	int32_t error = guiInterface::main(argc, argv);
	
	ewol::commandLine::Clean();
	
	return error;
}


void ewol::Stop(void)
{
	guiInterface::Stop();
}


void ewol::WindowsSet(ewol::Windows * windows)
{
	// Remove current Focus :
	ewol::widgetManager::FocusSetDefault(NULL);
	ewol::widgetManager::FocusRelease();
	// set display of the windows :
	eSystem::SetCurrentWindows(windows);
	// Set the new default Focus :
	ewol::widgetManager::FocusSetDefault(windows);
}

void ewol::WindowsPopUpAdd(ewol::Widget * tmpWidget)
{
	ewol::Windows* tmpWindows = eSystem::GetCurrentWindows();
	if (NULL != tmpWindows && NULL != tmpWidget) {
		tmpWindows->PopUpWidgetPush(tmpWidget);
	}
}

void ewol::ChangeSize(etk::Vector2D<int32_t> size)
{
	guiInterface::ChangeSize(size);
}

void ewol::ChangePos(etk::Vector2D<int32_t> pos)
{
	guiInterface::ChangePos(pos);
}

void ewol::ForceRedrawAll(void)
{
	eSystem::ForceRedrawAll();
}

void ewol::RequestUpdateSize(void)
{
	eSystem::RequestUpdateSize();
}

void ewol::Keyboard(bool hide)
{
	if (true == hide) {
		guiInterface::KeyboardHide();
	} else {
		guiInterface::KeyboardShow();
	}
}
void ewol::SetTitle(etk::UString title)
{
	guiInterface::SetTitle(title);
}

etk::UString ewol::GetVersion(void)
{
	return EWOL_VERSION_TAG_NAME;
}

int64_t ewol::GetTime(void)
{
	return guiInterface::GetTime();
}

void ewol::InputEventTransfertWidget(ewol::Widget* source, ewol::Widget* destination)
{
	eSystem::InputEventTransfertWidget(source, destination);
}

void ewol::ForceOrientation(ewol::orientation_te orientation)
{
	guiInterface::ForceOrientation(orientation);
}






