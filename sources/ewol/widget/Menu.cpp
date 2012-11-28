/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/ewol.h>
#include <ewol/eObject/EObject.h>
#include <ewol/widget/WidgetManager.h>
#include <ewol/widget/Menu.h>
#include <ewol/widget/Button.h>
#include <ewol/widget/ContextMenu.h>
#include <ewol/widget/SizerVert.h>

#undef __class__
#define __class__	"Menu"

widget::Menu::Menu(void)
{
	m_staticId = 0;
	m_widgetContextMenu = NULL;
}

widget::Menu::~Menu(void)
{
	Clear();
}


void widget::Menu::SubWidgetRemoveAll(void)
{
	Clear();
	widget::SizerHori::SubWidgetRemoveAll();
}

void widget::Menu::SubWidgetAdd(ewol::Widget* newWidget)
{
	EWOL_ERROR("Not availlable");
}

void widget::Menu::SubWidgetRemove(ewol::Widget* newWidget)
{
	EWOL_ERROR("Not availlable");
}

void widget::Menu::SubWidgetUnLink(ewol::Widget* newWidget)
{
	EWOL_ERROR("Not availlable");
}

void widget::Menu::Clear(void)
{
	for( int32_t iii=0; iii < m_listElement.Size(); iii++) {
		if (m_listElement[iii] != NULL) {
			delete(m_listElement[iii]);
			m_listElement[iii] = NULL;
		}
	}
	m_listElement.Clear();
}

int32_t widget::Menu::AddTitle(etk::UString label, etk::UString image, const char * generateEvent, const etk::UString message)
{
	return Add(-1, label, image, generateEvent, message);
}

int32_t widget::Menu::Add(int32_t parent, etk::UString label, etk::UString image, const char * generateEvent, const etk::UString message)
{
	widget::MenuElement * tmpObject = new widget::MenuElement();
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
		widget::Button * myButton = NULL;
		myButton = new widget::Button(label);
		if (NULL == myButton) {
			EWOL_ERROR("Allocation button error");
			return tmpObject->m_localId;
		}
		// set the image if one is present ...
		myButton->SetImage(tmpObject->m_image);
		// add it in the widget list
		widget::SizerHori::SubWidgetAdd(myButton);
		// keep the specific event ...
		myButton->RegisterOnEvent(this, ewolEventButtonPressed, ewolEventButtonPressed);
		tmpObject->m_widgetPointer = myButton;
	}
	return tmpObject->m_localId;
}

void widget::Menu::AddSpacer(void)
{
	EWOL_TODO("NOT now...");
}


void widget::Menu::OnReceiveMessage(ewol::EObject * CallerObject, const char * eventId, etk::UString data)
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
						delete(m_widgetContextMenu);
						m_widgetContextMenu = NULL;
					}
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
					m_widgetContextMenu = new widget::ContextMenu();
					if (NULL == m_widgetContextMenu) {
						EWOL_ERROR("Allocation Error");
						return;
					}
					// Get the button widget : 
					etk::Vector2D<float> newPosition;
					ewol::Widget * eventFromWidget = static_cast<ewol::Widget*>(CallerObject);
					if (NULL != eventFromWidget) {
						etk::Vector2D<float> tmpOri  = eventFromWidget->GetOrigin();
						etk::Vector2D<float> tmpSize = eventFromWidget->GetSize();
						// calculate the correct position
						newPosition.x = tmpOri.x + tmpSize.x/2;
						newPosition.y = tmpOri.y;
					}
					
					m_widgetContextMenu->SetPositionMark(widget::CONTEXT_MENU_MARK_TOP, newPosition );
					
					widget::SizerVert * mySizerVert = NULL;
					widget::Button * myButton = NULL;
					
					mySizerVert = new widget::SizerVert();
						mySizerVert->LockExpendContamination(true);
						// set it in the pop-up-system : 
						m_widgetContextMenu->SubWidgetSet(mySizerVert);
						
						for(int32_t jjj=m_listElement.Size()-1; jjj>=0; jjj--) {
							if (m_listElement[iii]!=NULL) {
								if (m_listElement[iii]->m_localId == m_listElement[jjj]->m_parentId) {
									myButton = new widget::Button(m_listElement[jjj]->m_label);
									if (NULL == myButton) {
										EWOL_ERROR("Allocation Error");
									} else {
										// set the image if one is present ...
										myButton->SetImage(m_listElement[jjj]->m_image);
										myButton->RegisterOnEvent(this, ewolEventButtonPressed, ewolEventButtonPressed);
										myButton->SetExpendX(true);
										myButton->SetFillX(true);
										// add it in the widget list
										mySizerVert->SubWidgetAdd(myButton);
										m_listElement[jjj]->m_widgetPointer = myButton;
									}
								}
							}
						}
					ewol::WindowsPopUpAdd(m_widgetContextMenu);
				}
				return;
			}
		}
	}
}


void widget::Menu::OnObjectRemove(ewol::EObject * removeObject)
{
	widget::SizerHori::OnObjectRemove(removeObject);
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

