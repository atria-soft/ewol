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
	std::shared_ptr<ewol::widget::Button> myButton;
	
	m_sizerVert = ewol::widget::Sizer::create(ewol::widget::Sizer::modeVert);
	if (m_sizerVert == nullptr) {
		APPL_ERROR("Allocation error mySizerVert");
		return;
	}
	setSubWidget(m_sizerVert);
		
		mySizerHori = ewol::widget::Sizer::create(ewol::widget::Sizer::modeHori);
		if (mySizerHori == nullptr) {
			APPL_ERROR("Allocation error mySizerHori");
			return;
		}
		m_sizerVert->subWidgetAdd(mySizerHori);
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
			m_sizerVert->subWidgetAdd(mySpacer);
		}
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
}

