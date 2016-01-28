/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */


#include <appl/debug.h>
#include <appl/MainWindows.h>

#include <ewol/widget/Button.h>
#include <ewol/widget/CheckBox.h>
#include <ewol/widget/Sizer.h>
#include <ewol/widget/Label.h>
#include <ewol/widget/Entry.h>
#include <ewol/widget/List.h>
#include <ewol/widget/ContextMenu.h>
#include <ewol/widget/PopUp.h>
#include <ewol/widget/Slider.h>
#include <ewol/widget/Menu.h>
#include <ewol/widget/meta/FileChooser.h>
#include <ewol/widget/meta/Parameter.h>
#include <ewol/widget/Manager.h>
#include <ewol/context/Context.h>
#include <appl/TestButton.h>
#include <appl/TestCheckBox.h>
#include <appl/TestButtonColor.h>
#include <appl/TestLabel.h>
#include <appl/TestImage.h>
#include <appl/TestDistanceField.h>
#include <etk/os/FSNode.h>


static const char * l_eventChangeTheme           = "event-change-theme";
static const char * l_eventChangeWidgetNext      = "event-change-widget-test-next";
static const char * l_eventChangeWidgetPrevious  = "event-change-widget-test-previous";


#undef __class__
#define __class__ "MainWindows"

appl::MainWindows::MainWindows() :
  m_idWidget(0) {
	APPL_DEBUG("CREATE WINDOWS ... ");
	addObjectType("appl::MainWindows");
}

void appl::MainWindows::init() {
	ewol::widget::Windows::init();
	std::shared_ptr<ewol::widget::Sizer> mySizerHori;
	std::shared_ptr<ewol::widget::Sizer> mySizerVert;
	std::shared_ptr<ewol::widget::Button> myButton;
	
	mySizerVert = ewol::widget::Sizer::create(ewol::widget::Sizer::modeVert);
	if (mySizerVert == nullptr) {
		APPL_ERROR("Allocation error mySizerVert");
		return;
	}
	setSubWidget(mySizerVert);
		
		mySizerHori = ewol::widget::Sizer::create(ewol::widget::Sizer::modeHori);
		if (mySizerHori == nullptr) {
			APPL_ERROR("Allocation error mySizerHori");
			return;
		}
		mySizerVert->subWidgetAdd(mySizerHori);
			myButton = ewol::widget::Button::create();
			if (myButton != nullptr) {
				myButton->setSubWidget(ewol::widget::Label::create("default theme (cube)"));
				myButton->setSubWidgetToggle(ewol::widget::Label::create("rounded theme"));
				myButton->setToggleMode(true);
				myButton->signalValue.bind(shared_from_this(), &appl::MainWindows::onCallbackThemeChange);
				mySizerHori->subWidgetAdd(myButton);
			}
			myButton = ewol::widget::Button::create();
			if (myButton != nullptr) {
				myButton->setSubWidget(ewol::widget::Label::create("Previous Widget test"));
				myButton->signalPressed.bind(shared_from_this(), &appl::MainWindows::onCallbackWidgetChange, -1);
				mySizerHori->subWidgetAdd(myButton);
			}
			myButton = ewol::widget::Button::create();
			if (myButton != nullptr) {
				myButton->setSubWidget(ewol::widget::Label::create("Next Widget test"));
				myButton->signalPressed.bind(shared_from_this(), &appl::MainWindows::onCallbackWidgetChange, 1);
				mySizerHori->subWidgetAdd(myButton);
			}
			m_testName = ewol::widget::Label::create("none");
			if (m_testName != nullptr) {
				mySizerHori->subWidgetAdd(m_testName);
			}
		std::shared_ptr<ewol::widget::Spacer> mySpacer = ewol::widget::Spacer::create();
		if (mySpacer != nullptr) {
			mySpacer->setExpand(bvec2(false,false));
			mySpacer->setFill(bvec2(true,false));
			mySpacer->setMinSize(vec2(3,3));
			mySpacer->setColor(0x000000FF);
			mySizerVert->subWidgetAdd(mySpacer);
		}
		mySizerHori = ewol::widget::Sizer::create(ewol::widget::Sizer::modeHori);
		if (mySizerHori == nullptr) {
			APPL_ERROR("Allocation error mySizerVert");
			return;
		}
		mySizerHori->setExpand(bvec2(true,true));
		mySizerHori->setFill(bvec2(true,true));
		mySizerHori->lockExpand(bvec2(true,true));
		mySizerVert->subWidgetAdd(mySizerHori);
			m_sizerDynamic = ewol::widget::Sizer::create(ewol::widget::Sizer::modeVert);
			if (m_sizerDynamic == nullptr) {
				APPL_ERROR("Allocation error mySizerHori");
				return;
			}
			m_sizerDynamic->setExpand(bvec2(false,true));
			m_sizerDynamic->setFill(bvec2(true,true));
			m_sizerDynamic->setMinSize(gale::Dimension::Dimension(vec2(12,0), gale::Dimension::Pourcent));
			m_sizerDynamic->lockExpand(bvec2(true,true));
			mySizerHori->subWidgetAdd(m_sizerDynamic);
			mySpacer = ewol::widget::Spacer::create();
			if (mySpacer == nullptr) {
				APPL_ERROR("Allocation error mySizerHori");
				return;
			}
			mySpacer->setExpand(bvec2(false,true));
			mySpacer->setFill(bvec2(false,true));
			mySpacer->setMinSize(vec2(3,3));
			mySpacer->setColor(0x000000FF);
			mySizerHori->subWidgetAdd(mySpacer);
			
			m_sizerVert = ewol::widget::Sizer::create(ewol::widget::Sizer::modeVert);
			if (m_sizerVert == nullptr) {
				APPL_ERROR("Allocation error mySizerVert");
				return;
			}
			mySizerHori->subWidgetAdd(m_sizerVert);
}

void appl::MainWindows::onCallbackThemeChange(const bool& _value) {
	if (_value == true) {
		etk::theme::setName("GUI", "shape/round/");
	} else {
		etk::theme::setName("GUI", "shape/square/");
	}
	// Reload shaders and graphic system ...
	ewol::getContext().getResourcesManager().reLoadResources();
	ewol::getContext().forceRedrawAll();
}

void appl::MainWindows::onCallbackWidgetChange(int32_t _increment) {
	m_idWidget += _increment;
	
	if (m_subWidget != nullptr) {
		m_subWidget->destroy();
		// in theory it must be removed ...
		m_subWidget.reset();
	}
	switch(m_idWidget) {
		case 0:
			m_subWidget = appl::TestButton::create();
			if (m_subWidget != nullptr) {
				m_sizerVert->subWidgetAdd(m_subWidget);
			}
			if (m_testName!=nullptr) {
				m_testName->setLabel("TestButton");
			};
			break;
		case 1:
			m_subWidget = TestDistanceField::create();
			if (m_subWidget != nullptr) {
				m_sizerVert->subWidgetAdd(m_subWidget);
			}
			if (m_testName != nullptr) {
				m_testName->setLabel("Test Distance Field");
			};
			break;
		case 2:
			m_subWidget = TestButtonColor::create();
			if (m_subWidget != nullptr) {
				m_sizerVert->subWidgetAdd(m_subWidget);
			}
			if (m_testName != nullptr) {
				m_testName->setLabel("TestButtonColor");
			};
			break;
		case 3:
			m_subWidget = TestLabel::create();
			if (m_subWidget != nullptr) {
				m_sizerVert->subWidgetAdd(m_subWidget);
			}
			if (m_testName != nullptr) {
				m_testName->setLabel("TestLabel");
			};
			break;
		case 4:
			m_subWidget = TestImage::create();
			if (m_subWidget != nullptr) {
				m_sizerVert->subWidgetAdd(m_subWidget);
			}
			if (m_testName != nullptr) {
				m_testName->setLabel("TestImage");
			};
			break;
		case 5:
			m_subWidget = appl::TestCheckBox::create();
			if (m_subWidget != nullptr) {
				m_sizerVert->subWidgetAdd(m_subWidget);
			}
			if (m_testName != nullptr) {
				m_testName->setLabel("TestCheckbox");
			};
			break;
		default:
			m_subWidget = ewol::widget::Label::create("Test software for EWOL");
			if (m_subWidget != nullptr) {
				m_subWidget->setExpand(bvec2(true,true));
				m_sizerVert->subWidgetAdd(m_subWidget);
			}
			if (m_testName != nullptr) {
				m_testName->setLabel("Label");
			};
			m_idWidget = -1;
			break;
	}
	updateProperty();
}

void appl::MainWindows::updateProperty() {
	// remove all elements:
	m_sizerDynamic->subWidgetRemoveAll();
	if (m_subWidget == nullptr) {
		return;
	}
	std::shared_ptr<ewol::widget::Label> widget = ewol::widget::Label::create(m_subWidget->getObjectType());
	m_sizerDynamic->subWidgetAdd(widget);
	std::shared_ptr<ewol::widget::Spacer> mySpacer = ewol::widget::Spacer::create();
	if (mySpacer != nullptr) {
		mySpacer->setExpand(bvec2(true,false));
		mySpacer->setFill(bvec2(true,false));
		mySpacer->setMinSize(vec2(3,3));
		mySpacer->setColor(0x000000AA);
		m_sizerDynamic->subWidgetAddStart(mySpacer);
	}
	for (size_t iii=0; iii<m_subWidget->getParameterCount(); ++iii) {
		ewol::parameter::Parameter* param = m_subWidget->getParameterRaw(iii);
		if (param == nullptr) {
			APPL_WARNING("Parameter EMPTY . " << iii << " : nullptr");
			continue;
		}
		std::string type = param->getType();
		if (type == typeid(std::string).name()) {
			type = "std::string";
		} else if (type == typeid(gale::Dimension).name()) {
			type = "gale::Dimension";
		} else if (type == typeid(bvec2).name()) {
			type = "bvec2";
		} else if (type == typeid(ivec2).name()) {
			type = "ivec2";
		} else if (type == typeid(uivec2).name()) {
			type = "uivec2";
		} else if (type == typeid(vec2).name()) {
			type = "vec2";
		} else if (type == typeid(bool).name()) {
			type = "bool";
		} else if (type == typeid(int64_t).name()) {
			type = "int64_t";
		} else if (type == typeid(int32_t).name()) {
			type = "int32_t";
		} else if (type == typeid(int16_t).name()) {
			type = "int16_t";
		} else if (type == typeid(int8_t).name()) {
			type = "int8_t";
		} else if (type == typeid(uint64_t).name()) {
			type = "uint64_t";
		} else if (type == typeid(uint32_t).name()) {
			type = "uint32_t";
		} else if (type == typeid(uint16_t).name()) {
			type = "uint16_t";
		} else if (type == typeid(uint8_t).name()) {
			type = "uint8_t";
		} else if (type == typeid(float).name()) {
			type = "float";
		} else if (type == typeid(double).name()) {
			type = "double";
		} else if (type == typeid(enum ewol::gravity).name()) {
			type = "enum ewol::gravity";
		}
		// / " + param->getParameterType() + "&lt;" + type + "&gt; / " + param->getString()
		std::shared_ptr<ewol::widget::Sizer> widgetSizer = ewol::widget::Sizer::create(ewol::widget::Sizer::modeHori);
		if (widgetSizer != nullptr) {
			widgetSizer->setExpand(bvec2(true,false));
			widgetSizer->setFill(bvec2(true,true));
			m_sizerDynamic->subWidgetAddStart(widgetSizer);
			
			std::shared_ptr<ewol::widget::Label> widget = ewol::widget::Label::create(param->getName() + ":");
			widgetSizer->subWidgetAdd(widget);
			mySpacer = ewol::widget::Spacer::create();
			if (mySpacer != nullptr) {
				mySpacer->setExpand(bvec2(true,false));
				mySpacer->setFill(bvec2(true,false));
				mySpacer->setMinSize(vec2(3,3));
				widgetSizer->subWidgetAdd(mySpacer);
			}
			// Main part to do ...
			std::string type = param->getType();
			if (type == typeid(std::string).name()) {
				type = "std::string";
			} else if (type == typeid(gale::Dimension).name()) {
				type = "gale::Dimension";
			} else if (type == typeid(bvec2).name()) {
				//type = "bvec2";
				std::shared_ptr<ewol::widget::CheckBox> widgetTmp = ewol::widget::CheckBox::create();
				widgetSizer->subWidgetAdd(widgetTmp);
				widgetTmp->signalValue.connect([=](const bool& _value) {
						if (m_subWidget == nullptr) {
							APPL_ERROR("nullptr...");
							return;
						}
						APPL_INFO("set parameter : X name=" << param->getName() << " value=" << _value);
						//m_subWidget->parameterSet(param->getName(), etk::to_string(_value));
						return;
					});
				std::shared_ptr<ewol::widget::Label> widgetLabel = ewol::widget::Label::create("x");
				widgetTmp->setSubWidget(widgetLabel);
				
				widgetTmp = ewol::widget::CheckBox::create();
				widgetSizer->subWidgetAdd(widgetTmp);
				widgetTmp->signalValue.connect([=](const bool& _value) {
						if (m_subWidget == nullptr) {
							APPL_ERROR("nullptr...");
							return;
						}
						APPL_INFO("set parameter : Y name=" << param->getName() << " value=" << _value);
						//m_subWidget->parameterSet(param->getName(), etk::to_string(_value));
						return;
					});
				widgetLabel = ewol::widget::Label::create("y");
				widgetTmp->setSubWidget(widgetLabel);
			} else if (type == typeid(ivec2).name()) {
				type = "ivec2";
			} else if (type == typeid(uivec2).name()) {
				type = "uivec2";
			} else if (type == typeid(vec2).name()) {
				type = "vec2";
			} else if (type == typeid(bool).name()) {
				type = "bool";
				std::shared_ptr<ewol::widget::CheckBox> widgetTmp = ewol::widget::CheckBox::create();
				widgetSizer->subWidgetAdd(widgetTmp);
				widgetTmp->signalValue.connect([=](const bool& _value) {
						if (m_subWidget == nullptr) {
							APPL_ERROR("nullptr...");
							return;
						}
						APPL_INFO("set parameter : name=" << param->getName() << " value=" << _value);
						m_subWidget->parameterSet(param->getName(), etk::to_string(_value));
						return;
					});
			} else if (type == typeid(int64_t).name()) {
				type = "int64_t";
			} else if (type == typeid(int32_t).name()) {
				type = "int32_t";
			} else if (type == typeid(int16_t).name()) {
				type = "int16_t";
			} else if (type == typeid(int8_t).name()) {
				type = "int8_t";
			} else if (type == typeid(uint64_t).name()) {
				type = "uint64_t";
			} else if (type == typeid(uint32_t).name()) {
				type = "uint32_t";
			} else if (type == typeid(uint16_t).name()) {
				type = "uint16_t";
			} else if (type == typeid(uint8_t).name()) {
				type = "uint8_t";
			} else if (type == typeid(float).name()) {
				type = "float";
			} else if (type == typeid(double).name()) {
				type = "double";
			} else if (type == typeid(enum ewol::gravity).name()) {
				type = "enum ewol::gravity";
			}
		}
		mySpacer = ewol::widget::Spacer::create();
		if (mySpacer != nullptr) {
			mySpacer->setExpand(bvec2(true,false));
			mySpacer->setFill(bvec2(true,false));
			mySpacer->setMinSize(vec2(3,3));
			mySpacer->setColor(0x000000AA);
			m_sizerDynamic->subWidgetAddStart(mySpacer);
		}
	}
	mySpacer = ewol::widget::Spacer::create();
	if (mySpacer != nullptr) {
		mySpacer->setExpand(bvec2(true,true));
		m_sizerDynamic->subWidgetAddStart(mySpacer);
	}
}

