/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ewol/ewol.hpp>
#include <ewol/widget/Manager.hpp>
#include <ewol/widget/Menu.hpp>
#include <ewol/widget/Button.hpp>
#include <ewol/widget/ContextMenu.hpp>
#include <ewol/widget/Composer.hpp>
#include <ewol/widget/Label.hpp>
#include <ewol/widget/Windows.hpp>
#include <ewol/widget/Spacer.hpp>

ewol::widget::Menu::Menu() :
  signalSelect(this, "select", "") {
	addObjectType("ewol::widget::Menu");
	m_staticId = 666;
	propertyLockExpand.setDirect(bvec2(true,true));
}

ewol::widget::Menu::~Menu() {
	clear();
}

void ewol::widget::Menu::subWidgetRemoveAll() {
	clear();
	ewol::widget::Sizer::subWidgetRemoveAll();
}

int32_t ewol::widget::Menu::subWidgetAdd(ewol::WidgetShared _newWidget) {
	EWOL_ERROR("Not availlable");
	return -1;
}

void ewol::widget::Menu::subWidgetRemove(ewol::WidgetShared _newWidget) {
	EWOL_ERROR("Not availlable");
}

void ewol::widget::Menu::subWidgetUnLink(ewol::WidgetShared _newWidget) {
	EWOL_ERROR("Not availlable");
}

void ewol::widget::Menu::clear() {
	m_listElement.clear();
}

int32_t ewol::widget::Menu::addTitle(const std::string& _label,
                                     const std::string& _image,
                                     const std::string& _message) {
	return add(-1, _label, _image, _message);
}

static const char* eventButtonPressed = "menu-local-pressed";

int32_t ewol::widget::Menu::get(const std::string& _label) {
	for (auto &it : m_listElement) {
		if (it.m_label == _label) {
			return it.m_localId;
		}
	}
	return -1;
}

int32_t ewol::widget::Menu::add(int32_t _parent,
                                const std::string& _label,
                                const std::string& _image,
                                const std::string& _message) {
	// try to find one already created:
	int32_t previous = get(_label);
	if (previous != -1) {
		return previous;
	}
	ewol::widget::MenuElement tmpObject;
	tmpObject.m_localId = m_staticId++;
	tmpObject.m_parentId = _parent;
	tmpObject.m_label = _label;
	tmpObject.m_image = _image;
	tmpObject.m_message = _message;
	if (tmpObject.m_parentId == -1) {
		ewol::widget::ButtonShared myButton = ewol::widget::Button::create();
		if (myButton == nullptr) {
			EWOL_ERROR("Allocation button error");
			return tmpObject.m_localId;
		}
		if (tmpObject.m_image.size()!=0) {
			std::string composeString ="<sizer mode='hori' expand='true,false' fill='true,true'>\n";
			if (etk::end_with(tmpObject.m_image, ".edf") == true) {
				composeString+="    <image src='" + tmpObject.m_image + "' size='8,8mm' distance-field='true'/>\n";
			} else {
				composeString+="    <image src='" + tmpObject.m_image + "' size='8,8mm'/>\n";
			}
			composeString+="    <label><left>" + tmpObject.m_label + "</left></label>\n";
			composeString+="</sizer>\n";
			myButton->setSubWidget(ewol::widget::composerGenerateString(composeString));
		} else {
			ewol::widget::LabelShared label = ewol::widget::Label::create();
			label->propertyValue.set("<left>" + tmpObject.m_label + "</left>");
			myButton->setSubWidget(label);
		}
		// add it in the widget list
		ewol::widget::Sizer::subWidgetAdd(myButton);
		// keep the specific event ...
		myButton->signalPressed.connect(sharedFromThis(), &ewol::widget::Menu::onButtonPressed, ewol::widget::ButtonWeak(myButton));
		tmpObject.m_widgetPointer = myButton;
	}
	m_listElement.push_back(tmpObject);
	return tmpObject.m_localId;
}

void ewol::widget::Menu::remove(int32_t _id) {
	EWOL_TODO("NOT remove...");
}


int32_t ewol::widget::Menu::addSpacer(int32_t _parent) {
	ewol::widget::MenuElement tmpObject;
	tmpObject.m_localId = m_staticId++;
	tmpObject.m_parentId = _parent;
	tmpObject.m_label = "";
	tmpObject.m_image = "";
	tmpObject.m_message = "";
	if (tmpObject.m_parentId == -1) {
		ewol::widget::SpacerShared mySpacer = ewol::widget::Spacer::create();
		if (mySpacer == nullptr) {
			EWOL_ERROR("Allocation spacer error");
			return tmpObject.m_localId;
		}
		mySpacer->propertyExpand.set(bvec2(true,true));
		mySpacer->propertyFill.set(bvec2(true,true));
		mySpacer->propertyMinSize.set(gale::Dimension(vec2(2,0), gale::distance::pixel));
		mySpacer->propertyMaxSize.set(gale::Dimension(vec2(2,10000), gale::distance::pixel));
		mySpacer->propertyColor.set(etk::Color<>(0,0,0,0xFF));
		// add it in the widget list
		ewol::widget::Sizer::subWidgetAdd(mySpacer);
	}
	m_listElement.push_back(tmpObject);
	return tmpObject.m_localId;
}

void ewol::widget::Menu::onButtonPressed(ewol::widget::ButtonWeak _button) {
	ewol::widget::ButtonShared caller = _button.lock();
	if (caller == nullptr) {
		return;
	}
	for (auto &it : m_listElement) {
		if (caller != it.m_widgetPointer.lock()) {
			continue;
		}
		// 2 posible case (have a message or have a child ...
		if (it.m_message.size() > 0) {
			EWOL_DEBUG("Menu  == > generate Event");
			// Send a multicast event ...
			signalSelect.emit(it.m_message);
			ewol::widget::ContextMenuShared tmpContext = m_widgetContextMenu.lock();
			if (tmpContext != nullptr) {
				EWOL_DEBUG("Mark the menu to remove ...");
				tmpContext->destroy();
			}
			return;
		}
		EWOL_DEBUG("Menu  == > load Sub Menu");
		bool findChild = false;
		for (auto &it2 : m_listElement) {
			if (it.m_localId == it2.m_parentId) {
				findChild = true;
				break;
			}
		}
		if (false == findChild) {
			EWOL_WARNING("Event on menu element with no child an no event... label=" << it.m_label);
			return;
		}
		// create a context menu:
		ewol::widget::ContextMenuShared tmpContext = ewol::widget::ContextMenu::create();
		m_widgetContextMenu = tmpContext;
		if (tmpContext == nullptr) {
			EWOL_ERROR("Allocation Error");
			return;
		}
		// get the button widget:
		vec2 newPosition;
		ewol::WidgetShared eventFromWidget = ememory::dynamicPointerCast<ewol::Widget>(caller);
		if (eventFromWidget != nullptr) {
			vec2 tmpOri  = eventFromWidget->getOrigin();
			vec2 tmpSize = eventFromWidget->getSize();
			// calculate the correct position
			newPosition.setValue(tmpOri.x() + tmpSize.x()/2,
			                     tmpOri.y() );
		}
		tmpContext->setPositionMark(ewol::widget::ContextMenu::markTop, newPosition);
		ewol::widget::SizerShared mySizer;
		ewol::widget::ButtonShared myButton;
		mySizer = ewol::widget::Sizer::create();
		if (mySizer != nullptr) {
			mySizer->propertyMode.set(widget::Sizer::modeVert);
			mySizer->propertyLockExpand.set(vec2(true,true));
			mySizer->propertyFill.set(vec2(true,true));
			// set it in the pop-up-system:
			tmpContext->setSubWidget(mySizer);
			bool menuHaveImage = false;
			for (auto &it2 : m_listElement) {
				if (it.m_localId != it2.m_parentId) {
					continue;
				}
				if (it2.m_image.size()!=0) {
					menuHaveImage = true;
					break;
				}
			}
			for (auto it2=m_listElement.rbegin(); it2!=m_listElement.rend() ; ++it2) {
				if (it.m_localId != it2->m_parentId) {
					continue;
				}
				if (it2->m_message == "" && it2->m_label == "") {
					ewol::widget::SpacerShared mySpacer = ewol::widget::Spacer::create();
					if (mySpacer == nullptr) {
						EWOL_ERROR("Allocation spacer error");
						continue;
					}
					mySpacer->propertyExpand.set(bvec2(true,true));
					mySpacer->propertyFill.set(bvec2(true,true));
					mySpacer->propertyMinSize.set(gale::Dimension(vec2(0,2), gale::distance::pixel));
					mySpacer->propertyMaxSize.set(gale::Dimension(vec2(10000,2), gale::distance::pixel));
					mySpacer->propertyColor.set(etk::Color<>(0,0,0,0xFF));
					// add it in the widget list
					mySizer->subWidgetAdd(mySpacer);
				} else {
					myButton = ewol::widget::Button::create();
					if (myButton == nullptr) {
						EWOL_ERROR("Allocation Error");
						continue;
					}
					myButton->propertyExpand.set(bvec2(true,true));
					myButton->propertyFill.set(bvec2(true,true));
					// set callback
					myButton->signalPressed.connect(sharedFromThis(), &ewol::widget::Menu::onButtonPressed, ewol::widget::ButtonWeak(myButton));
					// add it in the widget list
					mySizer->subWidgetAdd(myButton);
					if (it2->m_image.size() != 0) {
						std::string composeString;
						composeString+= "    <sizer mode='hori' expand='true,false' fill='true,true' lock='true'>\n";
						if (etk::end_with(it2->m_image, ".edf") == true) {
							composeString+="        <image src='" + it2->m_image + "' size='8,8mm' distance-field='true'/>\n";
						} else {
							composeString+="        <image src='" + it2->m_image + "' size='8,8mm'/>\n";
						}
						composeString+="        <label exand='true,true' fill='true,true'><left>" + it2->m_label + "</left></label>\n";
						composeString+="    </sizer>\n";
						myButton->setSubWidget(ewol::widget::composerGenerateString(composeString));
					} else {
						if (menuHaveImage == true) {
							myButton->setSubWidget(ewol::widget::composerGenerateString(
							        std::string() +
							        "	<sizer mode='hori' expand='true,false' fill='true,true' lock='true'>\n"
							        "		<spacer min-size='8,0mm'/>\n"
							        "		<label exand='true,true' fill='true,true'><![CDATA[<left>" + it2->m_label + "</left>]]></label>\n"
							        "	</sizer>\n")
							    );
						} else {
							ewol::widget::LabelShared tmpLabel = widget::Label::create();
							if (tmpLabel != nullptr) {
								tmpLabel->propertyValue.set(std::string("<left>") + it2->m_label + "</left>\n");
								tmpLabel->propertyExpand.set(bvec2(true,false));
								tmpLabel->propertyFill.set(bvec2(true,true));
								myButton->setSubWidget(tmpLabel);
							}
						}
					}
					it2->m_widgetPointer = myButton;
				}
			}
		}
		ewol::widget::WindowsShared currentWindows = getWindows();
		if (currentWindows == nullptr) {
			EWOL_ERROR("Can not get the curent Windows...");
		} else {
			currentWindows->popUpWidgetPush(tmpContext);
		}
		return;
	}
}

bool ewol::widget::Menu::loadXML(const exml::Element& _node) {
	if (_node.exist() == false) {
		return false;
	}
	// parse generic properties:
	ewol::Widget::loadXML(_node);
	// parse all the elements :
	for (const auto nodeIt : _node.nodes) {
		const exml::Element pNode = nodeIt.toElement();
		if (pNode.exist() == false) {
			// trash here all that is not element
			continue;
		}
		std::string widgetName = pNode.getValue();
		EWOL_INFO("Get node : " << pNode);
		if (widgetName == "elem") {
			// <elem title="_T{Title of the button}" image="DATA:List.svg" event="menu:exit">
			int32_t idMenu = addTitle(pNode.attributes["title"], pNode.attributes["image"], pNode.attributes["event"]);
			for (const auto nodeIt2 : pNode.nodes) {
				
				const exml::Element pNode2 = nodeIt2.toElement();
				if (pNode2.exist() == false) {
					// trash here all that is not element
					continue;
				}
				std::string widgetName2 = pNode2.getValue();
				if (widgetName2 == "elem") {
					// <elem title="_T{Title of the button}" image="DATA:List.svg" event="menu:exit">
					add(idMenu, pNode2.attributes["title"], pNode2.attributes["image"], pNode2.attributes["event"]);
				} else if (widgetName2 == "separator") {
					addSpacer(idMenu);
				} else {
					EWOL_ERROR("[" << getId() << "] {" << getObjectType() << "} (l " << pNode2.getPos() << ") Unknown basic node='" << widgetName2 << "' not in : [elem,separator]" );
				}
			}
		} else if (widgetName == "separator") {
			addSpacer();
		} else {
			EWOL_ERROR("[" << getId() << "] {" << getObjectType() << "} (l " << pNode.getPos() << ") Unknown basic node='" << widgetName << "' not in : [elem,separator]" );
		}
	}
	return true;
}
