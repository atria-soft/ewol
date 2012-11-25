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

#undef __class__
#define __class__	"ewol"


int32_t ewol::Run(int32_t argc, const char* argv[])
{
	// call standard RUN ...
	return guiInterface::main(argc, argv);
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






