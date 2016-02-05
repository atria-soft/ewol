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
#include <ewol/parameter/Value.h>


static const char * l_eventChangeTheme           = "event-change-theme";
static const char * l_eventChangeWidgetNext      = "event-change-widget-test-next";
static const char * l_eventChangeWidgetPrevious  = "event-change-widget-test-previous";


#undef __class__
#define __class__ "MainWindows"

appl::MainWindows::MainWindows() :
  m_gravity(ewol::gravity_buttomLeft),
  m_idWidget(0) {
	APPL_DEBUG("CREATE WINDOWS ... ");
	addObjectType("appl::MainWindows");
}

void appl::MainWindows::init() {
	ewol::widget::Windows::init();
	
	m_composer = ewol::widget::Composer::create(ewol::widget::Composer::file, "DATA:gui.xml");
	setSubWidget(m_composer);
	externSubBind(m_composer, ewol::widget::Button, "appl-theme-toggle", signalValue, shared_from_this(), &appl::MainWindows::onCallbackThemeChange);
	externSubBind(m_composer, ewol::widget::Button, "appl-previous-widget", signalPressed, shared_from_this(), &appl::MainWindows::onCallbackWidgetChange, -1);
	externSubBind(m_composer, ewol::widget::Button, "appl-next-widget", signalPressed, shared_from_this(), &appl::MainWindows::onCallbackWidgetChange, 1);
	externSubBind(m_composer, ewol::widget::Button, "appl-next-gravity", signalPressed, shared_from_this(), &appl::MainWindows::onCallbackGravityChange);
	
	m_sizerVert = std::dynamic_pointer_cast<ewol::widget::Sizer>(m_composer->getSubObjectNamed("appl-upper-test-widget"));
	m_sizerDynamic = std::dynamic_pointer_cast<ewol::widget::Sizer>(m_composer->getSubObjectNamed("appl-dynamic-config"));
	m_subWidget = std::dynamic_pointer_cast<ewol::Widget>(m_composer->getSubObjectNamed("[TEST]TO-TEST"));
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

void appl::MainWindows::onCallbackGravityChange() {
	switch(m_gravity) {
		case ewol::gravity_center:
			m_gravity = ewol::gravity_topLeft;
			break;
		case ewol::gravity_topLeft:
			m_gravity = ewol::gravity_top;
			break;
		case ewol::gravity_top:
			m_gravity = ewol::gravity_topRight;
			break;
		case ewol::gravity_topRight:
			m_gravity = ewol::gravity_right;
			break;
		case ewol::gravity_right:
			m_gravity = ewol::gravity_buttomRight;
			break;
		case ewol::gravity_buttomRight:
			m_gravity = ewol::gravity_buttom;
			break;
		case ewol::gravity_buttom:
			m_gravity = ewol::gravity_buttomLeft;
			break;
		case ewol::gravity_buttomLeft:
			m_gravity = ewol::gravity_left;
			break;
		case ewol::gravity_left:
			m_gravity = ewol::gravity_center;
			break;
	}
	parameterSetOnWidgetNamed("appl-upper-test-widget", "gravity", ewol::gravityToString(m_gravity));
	parameterSetOnWidgetNamed("appl-next-gravity-label", "value", "Next gravity<br/>(" + ewol::gravityToString(m_gravity) + ")");
}

void appl::MainWindows::onCallbackWidgetChange(int32_t _increment) {
	m_idWidget += _increment;
	std::shared_ptr<ewol::Widget> oldWidget = m_subWidget;
	std::string tmpDescription;
	std::string tmpConstruct;
	switch(m_idWidget) {
		case 0:
			tmpConstruct = std::string()
			    + "<button name='[TEST]Button:TO-TEST' expand='false,false' fill='false,false' >\n"
			    + "    <label>My <font color='#FF0000'>Button</font> <br/> And Some under line<br/> plop <br/> and an other super long line ...</label>\n"
			    + "</button>\n";
			tmpDescription = "TestButton";
			break;
		case 1:
			tmpConstruct = "<ButtonColor/>";
			tmpDescription = "TestButtonColor";
			break;
		case 2:
			tmpConstruct = "<label>Simple string</label>\n";
			tmpDescription = "TestLabel";
			break;
		case 3:
			tmpConstruct = "<image src='DATA:sphere.png'/>\n";
			tmpDescription = "TestImage";
			break;
		case 4:
			tmpConstruct = "<checkbox/>\n";
			tmpDescription = "TestCheckbox";
			break;
		default:
			tmpConstruct = "<label expand=false fill=false>Simple string</label>\n";
			tmpDescription = "Test Label";
			m_idWidget = -1;
			break;
			/*
			break;
		case 1:
			m_subWidget = TestDistanceField::create();
			if (m_subWidget != nullptr) {
				m_sizerVert->subWidgetAdd(m_subWidget);
			}
			tmpDescription = "Test Distance Field";
			*/
			break;
	}
	// create the widget with a xml generator (readable for test ...):
	m_subWidget = ewol::widget::composerGenerate(ewol::widget::Composer::String, tmpConstruct);
	if (m_subWidget != nullptr) {
		m_sizerVert->subWidgetReplace(oldWidget, m_subWidget);
	}
	parameterSetOnWidgetNamed("appl-label-test", "value", tmpDescription);
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
				ewol::parameter::Parameter* param = m_subWidget->getParameterRaw(iii);
				ewol::parameter::Value<bvec2>* paramValue = dynamic_cast<ewol::parameter::Value<bvec2>*>(param);
				if (paramValue == nullptr) {
					APPL_ERROR("nullptr... 2 ");
					return;
				}
				bvec2 value = paramValue->get();
				widgetTmp->setValue(value.x());
				widgetTmp->signalValue.connect([=](const bool& _value) {
						APPL_INFO("set parameter : X name=" << param->getName() << " value=" << _value);
						bvec2 lastValueInterpreted = paramValue->get();
						lastValueInterpreted.setX(_value);
						paramValue->set(lastValueInterpreted);
						return;
					});
				std::shared_ptr<ewol::widget::Label> widgetLabel = ewol::widget::Label::create("x");
				widgetTmp->setSubWidget(widgetLabel);
				
				widgetTmp = ewol::widget::CheckBox::create();
				widgetSizer->subWidgetAdd(widgetTmp);
				widgetTmp->setValue(value.y());
				widgetTmp->signalValue.connect([=](const bool& _value) {
						APPL_INFO("set parameter : Y name=" << param->getName() << " value=" << _value);
						bvec2 lastValueInterpreted = paramValue->get();
						lastValueInterpreted.setY(_value);
						paramValue->set(lastValueInterpreted);
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
			mySpacer->setColor(etk::color::blue);
			m_sizerDynamic->subWidgetAddStart(mySpacer);
		}
	}
}
