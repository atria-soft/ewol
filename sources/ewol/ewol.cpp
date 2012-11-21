/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/ewol.h>
#include <ewol/widget/WidgetManager.h>
#include <ewol/os/eSystem.h>

#include <ewol/os/gui.h>

#undef __class__
#define __class__	"ewol"


int32_t ewol::Run(int32_t argc, const char* argv[]);
{
	
	// call standard RUN ...
	
}



void ewol::DisplayWindows(ewol::Windows * windows)
{
	// Remove current Focus :
	ewol::widgetManager::FocusSetDefault(NULL);
	ewol::widgetManager::FocusRelease();
	// set display of the windows :
	eSystem::SetCurrentWindows(windows);
	// Set the new default Focus :
	ewol::widgetManager::FocusSetDefault(windows);
}


void ewol::Stop(void)
{
	guiInterface::Stop();
}


void ewol::ChangeSize(etk::Vector2D<int32_t> size)
{
	guiInterface::ChangeSize(size);
}

void ewol::ChangePos(etk::Vector2D<int32_t> pos)
{
	guiInterface::ChangePos(pos);
}

void ewol::GetAbsPos(etk::Vector2D<int32_t>& pos)
{
	guiInterface::GetAbsPos(pos);
}


void ewol::KeyboardShow(void)
{
	guiInterface::KeyboardShow();
}

void ewol::KeyboardHide(void)
{
	guiInterface::KeyboardHide();
}

void ewol::ForceRedrawAll(void)
{
	eSystem::ForceRedrawAll();
}


ewol::specialKey_ts specialCurrentKey;

bool ewol::IsSetCapsLock(void)
{
	return specialCurrentKey.capLock;
}

bool ewol::IsSetShift(void)
{
	return specialCurrentKey.shift;
}

bool ewol::IsSetCtrl(void)
{
	return specialCurrentKey.ctrl;
}

bool ewol::IsSetMeta(void)
{
	return specialCurrentKey.meta;
}

bool ewol::IsSetAlt(void)
{
	return specialCurrentKey.alt;
}

bool ewol::IsSetAltGr(void)
{
	return specialCurrentKey.altGr;
}

bool ewol::IsSetVerNum(void)
{
	return specialCurrentKey.verNum;
}

bool ewol::IsSetInsert(void)
{
	return specialCurrentKey.insert;
}


etk::UString ewol::GetVersion(void)
{
	return EWOL_VERSION_TAG_NAME;
}


int32_t ewol::GetCurrentWidth(void)
{
	etk::Vector2D<int32_t> tmpSize = eSystem::GetSize();
	return tmpSize.x;
}

int32_t ewol::GetCurrentHeight(void)
{
	etk::Vector2D<int32_t> tmpSize = eSystem::GetSize();
	return tmpSize.y;
}


void ewol::PopUpWidgetPush(ewol::Widget * tmpWidget)
{
	ewol::Windows* tmpWindows = eSystem::GetCurrentWindows();
	if (NULL != tmpWindows && NULL != tmpWidget) {
		tmpWindows->PopUpWidgetPush(tmpWidget);
	}
}

void ewol::SetTitle(etk::UString title)
{
	guiInterface::SetTitle(title);
}


void ewol::InputEventTransfertWidget(ewol::Widget* source, ewol::Widget* destination)
{
	eSystem::InputEventTransfertWidget(source, destination);
}

void ewol::ForceOrientation(ewol::orientation_te orientation)
{
	guiInterface::ForceOrientation(orientation);
}


// ------------------------------------------------------------------------
//                 Command line arguments
// ------------------------------------------------------------------------

static etk::Vector<etk::UString*> listArgs;

void ewol::CmdLine::Clean(void)
{
	for (int32_t iii=0; iii<listArgs.Size(); iii++) {
		if (NULL != listArgs[iii]) {
			delete listArgs[iii];
			listArgs[iii] = NULL;
		}
	}
	listArgs.Clear();
}

int32_t ewol::CmdLine::Nb(void)
{
	return listArgs.Size();
}

etk::UString ewol::CmdLine::Get(int32_t id)
{
	if (id<0 && id>=listArgs.Size()) {
		return "";
	}
	if (NULL == listArgs[id]) {
		return "";
	}
	return *listArgs[id];
}

void ewol::CmdLine::Add(etk::UString& newElement)
{
	etk::UString* tmpString = new etk::UString(newElement);
	if (NULL != tmpString) {
		listArgs.PushBack(tmpString);
	}
}

int64_t ewol::GetTime(void)
{
	return guiInterface::GetTime();
}






