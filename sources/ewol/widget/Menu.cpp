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

ewol::widget::Menu::Menu() :
  signalSelect(*this, "select") {
	addObjectType("ewol::widget::Menu");
	m_staticId = 666;
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
	// try to find one already created :
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
	if (-1 == tmpObject.m_parentId) {
		std::shared_ptr<ewol::widget::Button> myButton = ewol::widget::Button::create();
		if (myButton == nullptr) {
			EWOL_ERROR("Allocation button error");
			return tmpObject.m_localId;
		}
		if (tmpObject.m_image.size()!=0) {
			std::string composeString ="<sizer mode=\"hori\">\n";
			if (etk::end_with(tmpObject.m_image, ".edf") == true) {
				composeString+="    <image src=\"" + tmpObject.m_image + "\" size=\"8,8mm\" distance-field='true'/>\n";
			} else {
				composeString+="    <image src=\"" + tmpObject.m_image + "\" size=\"8,8mm\"/>\n";
			}
			composeString+="    <label><left>" + tmpObject.m_label + "</left></label>\n";
			composeString+="</sizer>\n";
			myButton->setSubWidget(ewol::widget::Composer::create(widget::Composer::String, composeString));
		} else {
			myButton->setSubWidget(ewol::widget::Label::create("<left>" + tmpObject.m_label + "</left>") );
		}
		// add it in the widget list
		ewol::widget::Sizer::subWidgetAdd(myButton);
		// keep the specific event ...
		myButton->signalPressed.connect(shared_from_this(), std::bind(&ewol::widget::Menu::onButtonPressed, this, std::weak_ptr<ewol::widget::Button>(myButton)));
		tmpObject.m_widgetPointer = myButton;
	}
	m_listElement.push_back(tmpObject);
	return tmpObject.m_localId;
}

void ewol::widget::Menu::remove(int32_t _id) {
	EWOL_TODO("NOT remove...");
}


int32_t ewol::widget::Menu::addSpacer() {
	EWOL_TODO("NOT addSpacer...");
	return -1;
}

void ewol::widget::Menu::onButtonPressed(std::weak_ptr<ewol::widget::Button> _button) {
	std::shared_ptr<ewol::widget::Button> caller = _button.lock();
	if (caller == nullptr) {
		return;
	}
	for (auto &it : m_listElement) {
		if (caller == it.m_widgetPointer.lock()) {
			// 2 posible case (have a message or have a child ...
			if (it.m_message.size() > 0) {
				EWOL_DEBUG("Menu  == > generate Event");
				// Send a multicast event ...
				signalSelect.emit(it.m_message);
				std::shared_ptr<ewol::widget::ContextMenu> tmpContext = m_widgetContextMenu.lock();
				if (tmpContext != nullptr) {
					EWOL_DEBUG("Mark the menu to remove ...");
					tmpContext->destroy();
				}
				return;
			} else{
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
				// create a context menu : 
				std::shared_ptr<ewol::widget::ContextMenu> tmpContext = ewol::widget::ContextMenu::create();
				m_widgetContextMenu = tmpContext;
				if (tmpContext == nullptr) {
					EWOL_ERROR("Allocation Error");
					return;
				}
				// get the button widget : 
				vec2 newPosition;
				std::shared_ptr<ewol::Widget> eventFromWidget = std::dynamic_pointer_cast<ewol::Widget>(caller);
				if (eventFromWidget != nullptr) {
					vec2 tmpOri  = eventFromWidget->getOrigin();
					vec2 tmpSize = eventFromWidget->getSize();
					// calculate the correct position
					newPosition.setValue(tmpOri.x() + tmpSize.x()/2,
					                     tmpOri.y() );
				}
				tmpContext->setPositionMark(ewol::widget::ContextMenu::markTop, newPosition );
				
				std::shared_ptr<ewol::widget::Sizer> mySizer;
				std::shared_ptr<ewol::widget::Button> myButton;
				
				mySizer = ewol::widget::Sizer::create(widget::Sizer::modeVert);
				if (nullptr != mySizer) {
					mySizer->lockExpand(vec2(true,true));
					// set it in the pop-up-system : 
					tmpContext->setSubWidget(mySizer);
					
					bool menuHaveImage = false;
					for (auto &it2 : m_listElement) {
						if (it.m_localId == it2.m_parentId) {
							if (it2.m_image.size()!=0) {
								menuHaveImage = true;
								break;
							}
						}
					}
					for (auto it2=m_listElement.rbegin(); it2!=m_listElement.rend() ; ++it2) {
						if (it.m_localId == it2->m_parentId) {
							myButton = ewol::widget::Button::create();
							if (myButton == nullptr) {
								EWOL_ERROR("Allocation Error");
							} else {
								if (it2->m_image.size()!=0) {
									std::string composeString = "<composer expand=\"true,false\" fill=\"true,true\">\n";
									composeString+= "    <sizer mode=\"hori\" expand=\"true,false\" fill=\"true,true\" lock=\"true\">\n";
									if (etk::end_with(it2->m_image, ".edf") == true) {
										composeString+="        <image src=\"" + it2->m_image + "\" size=\"8,8mm\"  distance-field='true'/>\n";
									} else {
										composeString+="        <image src=\"" + it2->m_image + "\" size=\"8,8mm\"/>\n";
									}
									composeString+="        <label exand=\"true,true\" fill=\"true,true\"><left>" + it2->m_label + "</left></label>\n";
									composeString+="    </sizer>\n";
									composeString+="</composer>\n";
									myButton->setSubWidget(ewol::widget::Composer::create(widget::Composer::String, composeString));
								} else {
									if (true == menuHaveImage) {
										myButton->setSubWidget(ewol::widget::Composer::create(widget::Composer::String,
										        std::string("<composer expand=\"true,false\" fill=\"true,true\">\n") + 
										        "	<sizer mode=\"hori\" expand=\"true,false\" fill=\"true,true\" lock=\"true\">\n"
										        "		<spacer min-size=\"8,0mm\"/>\n"
										        "		<label exand=\"true,true\" fill=\"true,true\"><![CDATA[<left>" + it2->m_label + "</left>]]></label>\n"
										        "	</sizer>\n"
										        "</composer>\n"));
									} else {
										std::shared_ptr<ewol::widget::Label> tmpLabel = widget::Label::create(std::string("<left>") + it2->m_label + "</left>\n");
										if (tmpLabel != nullptr) {
											tmpLabel->setExpand(bvec2(true,false));
											tmpLabel->setFill(bvec2(true,true));
											myButton->setSubWidget(tmpLabel);
										}
									}
								}
								// set the image if one is present ...
								//myButton->registerOnEvent(shared_from_this(), "pressed", eventButtonPressed);
								myButton->signalPressed.connect(shared_from_this(), std::bind(&ewol::widget::Menu::onButtonPressed, this, std::weak_ptr<ewol::widget::Button>(myButton)));
								myButton->setExpand(bvec2(true,false));
								myButton->setFill(bvec2(true,false));
								// add it in the widget list
								mySizer->subWidgetAdd(myButton);
								it2->m_widgetPointer = myButton;
							}
						}
					}
				}
				std::shared_ptr<ewol::widget::Windows> currentWindows = getWindows();
				if (currentWindows == nullptr) {
					EWOL_ERROR("Can not get the curent Windows...");
				} else {
					currentWindows->popUpWidgetPush(tmpContext);
				}
			}
			return;
		}
	}
}


