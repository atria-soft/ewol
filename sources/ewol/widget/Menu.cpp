/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#include <ewol/ewol.h>
#include <ewol/widget/Manager.h>
#include <ewol/widget/Menu.h>
#include <ewol/widget/Button.h>
#include <ewol/widget/ContextMenu.h>
#include <ewol/widget/Composer.h>
#include <ewol/widget/Label.h>
#include <ewol/widget/Windows.h>

#undef __class__
#define __class__ "Menu"

ewol::widget::Menu::Menu() {
	addObjectType("ewol::widget::Menu");
	m_staticId = 0;
}

void ewol::widget::Menu::init() {
	ewol::widget::Sizer::init();
}

ewol::widget::Menu::~Menu() {
	clear();
}

void ewol::widget::Menu::subWidgetRemoveAll() {
	clear();
	ewol::widget::Sizer::subWidgetRemoveAll();
}

int32_t ewol::widget::Menu::subWidgetAdd(std::shared_ptr<ewol::Widget> _newWidget) {
	EWOL_ERROR("Not availlable");
	return -1;
}

void ewol::widget::Menu::subWidgetRemove(std::shared_ptr<ewol::Widget> _newWidget) {
	EWOL_ERROR("Not availlable");
}

void ewol::widget::Menu::subWidgetUnLink(std::shared_ptr<ewol::Widget> _newWidget) {
	EWOL_ERROR("Not availlable");
}

void ewol::widget::Menu::clear() {
	for (size_t iii=0; iii < m_listElement.size(); iii++) {
		if (m_listElement[iii] != nullptr) {
			delete(m_listElement[iii]);
			m_listElement[iii] = nullptr;
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
	ewol::widget::MenuElement* tmpObject = new ewol::widget::MenuElement();
	if (tmpObject == nullptr) {
		EWOL_ERROR("Allocation problem");
		return -1;
	}
	tmpObject->m_localId = m_staticId++;
	tmpObject->m_parentId = _parent;
	tmpObject->m_widgetPointer = nullptr;
	tmpObject->m_label = std::string("<left>") + _label + "</left>";
	tmpObject->m_image = _image;
	tmpObject->m_generateEvent = _generateEvent;
	tmpObject->m_message = _message;
	m_listElement.push_back(tmpObject);
	if (-1 == tmpObject->m_parentId) {
		std::shared_ptr<ewol::widget::Button> myButton = ewol::widget::Button::create();
		if (myButton == nullptr) {
			EWOL_ERROR("Allocation button error");
			return tmpObject->m_localId;
		}
		if (tmpObject->m_image.size()!=0) {
			std::string composeString ="<sizer mode=\"hori\">\n";
			if (std::end_with(tmpObject->m_image, ".edf") == true) {
				composeString+="    <image src=\"" + tmpObject->m_image + "\" size=\"8,8mm\" distance-field='true'/>\n";
			} else {
				composeString+="    <image src=\"" + tmpObject->m_image + "\" size=\"8,8mm\"/>\n";
			}
			composeString+="    <label>" + tmpObject->m_label + "</label>\n";
			composeString+="</sizer>\n";
			myButton->setSubWidget(ewol::widget::Composer::create(widget::Composer::String, composeString));
		} else {
			myButton->setSubWidget(ewol::widget::Label::create(tmpObject->m_label) );
		}
		
		// add it in the widget list
		ewol::widget::Sizer::subWidgetAdd(myButton);
		// keep the specific event ...
		myButton->registerOnEvent(shared_from_this(), ewol::widget::Button::eventPressed, widget::Button::eventPressed);
		tmpObject->m_widgetPointer = myButton;
	}
	return tmpObject->m_localId;
}

void ewol::widget::Menu::addSpacer() {
	EWOL_TODO("NOT now...");
}


void ewol::widget::Menu::onReceiveMessage(const ewol::object::Message& _msg) {
	/*
	if (true == ewol::sizer::onReceiveMessage(_msg) {
		return true;
	}
	*/
	EWOL_ERROR(" receive message : " << _msg);
	if (_msg.getMessage() == ewol::widget::Button::eventPressed) {
		for (size_t iii=0; iii<m_listElement.size(); iii++) {
			if (_msg.getCaller() == m_listElement[iii]->m_widgetPointer) {
				// 2 posible case (have a message or have a child ...
				if (m_listElement[iii]->m_generateEvent != nullptr) {
					EWOL_DEBUG("Menu  == > generate Event");
					// Send a multicast event ...
					sendMultiCast(m_listElement[iii]->m_generateEvent, m_listElement[iii]->m_message);
					if (nullptr != m_widgetContextMenu) {
						EWOL_DEBUG("Mark the menu to remove ...");
						m_widgetContextMenu.reset();
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
					m_widgetContextMenu = ewol::widget::ContextMenu::create();
					if (nullptr == m_widgetContextMenu) {
						EWOL_ERROR("Allocation Error");
						return;
					}
					// get the button widget : 
					vec2 newPosition;
					std::shared_ptr<ewol::Widget> eventFromWidget = std::dynamic_pointer_cast<ewol::Widget>(_msg.getCaller());
					if (eventFromWidget != nullptr) {
						vec2 tmpOri  = eventFromWidget->getOrigin();
						vec2 tmpSize = eventFromWidget->getSize();
						// calculate the correct position
						newPosition.setValue(tmpOri.x() + tmpSize.x()/2,
						                     tmpOri.y() );
					}
					m_widgetContextMenu->setPositionMark(ewol::widget::ContextMenu::markTop, newPosition );
					
					std::shared_ptr<ewol::widget::Sizer> mySizer;
					std::shared_ptr<ewol::widget::Button> myButton;
					
					mySizer = ewol::widget::Sizer::create(widget::Sizer::modeVert);
					if (nullptr != mySizer) {
						mySizer->lockExpand(vec2(true,true));
						// set it in the pop-up-system : 
						m_widgetContextMenu->setSubWidget(mySizer);
						
						bool menuHaveImage = false;
						for (int64_t jjj=m_listElement.size()-1; jjj >= 0; jjj--) {
							if (m_listElement[iii]!=nullptr) {
								if (m_listElement[iii]->m_localId == m_listElement[jjj]->m_parentId) {
									if (m_listElement[jjj]->m_image.size()!=0) {
										menuHaveImage = true;
										break;
									}
								}
							}
						}
						for (int64_t jjj=m_listElement.size()-1; jjj >= 0; jjj--) {
							if (m_listElement[iii]!=nullptr) {
								if (m_listElement[iii]->m_localId == m_listElement[jjj]->m_parentId) {
									myButton = ewol::widget::Button::create();
									if (nullptr == myButton) {
										EWOL_ERROR("Allocation Error");
									} else {
										if (m_listElement[jjj]->m_image.size()!=0) {
										
											std::string composeString = "<composer expand=\"true,false\" fill=\"true,true\">\n";
											composeString+= "    <sizer mode=\"hori\" expand=\"true,false\" fill=\"true,true\" lock=\"true\">\n";
											if (std::end_with(m_listElement[jjj]->m_image, ".edf") == true) {
												composeString+="        <image src=\"" + m_listElement[jjj]->m_image + "\" size=\"8,8mm\"  distance-field='true'/>\n";
											} else {
												composeString+="        <image src=\"" + m_listElement[jjj]->m_image + "\" size=\"8,8mm\"/>\n";
											}
											composeString+="        <label exand=\"true,true\" fill=\"true,true\">" + m_listElement[jjj]->m_label + "</label>\n";
											composeString+="    </sizer>\n";
											composeString+="</composer>\n";
											myButton->setSubWidget(ewol::widget::Composer::create(widget::Composer::String, composeString));
										} else {
											if (true == menuHaveImage) {
												myButton->setSubWidget(ewol::widget::Composer::create(widget::Composer::String,
												        std::string("<composer expand=\"true,false\" fill=\"true,true\">\n") + 
												        "	<sizer mode=\"hori\" expand=\"true,false\" fill=\"true,true\" lock=\"true\">\n"
												        "		<spacer min-size=\"8,0mm\"/>\n"
												        "		<label exand=\"true,true\" fill=\"true,true\"><![CDATA[" + m_listElement[jjj]->m_label + "]]></label>\n"
												        "	</sizer>\n"
												        "</composer>\n"));
											} else {
												std::shared_ptr<ewol::widget::Label> tmpLabel = widget::Label::create(std::string("<left>") + m_listElement[jjj]->m_label + "</left>\n");
												if (tmpLabel != nullptr) {
													tmpLabel->setExpand(bvec2(true,false));
													tmpLabel->setFill(bvec2(true,true));
													myButton->setSubWidget(tmpLabel);
												}
											}
										}
										// set the image if one is present ...
										myButton->registerOnEvent(shared_from_this(), ewol::widget::Button::eventPressed, widget::Button::eventPressed);
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
					std::shared_ptr<ewol::widget::Windows> currentWindows = getWindows();
					if (currentWindows == nullptr) {
						EWOL_ERROR("Can not get the curent Windows...");
						m_widgetContextMenu.reset();
					} else {
						currentWindows->popUpWidgetPush(m_widgetContextMenu);
					}
				}
				return;
			}
		}
	}
}


void ewol::widget::Menu::onObjectRemove(const std::shared_ptr<ewol::Object>& _removeObject) {
	ewol::widget::Sizer::onObjectRemove(_removeObject);
	if (m_widgetContextMenu == _removeObject) {
		m_widgetContextMenu.reset();
	}
	for (size_t jjj=0; jjj<m_listElement.size(); jjj++) {
		if (m_listElement[jjj] != nullptr) {
			if (m_listElement[jjj]->m_widgetPointer == _removeObject) {
				m_listElement[jjj]->m_widgetPointer.reset();
			}
		}
	}
}

