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
#define __class__	"Menu"

ewol::Menu::Menu(void)
{
	m_staticId = 0;
	m_widgetContextMenu = NULL;
}

ewol::Menu::~Menu(void)
{
	Clear();
}


//!< EObject name :
extern const char * const ewol::TYPE_EOBJECT_WIDGET_MENU = "Menu";

/**
 * @brief Check if the object has the specific type.
 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
 * @param[in] objectType type of the object we want to check
 * @return true if the object is compatible, otherwise false
 */
bool ewol::Menu::CheckObjectType(const char * const objectType)
{
	if (NULL == objectType) {
		EWOL_ERROR("check error : \"" << ewol::TYPE_EOBJECT_WIDGET_MENU << "\" != NULL(pointer) ");
		return false;
	}
	if (objectType == ewol::TYPE_EOBJECT_WIDGET_MENU) {
		return true;
	} else {
		if(true == ewol::SizerHori::CheckObjectType(objectType)) {
			return true;
		}
		EWOL_ERROR("check error : \"" << ewol::TYPE_EOBJECT_WIDGET_MENU << "\" != \"" << objectType << "\"");
		return false;
	}
}

/**
 * @brief Get the current Object type of the EObject
 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
 * @param[in] objectType type description
 * @return true if the object is compatible, otherwise false
 */
const char * const ewol::Menu::GetObjectType(void)
{
	return ewol::TYPE_EOBJECT_WIDGET_MENU;
}


void ewol::Menu::SubWidgetRemoveAll(void)
{
	Clear();
	ewol::SizerHori::SubWidgetRemoveAll();
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
			return tmpObject->m_localId;
		}
		// set the image if one is present ...
		myButton->SetImage(tmpObject->m_image);
		// add it in the widget list
		ewol::SizerHori::SubWidgetAdd(myButton);
		// keep the specific event ...
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
					if (NULL != m_widgetContextMenu) {
						EWOL_DEBUG("Mark the menu to remove ...");
						m_widgetContextMenu->MarkToRemove();
					}
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
					Vector2D<float> newPosition;
					ewol::Widget * eventFromWidget = EWOL_CAST_WIDGET(CallerObject);
					if (NULL != eventFromWidget) {
						Vector2D<float> tmpOri  = eventFromWidget->GetOrigin();
						Vector2D<float> tmpSize = eventFromWidget->GetSize();
						// calculate the correct position
						newPosition.x = tmpOri.x + tmpSize.x/2;
						newPosition.y = tmpOri.y;
					}
					
					m_widgetContextMenu->SetPositionMark(ewol::CONTEXT_MENU_MARK_TOP, newPosition );
					
					ewol::SizerVert * mySizerVert = NULL;
					ewol::Button * myButton = NULL;
					
					mySizerVert = new ewol::SizerVert();
						mySizerVert->LockExpendContamination(true);
						// set it in the pop-up-system : 
						m_widgetContextMenu->SubWidgetSet(mySizerVert);
						
						for(int32_t jjj=m_listElement.Size()-1; jjj>=0; jjj--) {
							if (m_listElement[iii]!=NULL) {
								if (m_listElement[iii]->m_localId == m_listElement[jjj]->m_parentId) {
									myButton = new ewol::Button(m_listElement[jjj]->m_label);
									if (NULL == myButton) {
										EWOL_ERROR("Allocation Error");
									} else {
										// set the image if one is present ...
										myButton->SetImage(m_listElement[jjj]->m_image);
										myButton->RegisterOnEvent(this, ewolEventButtonPressed, ewolEventButtonPressed);
										myButton->SetExpendX(true);
										myButton->SetFillX(true);
										myButton->SetAlignement(ewol::TEXT_ALIGN_LEFT);
										// add it in the widget list
										mySizerVert->SubWidgetAdd(myButton);
										m_listElement[jjj]->m_widgetPointer = myButton;
									}
								}
							}
						}
					ewol::PopUpWidgetPush(m_widgetContextMenu);
				}
				return;
			}
		}
	}
}

/**
 * @brief Inform object that an other object is removed ...
 * @param[in] removeObject Pointer on the EObject remeved ==> the user must remove all reference on this EObject
 * @note : Sub classes must call this class
 * @return ---
 */
void ewol::Menu::OnObjectRemove(ewol::EObject * removeObject)
{
	ewol::SizerHori::OnObjectRemove(removeObject);
	if (m_widgetContextMenu == removeObject) {
		m_widgetContextMenu = NULL;
	}
	for(int32_t jjj=0; jjj<m_listElement.Size(); jjj++) {
		if (NULL != m_listElement[jjj]) {
			if (m_listElement[jjj]->m_widgetPointer == removeObject) {
				m_listElement[jjj]->m_widgetPointer = NULL;
			}
		}
	}
}

