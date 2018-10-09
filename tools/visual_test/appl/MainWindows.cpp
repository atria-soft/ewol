/** @file
 * @author Edouard DUPIN
 * @copyright 2010, Edouard DUPIN, all right reserved
 * @license BSD v3 (see license file)
 */


#include <appl/debug.hpp>
#include <appl/MainWindows.hpp>

#include <etk/theme/theme.hpp>

#include <ewol/widget/Button.hpp>
#include <ewol/widget/CheckBox.hpp>
#include <ewol/widget/Sizer.hpp>
#include <ewol/widget/Label.hpp>
#include <ewol/widget/Entry.hpp>
#include <ewol/widget/List.hpp>
#include <ewol/widget/ContextMenu.hpp>
#include <ewol/widget/PopUp.hpp>
#include <ewol/widget/Slider.hpp>
#include <ewol/widget/Menu.hpp>
#include <ewol/widget/meta/FileChooser.hpp>
#include <ewol/widget/meta/Parameter.hpp>
#include <ewol/widget/Select.hpp>
#include <ewol/widget/Manager.hpp>
#include <ewol/widget/Spin.hpp>
#include <ewol/context/Context.hpp>
#include <appl/TestDistanceField.hpp>
#include <eproperty/Value.hpp>

appl::MainWindows::MainWindows() :
  m_gravity(ewol::gravity_buttomLeft),
  m_idWidget(-1) {
	APPL_DEBUG("CREATE WINDOWS ... ");
	addObjectType("appl::MainWindows");
}

void appl::MainWindows::init() {
	ewol::widget::Windows::init();
	
		APPL_WARNING("plop " << ETK_GET_TYPE_NAME(etk::String));
		APPL_WARNING("plop " << ETK_GET_TYPE_NAME(vec3));
		APPL_WARNING("plop " << ETK_GET_TYPE_NAME(double));
		APPL_WARNING("plop " << ETK_GET_TYPE_NAME(uint32_t));
	m_composer = ewol::widget::Composer::create();
	m_composer->loadFromFile("DATA:///gui.xml");
	setSubWidget(m_composer);
	externSubBind(m_composer, ewol::widget::Button, "appl-theme-toggle", signalValue, sharedFromThis(), &appl::MainWindows::onCallbackThemeChange);
	externSubBind(m_composer, ewol::widget::Button, "appl-previous-widget", signalPressed, sharedFromThis(), &appl::MainWindows::onCallbackWidgetChange, -1);
	externSubBind(m_composer, ewol::widget::Button, "appl-next-widget", signalPressed, sharedFromThis(), &appl::MainWindows::onCallbackWidgetChange, 1);
	externSubBind(m_composer, ewol::widget::Button, "appl-next-gravity", signalPressed, sharedFromThis(), &appl::MainWindows::onCallbackGravityChange);
	
	m_sizerVert = ememory::dynamicPointerCast<ewol::widget::Sizer>(m_composer->getSubObjectNamed("appl-upper-test-widget"));
	if (m_sizerVert == null) {
		APPL_CRITICAL("Can not get vertical pointer");
	}
	m_sizerDynamic = ememory::dynamicPointerCast<ewol::widget::Sizer>(m_composer->getSubObjectNamed("appl-dynamic-config"));
	if (m_sizerDynamic == null) {
		APPL_CRITICAL("Can not get dynamic pointer");
	}
	m_subWidget = ememory::dynamicPointerCast<ewol::Widget>(m_composer->getSubObjectNamed("[TEST]TO-TEST"));
	if (m_subWidget == null) {
		APPL_CRITICAL("Can not get subWidget pointer");
	}
	shortCutAdd("F12",          "menu:reloade-shader");
	signalShortcut.connect(sharedFromThis(), &appl::MainWindows::onCallbackShortCut);
}

void appl::MainWindows::onCallbackShortCut(const etk::String& _value) {
	APPL_WARNING("Event from ShortCut : " << _value);
	if (_value == "menu:reloade-shader") {
		ewol::getContext().getResourcesManager().reLoadResources();
		ewol::getContext().forceRedrawAll();
	} else {
		APPL_ERROR("Event from Menu UNKNOW : '" << _value << "'");
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
	ememory::SharedPtr<ewol::Widget> oldWidget = m_subWidget;
	etk::String tmpDescription;
	etk::String tmpConstruct;
	switch(m_idWidget) {
		case 0:
			tmpConstruct = "<image src='DATA:///sphere.png'/>\n";
			tmpDescription = "Test ewol::widget::Image";
			break;
		case 1:
			tmpConstruct = etk::String()
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
			tmpConstruct = "<spin/>\n";
			tmpDescription = "Test ewol::widget::Spin";
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
			tmpConstruct = etk::String()
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
			if (m_subWidget != null) {
				m_sizerVert->subWidgetAdd(m_subWidget);
			}
			tmpDescription = "Test Distance Field";
			*/
			break;
	}
	// create the widget with a xml generator (readable for test ...):
	m_subWidget = ewol::widget::composerGenerateString(tmpConstruct);
	if (m_subWidget != null) {
		m_sizerVert->subWidgetReplace(oldWidget, m_subWidget);
	}
	propertySetOnWidgetNamed("appl-label-test", "value", tmpDescription);
	updateProperty();
}

static void addSpacer(ememory::SharedPtr<ewol::widget::Sizer> _sizer, etk::Color<> _color=etk::color::none) {
	ememory::SharedPtr<ewol::widget::Spacer> mySpacer = ewol::widget::Spacer::create();
	if (mySpacer != null) {
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
	if (m_subWidget == null) {
		return;
	}
	m_listConnection.clear();
	ememory::SharedPtr<ewol::widget::Label> widget = ewol::widget::Label::create();
	widget->propertyValue.set(m_subWidget->getObjectType());
	m_sizerDynamic->subWidgetAdd(widget);
	addSpacer(m_sizerDynamic, etk::color::red);
	for (size_t iii=0; iii<m_subWidget->properties.size(); ++iii) {
		eproperty::Property* param = m_subWidget->properties.getRaw(iii);
		if (param == null) {
			APPL_WARNING("Parameter EMPTY . " << iii << " : null");
			continue;
		}
		ememory::SharedPtr<ewol::widget::Sizer> widgetSizer = ewol::widget::Sizer::create();
		if (widgetSizer != null) {
			widgetSizer->propertyMode.set(ewol::widget::Sizer::modeHori);
			widgetSizer->propertyExpand.set(bvec2(true,false));
			widgetSizer->propertyFill.set(bvec2(true,true));
			m_sizerDynamic->subWidgetAddStart(widgetSizer);
			ememory::SharedPtr<ewol::widget::Label> widget = ewol::widget::Label::create();
			widget->propertyValue.set(param->getName() + ":");
			widgetSizer->subWidgetAdd(widget);
			etk::String type = param->getType();
			APPL_WARNING("Parameter TYPE=" << param->getType() << " name=" << param->getName());
			if (param->getPropertyType() != "eproperty::List") {
				//addSpacer(widgetSizer, etk::color::purple);
				// Main part TODO: ...
				if (type == ETK_GET_TYPE_NAME(etk::String)) {
					ewol::widget::EntryShared widgetTmp = ewol::widget::Entry::create();
					widgetSizer->subWidgetAdd(widgetTmp);
					eproperty::Value<etk::String>* paramValue = dynamic_cast<eproperty::Value<etk::String>*>(param);
					if (paramValue == null) {
						APPL_ERROR("null...");
						return;
					}
					etk::String value = paramValue->get();
					widgetTmp->propertyValue.set(value);
					widgetTmp->propertyExpand.set(bvec2(true,false));
					widgetTmp->propertyFill.set(bvec2(true,false));
					esignal::Connection conn = widgetTmp->signalModify.connect(
						[=](const etk::String& _value) {
							APPL_INFO("set parameter : NAME name=" << param->getName() << " value=" << _value);
							paramValue->set(_value);
							return;
						});
					m_listConnection.pushBack(etk::move(conn));
				} else if (type == ETK_GET_TYPE_NAME(gale::Dimension)) {
					addSpacer(widgetSizer);
					ewol::widget::SpinShared widgetTmp = ewol::widget::Spin::create();
					widgetSizer->subWidgetAdd(widgetTmp);
					eproperty::Value<gale::Dimension>* paramValue = dynamic_cast<eproperty::Value<gale::Dimension>*>(param);
					if (paramValue == null) {
						APPL_ERROR("null... 2 ");
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
					m_listConnection.pushBack(etk::move(conn));
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
					m_listConnection.pushBack(etk::move(conn));
					widgetLabel = ewol::widget::Label::create();
					widgetLabel->propertyValue.set("y");
					widgetSizer->subWidgetAdd(widgetLabel);
					
					ewol::widget::SelectShared widgetSelectTmp = ewol::widget::Select::create();
					widgetSizer->subWidgetAdd(widgetSelectTmp);
					widgetSelectTmp->propertyExpand.set(bvec2(true,false));
					widgetSelectTmp->propertyFill.set(bvec2(true,false));
					widgetSelectTmp->optionAdd(int32_t(gale::distance::pourcent), "Pourcent");
					widgetSelectTmp->optionAdd(int32_t(gale::distance::pixel), "Pixel");
					widgetSelectTmp->optionAdd(int32_t(gale::distance::meter), "Meter");
					widgetSelectTmp->optionAdd(int32_t(gale::distance::centimeter), "Centimeter");
					widgetSelectTmp->optionAdd(int32_t(gale::distance::millimeter), "Millimeter");
					widgetSelectTmp->optionAdd(int32_t(gale::distance::kilometer), "Kilometer");
					widgetSelectTmp->optionAdd(int32_t(gale::distance::inch), "Inch");
					widgetSelectTmp->optionAdd(int32_t(gale::distance::foot), "foot");
					widgetSelectTmp->propertyValue.set(int32_t(value.getType()));
					conn = widgetSelectTmp->signalValue.connect(
						[=](const int32_t& _value) {
							APPL_INFO("set parameter: gravity name=" << param->getName() << " value=" << (enum gale::distance)_value);
							gale::Dimension lastValueInterpreted = paramValue->get();
							lastValueInterpreted.set(lastValueInterpreted.get(lastValueInterpreted.getType()), (enum gale::distance)_value);
							paramValue->set(lastValueInterpreted);
							return;
						});
					m_listConnection.pushBack(etk::move(conn));
					
				} else if (type == ETK_GET_TYPE_NAME(bvec2)) {
					addSpacer(widgetSizer);
					ewol::widget::CheckBoxShared widgetTmp = ewol::widget::CheckBox::create();
					widgetSizer->subWidgetAdd(widgetTmp);
					eproperty::Value<bvec2>* paramValue = dynamic_cast<eproperty::Value<bvec2>*>(param);
					if (paramValue == null) {
						APPL_ERROR("null... 2 ");
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
					m_listConnection.pushBack(etk::move(conn));
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
					m_listConnection.pushBack(etk::move(conn));
					widgetLabel = ewol::widget::Label::create();
					widgetLabel->propertyValue.set("y");
					widgetTmp->setSubWidget(widgetLabel);
				} else if (type == ETK_GET_TYPE_NAME(ivec2)) {
					addSpacer(widgetSizer);
					ewol::widget::SpinShared widgetTmp = ewol::widget::Spin::create();
					widgetSizer->subWidgetAdd(widgetTmp);
					eproperty::Value<ivec2>* paramValue = dynamic_cast<eproperty::Value<ivec2>*>(param);
					if (paramValue == null) {
						APPL_ERROR("null... 2 ");
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
					m_listConnection.pushBack(etk::move(conn));
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
					m_listConnection.pushBack(etk::move(conn));
					widgetLabel = ewol::widget::Label::create();
					widgetLabel->propertyValue.set("y");
					widgetSizer->subWidgetAdd(widgetLabel);
				} else if (type == ETK_GET_TYPE_NAME(uivec2)) {
					addSpacer(widgetSizer);
					ewol::widget::SpinShared widgetTmp = ewol::widget::Spin::create();
					widgetSizer->subWidgetAdd(widgetTmp);
					eproperty::Value<uivec2>* paramValue = dynamic_cast<eproperty::Value<uivec2>*>(param);
					if (paramValue == null) {
						APPL_ERROR("null... 2 ");
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
					m_listConnection.pushBack(etk::move(conn));
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
					m_listConnection.pushBack(etk::move(conn));
					widgetLabel = ewol::widget::Label::create();
					widgetLabel->propertyValue.set("y");
					widgetSizer->subWidgetAdd(widgetLabel);
				} else if (type == ETK_GET_TYPE_NAME(vec2)) {
					addSpacer(widgetSizer);
					ewol::widget::SpinShared widgetTmp = ewol::widget::Spin::create();
					widgetSizer->subWidgetAdd(widgetTmp);
					eproperty::Value<vec2>* paramValue = dynamic_cast<eproperty::Value<vec2>*>(param);
					if (paramValue == null) {
						APPL_ERROR("null... 2 ");
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
					m_listConnection.pushBack(etk::move(conn));
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
					m_listConnection.pushBack(etk::move(conn));
					widgetLabel = ewol::widget::Label::create();
					widgetLabel->propertyValue.set("y");
					widgetSizer->subWidgetAdd(widgetLabel);
				} else if (type == ETK_GET_TYPE_NAME(bool)) {
					addSpacer(widgetSizer);
					ewol::widget::CheckBoxShared widgetTmp = ewol::widget::CheckBox::create();
					widgetSizer->subWidgetAdd(widgetTmp);
					esignal::Connection conn = widgetTmp->signalValue.connect(
						[=](const bool& _value) {
							if (m_subWidget == null) {
								APPL_ERROR("null...");
								return;
							}
							APPL_INFO("set parameter : name=" << param->getName() << " value=" << _value);
							m_subWidget->properties.set(param->getName(), etk::toString(_value));
							return;
						});
					m_listConnection.pushBack(etk::move(conn));
				} else if (    type == ETK_GET_TYPE_NAME(int64_t)
				            || type == ETK_GET_TYPE_NAME(int32_t)
				            || type == ETK_GET_TYPE_NAME(int16_t)
				            || type == ETK_GET_TYPE_NAME(int8_t)
				            || type == ETK_GET_TYPE_NAME(uint64_t)
				            || type == ETK_GET_TYPE_NAME(uint32_t)
				            || type == ETK_GET_TYPE_NAME(uint16_t)
				            || type == ETK_GET_TYPE_NAME(uint8_t)) {
					ewol::widget::SpinShared widgetTmp = ewol::widget::Spin::create();
					widgetSizer->subWidgetAdd(widgetTmp);
					etk::String value = param->getString();
					widgetTmp->propertyValue.set(etk::string_to_int8_t(value));
					widgetTmp->propertyExpand.set(bvec2(true,false));
					widgetTmp->propertyFill.set(bvec2(true,false));
					
					esignal::Connection conn = widgetTmp->signalValue.connect(
						[=](const int64_t& _value) {
							APPL_INFO("set parameter : NAME name=" << param->getName() << " value=" << _value);
							param->setString(etk::toString(_value));
							return;
						});
					m_listConnection.pushBack(etk::move(conn));
					if (type == ETK_GET_TYPE_NAME(int64_t)) {
						widgetTmp->propertyMin.set(0x8000000000000000LL);
						widgetTmp->propertyMax.set(0x7FFFFFFFFFFFFFFFLL);
					} else if (type == ETK_GET_TYPE_NAME(int32_t)) {
						widgetTmp->propertyMin.set(0x80000000LL);
						widgetTmp->propertyMax.set(0x7FFFFFFFLL);
					} else if (type == ETK_GET_TYPE_NAME(int16_t)) {
						widgetTmp->propertyMin.set(-65338);
						widgetTmp->propertyMax.set(65337);
					} else if (type == ETK_GET_TYPE_NAME(int8_t)) {
						widgetTmp->propertyMin.set(-128);
						widgetTmp->propertyMax.set(127);
					} else if (type == ETK_GET_TYPE_NAME(uint64_t)) {
						widgetTmp->propertyMin.set(0);
						widgetTmp->propertyMax.set(0x7FFFFFFFFFFFFFFFLL);
					} else if (type == ETK_GET_TYPE_NAME(uint32_t)) {
						widgetTmp->propertyMin.set(0);
						widgetTmp->propertyMax.set(0x7FFFFFFFLL);
					} else if (type == ETK_GET_TYPE_NAME(uint16_t)) {
						widgetTmp->propertyMin.set(0);
						widgetTmp->propertyMax.set(65337*2);
					} else if (type == ETK_GET_TYPE_NAME(uint8_t)) {
						widgetTmp->propertyMin.set(0);
						widgetTmp->propertyMax.set(256);
					}
				} else if (type == ETK_GET_TYPE_NAME(float)) {
					type = "float";
				} else if (type == ETK_GET_TYPE_NAME(double)) {
					type = "double";
				}
			} else {
				// property list ...
				etk::Vector<etk::String> listElement = param->getListValue();
				ewol::widget::SelectShared widgetTmp = ewol::widget::Select::create();
				widgetSizer->subWidgetAdd(widgetTmp);
				widgetTmp->propertyExpand.set(bvec2(true,false));
				widgetTmp->propertyFill.set(bvec2(true,false));
				etk::String value = param->getString();
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
				m_listConnection.pushBack(etk::move(conn));
			}
		}
		ewol::widget::SpacerShared mySpacer = ewol::widget::Spacer::create();
		if (mySpacer != null) {
			mySpacer->propertyExpand.set(bvec2(true,false));
			mySpacer->propertyFill.set(bvec2(true,false));
			mySpacer->propertyMinSize.set(vec2(3,3));
			mySpacer->propertyColor.set(etk::color::blue);
			m_sizerDynamic->subWidgetAddStart(mySpacer);
		}
	}
}

