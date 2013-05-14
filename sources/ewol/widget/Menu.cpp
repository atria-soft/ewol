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
#include <ewol/widget/Composer.h>
#include <ewol/widget/Label.h>

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
	widget::Sizer::SubWidgetRemoveAll();
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
	tmpObject->m_label = etk::UString("<left>") + label + "</left>";
	tmpObject->m_image = image;
	tmpObject->m_generateEvent = generateEvent;
	tmpObject->m_message = message;
	m_listElement.PushBack(tmpObject);
	if (-1 == tmpObject->m_parentId) {
		widget::Button * myButton = NULL;
		myButton = new widget::Button();
		if (NULL == myButton) {
			EWOL_ERROR("Allocation button error");
			return tmpObject->m_localId;
		}
		if (tmpObject->m_image.Size()!=0) {
			myButton->SetSubWidget(
			    new widget::Composer(widget::Composer::String,
			        etk::UString("<composer>\n") + 
			        "	<sizer mode=\"hori\">\n"
			        "		<image src=\"" + tmpObject->m_image + "\" size=\"8,8mm\"/>\n"
			        "		<label>" + label + "</label>\n"
			        "	</sizer>\n"
			        "</composer\n"));
		} else {
			myButton->SetSubWidget( new widget::Label(label) );
		}
		
		// add it in the widget list
		widget::Sizer::SubWidgetAdd(myButton);
		// keep the specific event ...
		myButton->RegisterOnEvent(this, widget::Button::eventPressed, widget::Button::eventPressed);
		tmpObject->m_widgetPointer = myButton;
	}
	return tmpObject->m_localId;
}

void widget::Menu::AddSpacer(void)
{
	EWOL_TODO("NOT now...");
}


void widget::Menu::OnReceiveMessage(const ewol::EMessage& _msg)
{
	/*
	if (true == ewol::Sizer$::OnReceiveMessage(_msg) {
		return true;
	}
	*/
	if (_msg.GetMessage() == widget::Button::eventPressed) {
		for(int32_t iii=0; iii<m_listElement.Size(); iii++) {
			if (_msg.GetCaller() == m_listElement[iii]->m_widgetPointer) {
				// 2 posible case (have a message or have a child ...
				if (m_listElement[iii]->m_generateEvent != NULL) {
					EWOL_DEBUG("Menu ==> Generate Event");
					// Send a multicast event ...
					SendMultiCast(m_listElement[iii]->m_generateEvent, m_listElement[iii]->m_message);
					if (NULL != m_widgetContextMenu) {
						EWOL_DEBUG("Mark the menu to remove ...");
						m_widgetContextMenu->RemoveObject();
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
					vec2 newPosition;
					ewol::Widget * eventFromWidget = static_cast<ewol::Widget*>(_msg.GetCaller());
					if (NULL != eventFromWidget) {
						vec2 tmpOri  = eventFromWidget->GetOrigin();
						vec2 tmpSize = eventFromWidget->GetSize();
						// calculate the correct position
						newPosition.setValue(tmpOri.x() + tmpSize.x()/2,
						                     tmpOri.y() );
					}
					
					m_widgetContextMenu->SetPositionMark(widget::CONTEXT_MENU_MARK_TOP, newPosition );
					
					widget::Sizer * mySizer = NULL;
					widget::Button * myButton = NULL;
					
					// TODO : Set a gird ...
					mySizer = new widget::Sizer(widget::Sizer::modeVert);
						mySizer->LockExpand(vec2(true,true));
						// set it in the pop-up-system : 
						m_widgetContextMenu->SetSubWidget(mySizer);
						
						for(int32_t jjj=m_listElement.Size()-1; jjj>=0; jjj--) {
							if (m_listElement[iii]!=NULL) {
								if (m_listElement[iii]->m_localId == m_listElement[jjj]->m_parentId) {
									myButton = new widget::Button();
									if (NULL == myButton) {
										EWOL_ERROR("Allocation Error");
									} else {
										/*if (m_listElement[jjj]->m_image.Size()!=0) {
											myButton->SetSubWidget(
											    new widget::Composer(widget::Composer::String,
											        etk::UString("<composer>\n") + 
											        "	<sizer mode=\"hori\">\n"
											        "		<image src=\"" + m_listElement[jjj]->m_image + "\" size=\"8,8mm\"/>\n"
											        "		<label exand=\"true,false\">" + m_listElement[jjj]->m_label + "</label>\n"
											        "	</sizer>\n"
											        "</composer\n"));
										} else */ {
											myButton->SetSubWidget( new widget::Label(etk::UString("<left>") + m_listElement[jjj]->m_label + "</left>") );
										}
										// set the image if one is present ...
										myButton->RegisterOnEvent(this, widget::Button::eventPressed, widget::Button::eventPressed);
										myButton->SetExpand(bvec2(true,false));
										myButton->SetFill(bvec2(true,false));
										// add it in the widget list
										mySizer->SubWidgetAdd(myButton);
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


void widget::Menu::OnObjectRemove(ewol::EObject * _removeObject)
{
	widget::Sizer::OnObjectRemove(_removeObject);
	if (m_widgetContextMenu == _removeObject) {
		m_widgetContextMenu = NULL;
	}
	for(int32_t jjj=0; jjj<m_listElement.Size(); jjj++) {
		if (NULL != m_listElement[jjj]) {
			if (m_listElement[jjj]->m_widgetPointer == _removeObject) {
				m_listElement[jjj]->m_widgetPointer = NULL;
			}
		}
	}
}

