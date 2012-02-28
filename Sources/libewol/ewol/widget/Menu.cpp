/**
 *******************************************************************************
 * @file ewol/widget/Menu.cpp
 * @brief ewol Menu widget system (Sources)
 * @author Edouard DUPIN
 * @date 16/02/2012
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
#include <ewol/EObject.h>
#include <ewol/WidgetManager.h>
#include <ewol/widget/Menu.h>
#include <ewol/widget/Button.h>
#include <ewol/widget/ContextMenu.h>
#include <ewol/widget/SizerVert.h>

#undef __class__
#define __class__	"ewol::Menu"

ewol::Menu::Menu(void)
{
	m_staticId = 0;
	m_widgetContextMenu = NULL;
}

ewol::Menu::~Menu(void)
{
	Clear();
}

void ewol::Menu::SubWidgetRemoveAll(void)
{
	EWOL_ERROR("Not availlable");
}

void ewol::Menu::SubWidgetAdd(ewol::Widget* newWidget)
{
	EWOL_ERROR("Not availlable");
}

void ewol::Menu::SubWidgetRemove(ewol::Widget* newWidget)
{
	EWOL_ERROR("Not availlable");
}

void ewol::Menu::SubWidgetUnLink(ewol::Widget* newWidget)
{
	EWOL_ERROR("Not availlable");
}

void ewol::Menu::Clear(void)
{
	for( int32_t iii=0; iii < m_listElement.Size(); iii++) {
		if (m_listElement[iii] != NULL) {
			delete(m_listElement[iii]);
			m_listElement[iii] = NULL;
		}
	}
	m_listElement.Clear();
}

int32_t ewol::Menu::AddTitle(etk::UString label, etk::UString image, const char * generateEvent, const etk::UString message)
{
	return Add(-1, label, image, generateEvent, message);
}

int32_t ewol::Menu::Add(int32_t parent, etk::UString label, etk::UString image, const char * generateEvent, const etk::UString message)
{
	ewol::MenuElement * tmpObject = new ewol::MenuElement();
	if (NULL == tmpObject) {
		EWOL_ERROR("Allocation problem");
		return -1;
	}
	tmpObject->m_localId = m_staticId++;
	tmpObject->m_parentId = parent;
	tmpObject->m_widgetPointer = NULL;
	tmpObject->m_label = label;
	tmpObject->m_image = image;
	tmpObject->m_generateEvent = generateEvent;
	tmpObject->m_message = message;
	m_listElement.PushBack(tmpObject);
	if (-1 == tmpObject->m_parentId) {
		ewol::Button * myButton = NULL;
		myButton = new ewol::Button(label);
		if (NULL == myButton) {
			EWOL_ERROR("Allocation button error");
			return tmpObject->m_localId;;
		}
		ewol::SizerHori::SubWidgetAdd(myButton);
		myButton->RegisterOnEvent(this, ewolEventButtonPressed, ewolEventButtonPressed);
		tmpObject->m_widgetPointer = myButton;
	}
	return tmpObject->m_localId;
}

void ewol::Menu::AddSpacer(void)
{
	EWOL_TODO("NOT now...");
}


/**
 * @brief Receive a message from an other EObject with a specific eventId and data
 * @param[in] CallerObject Pointer on the EObject that information came from
 * @param[in] eventId Message registered by this class
 * @param[in] data Data registered by this class
 * @return ---
 */
void ewol::Menu::OnReceiveMessage(ewol::EObject * CallerObject, const char * eventId, etk::UString data)
{
	/*
	if (true == ewol::SizerHori::OnReceiveMessage(CallerObject, eventId, data)) {
		return true;
	}
	*/
	if (eventId == ewolEventButtonPressed) {
		for(int32_t iii=0; iii<m_listElement.Size(); iii++) {
			if (CallerObject == m_listElement[iii]->m_widgetPointer) {
				// 2 posible case (have a message or have a child ...
				if (m_listElement[iii]->m_generateEvent != NULL) {
					EWOL_DEBUG("Menu ==> Generate Event");
					// Send a multicast event ...
					SendMultiCast(m_listElement[iii]->m_generateEvent, m_listElement[iii]->m_message);
					/*
					if (NULL != m_widgetContextMenu) {
						m_widgetContextMenu->MarkToRemove();
					}
					*/
					m_widgetContextMenu = NULL;
					return;
				} else{
					EWOL_DEBUG("Menu ==> Load Sub Menu");
					bool findChild = false;
					for(int32_t jjj=0; jjj<m_listElement.Size(); jjj++) {
						if (m_listElement[iii]->m_localId == m_listElement[jjj]->m_parentId) {
							findChild = true;
							break;
						}
					}
					if (false == findChild) {
						EWOL_WARNING("Event on menu element with no child an no event... label=" << m_listElement[iii]->m_label);
						return;
					}
					// create a context menu : 
					m_widgetContextMenu = new ewol::ContextMenu();
					if (NULL == m_widgetContextMenu) {
						EWOL_ERROR("Allocation Error");
						return;
					}
					// Get the button widget : 
					coord2D_ts newPosition;
					ewol::Widget * eventFromWidget = static_cast<ewol::Widget*>(CallerObject);
					if (NULL != eventFromWidget) {
						coord2D_ts tmpOri  = eventFromWidget->GetOrigin();
						coord2D_ts tmpSize = eventFromWidget->GetSize();
						// calculate the correct position
						newPosition.x = tmpOri.x + tmpSize.x/2;
						newPosition.y = tmpOri.y + tmpSize.y;
					}
					
					m_widgetContextMenu->SetPositionMark(ewol::CONTEXT_MENU_MARK_TOP, newPosition );
					
					ewol::SizerVert * mySizerVert = NULL;
					ewol::Button * myButton = NULL;
					
					mySizerVert = new ewol::SizerVert();
						mySizerVert->LockExpendContamination(true);
						// set it in the pop-up-system : 
						m_widgetContextMenu->SubWidgetSet(mySizerVert);
						
						for(int32_t jjj=0; jjj<m_listElement.Size(); jjj++) {
							if (m_listElement[iii]->m_localId == m_listElement[jjj]->m_parentId) {
								myButton = new ewol::Button(m_listElement[jjj]->m_label);
								if (NULL == myButton) {
									EWOL_ERROR("Allocation Error");
								}
								m_listElement[jjj]->m_widgetPointer = myButton;
								myButton->RegisterOnEvent(this, ewolEventButtonPressed, ewolEventButtonPressed, "");
								myButton->SetExpendX(true);
								myButton->SetFillX(true);
								myButton->SetAlignement(ewol::TEXT_ALIGN_LEFT);
								mySizerVert->SubWidgetAdd(myButton);
							}
						}
					ewol::PopUpWidgetPush(m_widgetContextMenu);
				}
				return;
			}
		}
	}
}
