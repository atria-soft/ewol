/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/ewol.h>
#include <ewol/widget/WidgetManager.h>
#include <ewol/widget/Menu.h>
#include <ewol/widget/Button.h>
#include <ewol/widget/ContextMenu.h>
#include <ewol/widget/Composer.h>
#include <ewol/widget/Label.h>
#include <ewol/widget/Windows.h>

#undef __class__
#define __class__ "Menu"

ewol::widget::Menu::Menu(void) {
	addObjectType("ewol::widget::Menu");
	m_staticId = 0;
	m_widgetContextMenu = NULL;
}

ewol::widget::Menu::~Menu(void) {
	clear();
}

void ewol::widget::Menu::subWidgetRemoveAll(void) {
	clear();
	widget::Sizer::subWidgetRemoveAll();
}

int32_t ewol::widget::Menu::subWidgetAdd(ewol::Widget* _newWidget) {
	EWOL_ERROR("Not availlable");
	return -1;
}

void ewol::widget::Menu::subWidgetRemove(ewol::Widget* _newWidget) {
	EWOL_ERROR("Not availlable");
}

void ewol::widget::Menu::subWidgetUnLink(ewol::Widget* _newWidget) {
	EWOL_ERROR("Not availlable");
}

void ewol::widget::Menu::clear(void) {
	for (size_t iii=0; iii < m_listElement.size(); iii++) {
		if (m_listElement[iii] != NULL) {
			delete(m_listElement[iii]);
			m_listElement[iii] = NULL;
		}
	}
	m_listElement.clear();
}

int32_t ewol::widget::Menu::addTitle(std::string _label,
                               std::string _image,
                               const char * _generateEvent,
                               const std::string _message) {
	return add(-1, _label, _image, _generateEvent, _message);
}

int32_t ewol::widget::Menu::add(int32_t _parent,
                          std::string _label,
                          std::string _image,
                          const char *_generateEvent,
                          const std::string _message) {
	ewol::widget::MenuElement *tmpObject = new ewol::widget::MenuElement();
	if (NULL == tmpObject) {
		EWOL_ERROR("Allocation problem");
		return -1;
	}
	tmpObject->m_localId = m_staticId++;
	tmpObject->m_parentId = _parent;
	tmpObject->m_widgetPointer = NULL;
	tmpObject->m_label = std::string("<left>") + _label + "</left>";
	tmpObject->m_image = _image;
	tmpObject->m_generateEvent = _generateEvent;
	tmpObject->m_message = _message;
	m_listElement.push_back(tmpObject);
	if (-1 == tmpObject->m_parentId) {
		widget::Button *myButton = NULL;
		myButton = new widget::Button();
		if (NULL == myButton) {
			EWOL_ERROR("Allocation button error");
			return tmpObject->m_localId;
		}
		if (tmpObject->m_image.size()!=0) {
			myButton->setSubWidget(
			    new widget::Composer(widget::Composer::String,
			        std::string("<composer>\n") + 
			        "	<sizer mode=\"hori\">\n"
			        "		<image src=\"" + tmpObject->m_image + "\" size=\"8,8mm\"/>\n"
			        "		<label>" + tmpObject->m_label + "</label>\n"
			        "	</sizer>\n"
			        "</composer>\n"));
		} else {
			myButton->setSubWidget( new widget::Label(tmpObject->m_label) );
		}
		
		// add it in the widget list
		widget::Sizer::subWidgetAdd(myButton);
		// keep the specific event ...
		myButton->registerOnEvent(this, widget::Button::eventPressed, widget::Button::eventPressed);
		tmpObject->m_widgetPointer = myButton;
	}
	return tmpObject->m_localId;
}

void ewol::widget::Menu::addSpacer(void) {
	EWOL_TODO("NOT now...");
}


void ewol::widget::Menu::onReceiveMessage(const ewol::EMessage& _msg) {
	/*
	if (true == ewol::sizer::onReceiveMessage(_msg) {
		return true;
	}
	*/
	EWOL_ERROR(" receive message : " << _msg);
	if (_msg.getMessage() == widget::Button::eventPressed) {
		for (size_t iii=0; iii<m_listElement.size(); iii++) {
			if (_msg.getCaller() == m_listElement[iii]->m_widgetPointer) {
				// 2 posible case (have a message or have a child ...
				if (m_listElement[iii]->m_generateEvent != NULL) {
					EWOL_DEBUG("Menu  == > generate Event");
					// Send a multicast event ...
					sendMultiCast(m_listElement[iii]->m_generateEvent, m_listElement[iii]->m_message);
					if (NULL != m_widgetContextMenu) {
						EWOL_DEBUG("Mark the menu to remove ...");
						m_widgetContextMenu->removeObject();
						m_widgetContextMenu = NULL;
					}
					return;
				} else{
					EWOL_DEBUG("Menu  == > load Sub Menu");
					bool findChild = false;
					for (size_t jjj=0; jjj<m_listElement.size(); jjj++) {
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
					// get the button widget : 
					vec2 newPosition;
					ewol::Widget * eventFromWidget = static_cast<ewol::Widget*>(_msg.getCaller());
					if (NULL != eventFromWidget) {
						vec2 tmpOri  = eventFromWidget->getOrigin();
						vec2 tmpSize = eventFromWidget->getSize();
						// calculate the correct position
						newPosition.setValue(tmpOri.x() + tmpSize.x()/2,
						                     tmpOri.y() );
					}
					
					m_widgetContextMenu->setPositionMark(widget::ContextMenu::markTop, newPosition );
					
					widget::Sizer * mySizer = NULL;
					widget::Button * myButton = NULL;
					
					mySizer = new widget::Sizer(widget::Sizer::modeVert);
					if (NULL != mySizer) {
						mySizer->lockExpand(vec2(true,true));
						// set it in the pop-up-system : 
						m_widgetContextMenu->setSubWidget(mySizer);
						
						bool menuHaveImage = false;
						for (int64_t jjj=m_listElement.size()-1; jjj >= 0; jjj--) {
							if (m_listElement[iii]!=NULL) {
								if (m_listElement[iii]->m_localId == m_listElement[jjj]->m_parentId) {
									if (m_listElement[jjj]->m_image.size()!=0) {
										menuHaveImage = true;
										break;
									}
								}
							}
						}
						for (int64_t jjj=m_listElement.size()-1; jjj >= 0; jjj--) {
							if (m_listElement[iii]!=NULL) {
								if (m_listElement[iii]->m_localId == m_listElement[jjj]->m_parentId) {
									myButton = new widget::Button();
									if (NULL == myButton) {
										EWOL_ERROR("Allocation Error");
									} else {
										if (m_listElement[jjj]->m_image.size()!=0) {
											myButton->setSubWidget(
											    new widget::Composer(widget::Composer::String,
											        std::string("<composer expand=\"true,false\" fill=\"true,true\">\n") + 
											        "	<sizer mode=\"hori\" expand=\"true,false\" fill=\"true,true\" lock=\"true\">\n"
											        "		<image src=\"" + m_listElement[jjj]->m_image + "\" size=\"8,8mm\"/>\n"
											        "		<label exand=\"true,true\" fill=\"true,true\"><![CDATA[" + m_listElement[jjj]->m_label + " ]]></label>\n"
											        "	</sizer>\n"
											        "</composer>\n"));
										} else {
											if (true == menuHaveImage) {
												myButton->setSubWidget(
												    new widget::Composer(widget::Composer::String,
												        std::string("<composer expand=\"true,false\" fill=\"true,true\">\n") + 
												        "	<sizer mode=\"hori\" expand=\"true,false\" fill=\"true,true\" lock=\"true\">\n"
												        "		<spacer min-size=\"8,0mm\"/>\n"
												        "		<label exand=\"true,true\" fill=\"true,true\"><![CDATA[" + m_listElement[jjj]->m_label + "]]></label>\n"
												        "	</sizer>\n"
												        "</composer>\n"));
											} else {
												widget::Label* tmpLabel = new widget::Label(std::string("<left>") + m_listElement[jjj]->m_label + "</left>\n");
												if (NULL != tmpLabel) {
													tmpLabel->setExpand(bvec2(true,false));
													tmpLabel->setFill(bvec2(true,true));
													myButton->setSubWidget(tmpLabel);
												}
											}
										}
										// set the image if one is present ...
										myButton->registerOnEvent(this, widget::Button::eventPressed, widget::Button::eventPressed);
										myButton->setExpand(bvec2(true,false));
										myButton->setFill(bvec2(true,false));
										// add it in the widget list
										mySizer->subWidgetAdd(myButton);
										m_listElement[jjj]->m_widgetPointer = myButton;
									}
								}
							}
						}
					}
					ewol::Windows* currentWindows = getWindows();
					if (NULL == currentWindows) {
						EWOL_ERROR("Can not get the curent Windows...");
						delete(m_widgetContextMenu);
						m_widgetContextMenu=NULL;
					} else {
						currentWindows->popUpWidgetPush(m_widgetContextMenu);
					}
				}
				return;
			}
		}
	}
}


void ewol::widget::Menu::onObjectRemove(ewol::EObject * _removeObject) {
	widget::Sizer::onObjectRemove(_removeObject);
	if (m_widgetContextMenu == _removeObject) {
		m_widgetContextMenu = NULL;
	}
	for (size_t jjj=0; jjj<m_listElement.size(); jjj++) {
		if (NULL != m_listElement[jjj]) {
			if (m_listElement[jjj]->m_widgetPointer == _removeObject) {
				m_listElement[jjj]->m_widgetPointer = NULL;
			}
		}
	}
}

