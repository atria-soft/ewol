/**
 *******************************************************************************
 * @file ewol/ewol.cpp
 * @brief Main code of ewol (sources)
 * @author Edouard DUPIN
 * @date 17/10/2011
 * @par Project
 * ewol
 *
 * @par Copyright
 * Copyright 2011 Edouard DUPIN, all right reserved
 *
 * This software is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY.
 *
 * Licence summary : 
 *    You can modify and redistribute the sources code and binaries.
 *    You can send me the bug-fix
 *
 * Term of the licence in in the file licence.txt.
 *
 *******************************************************************************
 */


#include <ewol/ewol.h>
#include <ewol/widget/WidgetManager.h>
#include <ewol/os/eSystem.h>

#include <ewol/os/gui.h>

#undef __class__
#define __class__	"ewol"

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


void ewol::ChangeSize(Vector2D<int32_t> size)
{
	guiInterface::ChangeSize(size);
}

void ewol::ChangePos(Vector2D<int32_t> pos)
{
	guiInterface::ChangePos(pos);
}

void ewol::GetAbsPos(Vector2D<int32_t>& pos)
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


eSystem::specialKey_ts specialCurrentKey;
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
	Vector2D<int32_t> tmpSize = eSystem::GetSize();
	return tmpSize.x;
}

int32_t ewol::GetCurrentHeight(void)
{
	Vector2D<int32_t> tmpSize = eSystem::GetSize();
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


/**
 * @brief This is to transfert the event from one widget to another one
 * @param source the widget where the event came from
 * @param destination the widget where the event mitgh be generated now
 * @return ---
 */
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