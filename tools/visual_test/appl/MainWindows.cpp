/** @file
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
#include <ewol/widget/Select.h>
#include <ewol/widget/Manager.h>
#include <ewol/widget/Spin.h>
#include <ewol/context/Context.h>
#include <appl/TestDistanceField.h>
#include <etk/os/FSNode.h>
#include <eproperty/Value.h>

appl::MainWindows::MainWindows() :
  m_gravity(ewol::gravity_buttomLeft),
  m_idWidget(-1) {
	APPL_DEBUG("CREATE WINDOWS ... ");
	addObjectType("appl::MainWindows");
}

void appl::MainWindows::init() {
	ewol::widget::Windows::init();
	
	m_composer = ewol::widget::Composer::create();
	m_composer->loadFromFile("DATA:gui.xml");
	setSubWidget(m_composer);
	externSubBind(m_composer, ewol::widget::Button, "appl-theme-toggle", signalValue, shared_from_this(), &appl::MainWindows::onCallbackThemeChange);
	externSubBind(m_composer, ewol::widget::Button, "appl-previous-widget", signalPressed, shared_from_this(), &appl::MainWindows::onCallbackWidgetChange, -1);
	externSubBind(m_composer, ewol::widget::Button, "appl-next-widget", signalPressed, shared_from_this(), &appl::MainWindows::onCallbackWidgetChange, 1);
	externSubBind(m_composer, ewol::widget::Button, "appl-next-gravity", signalPressed, shared_from_this(), &appl::MainWindows::onCallbackGravityChange);
	
	m_sizerVert = std::dynamic_pointer_cast<ewol::widget::Sizer>(m_composer->getSubObjectNamed("appl-upper-test-widget"));
	if (m_sizerVert == nullptr) {
		APPL_CRITICAL("Can not get vertical pointer");
	}
	m_sizerDynamic = std::dynamic_pointer_cast<ewol::widget::Sizer>(m_composer->getSubObjectNamed("appl-dynamic-config"));
	if (m_sizerDynamic == nullptr) {
		APPL_CRITICAL("Can not get dynamic pointer");
	}
	m_subWidget = std::dynamic_pointer_cast<ewol::Widget>(m_composer->getSubObjectNamed("[TEST]TO-TEST"));
	if (m_subWidget == nullptr) {
		APPL_CRITICAL("Can not get subWidget pointer");
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
	propertySetOnWidgetNamed("appl-upper-test-widget", "gravity", ewol::gravityToString(m_gravity));
	propertySetOnWidgetNamed("appl-next-gravity-label", "value", "Next gravity<br/>(" + ewol::gravityToString(m_gravity) + ")");
}

void appl::MainWindows::onCallbackWidgetChange(int32_t _increment) {
	m_idWidget += _increment;
	std::shared_ptr<ewol::Widget> oldWidget = m_subWidget;
	std::string tmpDescription;
	std::string tmpConstruct;
	switch(m_idWidget) {
		case 0:
			tmpConstruct = "<spin/>\n";
			tmpDescription = "Test ewol::widget::Spin";
			break;
		case 1:
			tmpConstruct = std::string()
			    + "<select>\n"
			    + "    <option id='1'>plop 1</option>\n"
			    + "    <option id='2'>plop 2</option>\n"
			    + "    <option id='3' select='true'>plop 3</option>\n"
			    + "    <option id='4'>plop 4</option>\n"
			    + "    <option id='5'>plop 5</option>\n"
			    + "</select>\n";
			tmpDescription = "Test ewol::widget::Select";
			break;
		case 2:
			tmpConstruct = "<ButtonColor/>";
			tmpDescription = "Test ewol::widget::ButtonColor";
			break;
		case 3:
			tmpConstruct = "<label>Simple string</label>\n";
			tmpDescription = "Test ewol::widget::Label";
			break;
		case 4:
			tmpConstruct = "<image src='DATA:sphere.png'/>\n";
			tmpDescription = "Test ewol::widget::Image";
			break;
		case 5:
			tmpConstruct = "<checkbox><label>Simple string</label></checkbox>\n";
			tmpDescription = "Test ewol::widget::Checkbox";
			break;
		case 6:
			tmpConstruct = "<entry/>\n";
			tmpDescription = "Test ewol::widget::Entry";
			break;
		case 7:
			tmpConstruct = "<slider/>\n";
			tmpDescription = "Test ewol::widget::Entry";
			break;
		case 8:
			tmpConstruct = std::string()
			    + "<button name='[TEST]Button:TO-TEST' expand='false,false' fill='false,false' >\n"
			    + "    <label>My <font color='#FF0000'>Button</font> <br/> And Some under line<br/> plop <br/> and an other super long line ...</label>\n"
			    + "</button>\n";
			tmpDescription = "Test ewol::widget::Button";
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
	m_subWidget = ewol::widget::composerGenerateString(tmpConstruct);
	if (m_subWidget != nullptr) {
		m_sizerVert->subWidgetReplace(oldWidget, m_subWidget);
	}
	propertySetOnWidgetNamed("appl-label-test", "value", tmpDescription);
	updateProperty();
}

static void addSpacer(const std::shared_ptr<ewol::widget::Sizer>& _sizer, etk::Color<> _color=etk::color::none) {
	std::shared_ptr<ewol::widget::Spacer> mySpacer = ewol::widget::Spacer::create();
	if (mySpacer != nullptr) {
		mySpacer->propertyExpand.set(bvec2(true,false));
		mySpacer->propertyFill.set(bvec2(true,false));
		if (_color == etk::color::none) {
			mySpacer->propertyMinSize.set(vec2(3,3));
			mySpacer->propertyColor.set(_color);
		}
		_sizer->subWidgetAdd(mySpacer);
	}
}

void appl::MainWindows::updateProperty() {
	// remove all elements:
	m_sizerDynamic->subWidgetRemoveAll();
	if (m_subWidget == nullptr) {
		return;
	}
	m_listConnection.clear();
	std::shared_ptr<ewol::widget::Label> widget = ewol::widget::Label::create();
	widget->propertyValue.set(m_subWidget->getObjectType());
	m_sizerDynamic->subWidgetAdd(widget);
	addSpacer(m_sizerDynamic, etk::color::red);
	for (size_t iii=0; iii<m_subWidget->properties.size(); ++iii) {
		eproperty::Property* param = m_subWidget->properties.getRaw(iii);
		if (param == nullptr) {
			APPL_WARNING("Parameter EMPTY . " << iii << " : nullptr");
			continue;
		}
		std::shared_ptr<ewol::widget::Sizer> widgetSizer = ewol::widget::Sizer::create();
		if (widgetSizer != nullptr) {
			widgetSizer->propertyMode.set(ewol::widget::Sizer::modeHori);
			widgetSizer->propertyExpand.set(bvec2(true,false));
			widgetSizer->propertyFill.set(bvec2(true,true));
			m_sizerDynamic->subWidgetAddStart(widgetSizer);
			std::shared_ptr<ewol::widget::Label> widget = ewol::widget::Label::create();
			widget->propertyValue.set(param->getName() + ":");
			widgetSizer->subWidgetAdd(widget);
			std::string type = param->getType();
			if (param->getPropertyType() != "eproperty::List") {
				//addSpacer(widgetSizer, etk::color::purple);
				// Main part TODO: ...
				if (type == typeid(std::string).name()) {
					ewol::widget::EntryShared widgetTmp = ewol::widget::Entry::create();
					widgetSizer->subWidgetAdd(widgetTmp);
					eproperty::Value<std::string>* paramValue = dynamic_cast<eproperty::Value<std::string>*>(param);
					if (paramValue == nullptr) {
						APPL_ERROR("nullptr...");
						return;
					}
					std::string value = paramValue->get();
					widgetTmp->propertyValue.set(value);
					widgetTmp->propertyExpand.set(bvec2(true,false));
					widgetTmp->propertyFill.set(bvec2(true,false));
					esignal::Connection conn = widgetTmp->signalModify.connect(
						[=](const std::string& _value) {
							APPL_INFO("set parameter : NAME name=" << param->getName() << " value=" << _value);
							paramValue->set(_value);
							return;
						});
					m_listConnection.push_back(std::move(conn));
				} else if (type == typeid(gale::Dimension).name()) {
					addSpacer(widgetSizer);
					ewol::widget::SpinShared widgetTmp = ewol::widget::Spin::create();
					widgetSizer->subWidgetAdd(widgetTmp);
					eproperty::Value<gale::Dimension>* paramValue = dynamic_cast<eproperty::Value<gale::Dimension>*>(param);
					if (paramValue == nullptr) {
						APPL_ERROR("nullptr... 2 ");
						return;
					}
					gale::Dimension value = paramValue->get();
					widgetTmp->propertyMantis.set(3);
					widgetTmp->propertyValue.set(value.get(value.getType()).x()*1000);
					esignal::Connection conn = widgetTmp->signalValueDouble.connect(
						[=](const double& _value) {
							APPL_INFO("set parameter : X name=" << param->getName() << " value=" << _value);
							gale::Dimension lastValueInterpreted = paramValue->get();
							vec2 val = lastValueInterpreted.get(lastValueInterpreted.getType());
							val.setX(_value);
							lastValueInterpreted.set(val, lastValueInterpreted.getType());
							paramValue->set(lastValueInterpreted);
							return;
						});
					m_listConnection.push_back(std::move(conn));
					ewol::widget::LabelShared widgetLabel = ewol::widget::Label::create();
					widgetLabel->propertyValue.set("x");
					widgetSizer->subWidgetAdd(widgetLabel);
					
					widgetTmp = ewol::widget::Spin::create();
					widgetSizer->subWidgetAdd(widgetTmp);
					widgetTmp->propertyValue.set(value.get(value.getType()).y()*1000);
					widgetTmp->propertyMantis.set(3);
					conn = widgetTmp->signalValueDouble.connect(
						[=](const double& _value) {
							APPL_INFO("set parameter : Y name=" << param->getName() << " value=" << _value);
							gale::Dimension lastValueInterpreted = paramValue->get();
							vec2 val = lastValueInterpreted.get(lastValueInterpreted.getType());
							val.setY(_value);
							lastValueInterpreted.set(val, lastValueInterpreted.getType());
							paramValue->set(lastValueInterpreted);
							return;
						});
					m_listConnection.push_back(std::move(conn));
					widgetLabel = ewol::widget::Label::create();
					widgetLabel->propertyValue.set("y");
					widgetSizer->subWidgetAdd(widgetLabel);
					
					ewol::widget::SelectShared widgetSelectTmp = ewol::widget::Select::create();
					widgetSizer->subWidgetAdd(widgetSelectTmp);
					widgetSelectTmp->propertyExpand.set(bvec2(true,false));
					widgetSelectTmp->propertyFill.set(bvec2(true,false));
					widgetSelectTmp->optionAdd(gale::Dimension::Pourcent, "Pourcent");
					widgetSelectTmp->optionAdd(gale::Dimension::Pixel, "Pixel");
					widgetSelectTmp->optionAdd(gale::Dimension::Meter, "Meter");
					widgetSelectTmp->optionAdd(gale::Dimension::Centimeter, "Centimeter");
					widgetSelectTmp->optionAdd(gale::Dimension::Millimeter, "Millimeter");
					widgetSelectTmp->optionAdd(gale::Dimension::Kilometer, "Kilometer");
					widgetSelectTmp->optionAdd(gale::Dimension::Inch, "Inch");
					widgetSelectTmp->optionAdd(gale::Dimension::foot, "foot");
					widgetSelectTmp->propertyValue.set(value.getType());
					conn = widgetSelectTmp->signalValue.connect(
						[=](const int32_t& _value) {
							APPL_INFO("set parameter: gravity name=" << param->getName() << " value=" << (enum gale::Dimension::distance)_value);
							gale::Dimension lastValueInterpreted = paramValue->get();
							lastValueInterpreted.set(lastValueInterpreted.get(lastValueInterpreted.getType()), (enum gale::Dimension::distance)_value);
							paramValue->set(lastValueInterpreted);
							return;
						});
					m_listConnection.push_back(std::move(conn));
					
				} else if (type == typeid(bvec2).name()) {
					addSpacer(widgetSizer);
					ewol::widget::CheckBoxShared widgetTmp = ewol::widget::CheckBox::create();
					widgetSizer->subWidgetAdd(widgetTmp);
					eproperty::Value<bvec2>* paramValue = dynamic_cast<eproperty::Value<bvec2>*>(param);
					if (paramValue == nullptr) {
						APPL_ERROR("nullptr... 2 ");
						return;
					}
					bvec2 value = paramValue->get();
					widgetTmp->propertyValue.set(value.x());
					esignal::Connection conn = widgetTmp->signalValue.connect(
						[=](const bool& _value) {
							APPL_INFO("set parameter : X name=" << param->getName() << " value=" << _value);
							bvec2 lastValueInterpreted = paramValue->get();
							lastValueInterpreted.setX(_value);
							paramValue->set(lastValueInterpreted);
							return;
						});
					m_listConnection.push_back(std::move(conn));
					ewol::widget::LabelShared widgetLabel = ewol::widget::Label::create();
					widgetLabel->propertyValue.set("x");
					widgetTmp->setSubWidget(widgetLabel);
					
					widgetTmp = ewol::widget::CheckBox::create();
					widgetSizer->subWidgetAdd(widgetTmp);
					widgetTmp->propertyValue.set(value.y());
					conn = widgetTmp->signalValue.connect(
						[=](const bool& _value) {
							APPL_INFO("set parameter : Y name=" << param->getName() << " value=" << _value);
							bvec2 lastValueInterpreted = paramValue->get();
							lastValueInterpreted.setY(_value);
							paramValue->set(lastValueInterpreted);
							return;
						});
					m_listConnection.push_back(std::move(conn));
					widgetLabel = ewol::widget::Label::create();
					widgetLabel->propertyValue.set("y");
					widgetTmp->setSubWidget(widgetLabel);
				} else if (type == typeid(ivec2).name()) {
					addSpacer(widgetSizer);
					ewol::widget::SpinShared widgetTmp = ewol::widget::Spin::create();
					widgetSizer->subWidgetAdd(widgetTmp);
					eproperty::Value<ivec2>* paramValue = dynamic_cast<eproperty::Value<ivec2>*>(param);
					if (paramValue == nullptr) {
						APPL_ERROR("nullptr... 2 ");
						return;
					}
					ivec2 value = paramValue->get();
					widgetTmp->propertyValue.set(value.x());
					esignal::Connection conn = widgetTmp->signalValue.connect(
						[=](const int32_t& _value) {
							APPL_INFO("set parameter : X name=" << param->getName() << " value=" << _value);
							ivec2 lastValueInterpreted = paramValue->get();
							lastValueInterpreted.setX(_value);
							paramValue->set(lastValueInterpreted);
							return;
						});
					m_listConnection.push_back(std::move(conn));
					ewol::widget::LabelShared widgetLabel = ewol::widget::Label::create();
					widgetLabel->propertyValue.set("x");
					widgetSizer->subWidgetAdd(widgetLabel);
					
					widgetTmp = ewol::widget::Spin::create();
					widgetSizer->subWidgetAdd(widgetTmp);
					widgetTmp->propertyValue.set(value.y());
					conn = widgetTmp->signalValue.connect(
						[=](const int32_t& _value) {
							APPL_INFO("set parameter : Y name=" << param->getName() << " value=" << _value);
							ivec2 lastValueInterpreted = paramValue->get();
							lastValueInterpreted.setY(_value);
							paramValue->set(lastValueInterpreted);
							return;
						});
					m_listConnection.push_back(std::move(conn));
					widgetLabel = ewol::widget::Label::create();
					widgetLabel->propertyValue.set("y");
					widgetSizer->subWidgetAdd(widgetLabel);
				} else if (type == typeid(uivec2).name()) {
					addSpacer(widgetSizer);
					ewol::widget::SpinShared widgetTmp = ewol::widget::Spin::create();
					widgetSizer->subWidgetAdd(widgetTmp);
					eproperty::Value<uivec2>* paramValue = dynamic_cast<eproperty::Value<uivec2>*>(param);
					if (paramValue == nullptr) {
						APPL_ERROR("nullptr... 2 ");
						return;
					}
					uivec2 value = paramValue->get();
					widgetTmp->propertyValue.set(value.x());
					esignal::Connection conn = widgetTmp->signalValue.connect(
						[=](const uint32_t& _value) {
							APPL_INFO("set parameter : X name=" << param->getName() << " value=" << _value);
							uivec2 lastValueInterpreted = paramValue->get();
							lastValueInterpreted.setX(_value);
							paramValue->set(lastValueInterpreted);
							return;
						});
					m_listConnection.push_back(std::move(conn));
					ewol::widget::LabelShared widgetLabel = ewol::widget::Label::create();
					widgetLabel->propertyValue.set("x");
					widgetSizer->subWidgetAdd(widgetLabel);
					
					widgetTmp = ewol::widget::Spin::create();
					widgetSizer->subWidgetAdd(widgetTmp);
					widgetTmp->propertyValue.set(value.y());
					conn = widgetTmp->signalValue.connect(
						[=](const uint32_t& _value) {
							APPL_INFO("set parameter : Y name=" << param->getName() << " value=" << _value);
							uivec2 lastValueInterpreted = paramValue->get();
							lastValueInterpreted.setY(_value);
							paramValue->set(lastValueInterpreted);
							return;
						});
					m_listConnection.push_back(std::move(conn));
					widgetLabel = ewol::widget::Label::create();
					widgetLabel->propertyValue.set("y");
					widgetSizer->subWidgetAdd(widgetLabel);
				} else if (type == typeid(vec2).name()) {
					addSpacer(widgetSizer);
					ewol::widget::SpinShared widgetTmp = ewol::widget::Spin::create();
					widgetSizer->subWidgetAdd(widgetTmp);
					eproperty::Value<vec2>* paramValue = dynamic_cast<eproperty::Value<vec2>*>(param);
					if (paramValue == nullptr) {
						APPL_ERROR("nullptr... 2 ");
						return;
					}
					vec2 value = paramValue->get();
					widgetTmp->propertyMantis.set(3);
					widgetTmp->propertyValue.set(value.x()*1000);
					esignal::Connection conn = widgetTmp->signalValueDouble.connect(
						[=](const double& _value) {
							APPL_INFO("set parameter : X name=" << param->getName() << " value=" << _value);
							vec2 lastValueInterpreted = paramValue->get();
							lastValueInterpreted.setX(_value);
							paramValue->set(lastValueInterpreted);
							return;
						});
					m_listConnection.push_back(std::move(conn));
					ewol::widget::LabelShared widgetLabel = ewol::widget::Label::create();
					widgetLabel->propertyValue.set("x");
					widgetSizer->subWidgetAdd(widgetLabel);
					
					widgetTmp = ewol::widget::Spin::create();
					widgetSizer->subWidgetAdd(widgetTmp);
					widgetTmp->propertyValue.set(value.y()*1000);
					widgetTmp->propertyMantis.set(3);
					conn = widgetTmp->signalValueDouble.connect(
						[=](const double& _value) {
							APPL_INFO("set parameter : Y name=" << param->getName() << " value=" << _value);
							vec2 lastValueInterpreted = paramValue->get();
							lastValueInterpreted.setY(_value);
							paramValue->set(lastValueInterpreted);
							return;
						});
					m_listConnection.push_back(std::move(conn));
					widgetLabel = ewol::widget::Label::create();
					widgetLabel->propertyValue.set("y");
					widgetSizer->subWidgetAdd(widgetLabel);
				} else if (type == typeid(bool).name()) {
					addSpacer(widgetSizer);
					ewol::widget::CheckBoxShared widgetTmp = ewol::widget::CheckBox::create();
					widgetSizer->subWidgetAdd(widgetTmp);
					esignal::Connection conn = widgetTmp->signalValue.connect(
						[=](const bool& _value) {
							if (m_subWidget == nullptr) {
								APPL_ERROR("nullptr...");
								return;
							}
							APPL_INFO("set parameter : name=" << param->getName() << " value=" << _value);
							m_subWidget->properties.set(param->getName(), etk::to_string(_value));
							return;
						});
					m_listConnection.push_back(std::move(conn));
				} else if (    type == typeid(int64_t).name()
				            || type == typeid(int32_t).name()
				            || type == typeid(int16_t).name()
				            || type == typeid(int8_t).name()
				            || type == typeid(uint64_t).name()
				            || type == typeid(uint32_t).name()
				            || type == typeid(uint16_t).name()
				            || type == typeid(uint8_t).name()) {
					ewol::widget::SpinShared widgetTmp = ewol::widget::Spin::create();
					widgetSizer->subWidgetAdd(widgetTmp);
					std::string value = param->getString();
					widgetTmp->propertyValue.set(etk::string_to_int8_t(value));
					widgetTmp->propertyExpand.set(bvec2(true,false));
					widgetTmp->propertyFill.set(bvec2(true,false));
					
					esignal::Connection conn = widgetTmp->signalValue.connect(
						[=](const int64_t& _value) {
							APPL_INFO("set parameter : NAME name=" << param->getName() << " value=" << _value);
							param->setString(etk::to_string(_value));
							return;
						});
					m_listConnection.push_back(std::move(conn));
					if (type == typeid(int64_t).name()) {
						widgetTmp->propertyMin.set(0x8000000000000000LL);
						widgetTmp->propertyMax.set(0x7FFFFFFFFFFFFFFFLL);
					} else if (type == typeid(int32_t).name()) {
						widgetTmp->propertyMin.set(0x80000000LL);
						widgetTmp->propertyMax.set(0x7FFFFFFFLL);
					} else if (type == typeid(int16_t).name()) {
						widgetTmp->propertyMin.set(-65338);
						widgetTmp->propertyMax.set(65337);
					} else if (type == typeid(int8_t).name()) {
						widgetTmp->propertyMin.set(-128);
						widgetTmp->propertyMax.set(127);
					} else if (type == typeid(uint64_t).name()) {
						widgetTmp->propertyMin.set(0);
						widgetTmp->propertyMax.set(0x7FFFFFFFFFFFFFFFLL);
					} else if (type == typeid(uint32_t).name()) {
						widgetTmp->propertyMin.set(0);
						widgetTmp->propertyMax.set(0x7FFFFFFFLL);
					} else if (type == typeid(uint16_t).name()) {
						widgetTmp->propertyMin.set(0);
						widgetTmp->propertyMax.set(65337*2);
					} else if (type == typeid(uint8_t).name()) {
						widgetTmp->propertyMin.set(0);
						widgetTmp->propertyMax.set(256);
					}
				} else if (type == typeid(float).name()) {
					type = "float";
				} else if (type == typeid(double).name()) {
					type = "double";
				}
			} else {
				// property list ...
				std::vector<std::string> listElement = param->getListValue();
				ewol::widget::SelectShared widgetTmp = ewol::widget::Select::create();
				widgetSizer->subWidgetAdd(widgetTmp);
				widgetTmp->propertyExpand.set(bvec2(true,false));
				widgetTmp->propertyFill.set(bvec2(true,false));
				std::string value = param->getString();
				int32_t selectId = 0;
				for (int32_t iii=0; iii<listElement.size(); ++iii) {
					widgetTmp->optionAdd(iii, listElement[iii]);
					if (listElement[iii] == value) {
						selectId = iii;
					}
				}
				widgetTmp->propertyValue.set(selectId);
				esignal::Connection conn = widgetTmp->signalValue.connect(
					[=](const int32_t& _value) {
						APPL_INFO("set parameter: gravity name=" << param->getName() << " value=" << listElement[_value]);
						param->setString(listElement[_value]);
						return;
					});
				m_listConnection.push_back(std::move(conn));
			}
		}
		ewol::widget::SpacerShared mySpacer = ewol::widget::Spacer::create();
		if (mySpacer != nullptr) {
			mySpacer->propertyExpand.set(bvec2(true,false));
			mySpacer->propertyFill.set(bvec2(true,false));
			mySpacer->propertyMinSize.set(vec2(3,3));
			mySpacer->propertyColor.set(etk::color::blue);
			m_sizerDynamic->subWidgetAddStart(mySpacer);
		}
	}
}

