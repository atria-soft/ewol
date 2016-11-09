/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */

#include <etk/types.hpp>
#include <etk/types.hpp>
#include <ewol/ewol.hpp>
#include <gale/renderer/openGL/openGL.hpp>
#include <gale/renderer/openGL/openGL-include.hpp>
#include <ewol/context/Context.hpp>
#include <ewol/widget/Widget.hpp>
#include <ewol/widget/Windows.hpp>
#include <ewol/widget/Manager.hpp>
#include <ewol/widget/meta/StdPopUp.hpp>

ewol::widget::Windows::Windows() :
  propertyColorConfiguration(this, "file-color", "{ewol}THEME:COLOR:Windows.json", "color file link on the theme", &ewol::widget::Windows::onChangePropertyColor),
  propertyTitle(this, "title", "No title", "Title of the windows", &ewol::widget::Windows::onChangePropertyTitle),
  m_resourceColor(nullptr),
  m_colorBg(-1) {
	addObjectType("ewol::widget::Windows");
	propertyCanFocus.setDirectCheck(true);
	//KeyboardShow(KEYBOARD_MODE_CODE);
}


void ewol::widget::Windows::init() {
	ewol::Widget::init();
	onChangePropertyColor();
}

ewol::widget::Windows::~Windows() {
	m_subWidget.reset();
	m_popUpWidgetList.clear();
}

void ewol::widget::Windows::onChangeSize() {
	ewol::Widget::onChangeSize();
	if (m_subWidget != nullptr) {
		m_subWidget->calculateMinMaxSize();
		// TODO : do it better ... and manage gravity ...
		m_subWidget->setSize(m_size);
		m_subWidget->setOrigin(vec2(0.0f, 0.0f));
		m_subWidget->onChangeSize();
	}
	for (auto &it : m_popUpWidgetList) {
		if(it != nullptr) {
			it->calculateMinMaxSize();
			it->setSize(m_size);
			it->setOrigin(vec2(0.0f, 0.0f));
			it->onChangeSize();
		}
	}
}

ewol::WidgetShared ewol::widget::Windows::getWidgetAtPos(const vec2& _pos) {
	EWOL_VERBOSE("Get widget at pos : " << _pos);
	// calculate relative position
	vec2 relativePos = relativePosition(_pos);
	// event go directly on the pop-up
	if (m_popUpWidgetList.size() != 0) {
		return m_popUpWidgetList.back()->getWidgetAtPos(_pos);
	// otherwise in the normal windows
	} else if (m_subWidget != nullptr) {
		return m_subWidget->getWidgetAtPos(_pos);
	}
	// otherwise the event go to this widget ...
	return ememory::dynamicPointerCast<ewol::Widget>(sharedFromThis());
}

void ewol::widget::Windows::sysDraw() {
	EWOL_VERBOSE("Draw on " << m_size);
	// set the size of the open GL system
	gale::openGL::setViewPort(vec2(0,0), m_size);
	gale::openGL::disable(gale::openGL::flag_dither);
	//gale::openGL::disable(gale::openGL::flag_blend);
	gale::openGL::disable(gale::openGL::flag_stencilTest);
	gale::openGL::disable(gale::openGL::flag_alphaTest);
	gale::openGL::disable(gale::openGL::flag_fog);
	#if (    !defined(__TARGET_OS__Android) \
	      && !defined(__TARGET_OS__IOs) \
	      && !defined(__TARGET_OS__Web))
		glPixelZoom(1.0,1.0);
	#endif
	gale::openGL::disable(gale::openGL::flag_texture2D);
	gale::openGL::disable(gale::openGL::flag_depthTest);
	
	gale::openGL::enable(gale::openGL::flag_blend);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	// clear the matrix system :
	mat4 newOne;
	gale::openGL::setBasicMatrix(newOne);
	
	ewol::DrawProperty displayProp;
	displayProp.m_windowsSize = m_size;
	displayProp.m_origin.setValue(0,0);
	displayProp.m_size = m_size;
	systemDraw(displayProp);
	gale::openGL::disable(gale::openGL::flag_blend);
	return;
}

void ewol::widget::Windows::onRegenerateDisplay() {
	if (m_subWidget != nullptr) {
		m_subWidget->onRegenerateDisplay();
	}
	for (auto &it : m_popUpWidgetList) {
		if (it != nullptr) {
			it->onRegenerateDisplay();
		}
	}
}

//#define TEST_PERFO_WINDOWS

void ewol::widget::Windows::systemDraw(const ewol::DrawProperty& _displayProp) {
	ewol::Widget::systemDraw(_displayProp);
	#ifdef TEST_PERFO_WINDOWS
	int64_t ___startTime0 = ewol::getTime();
	#endif
	// clear the screen with transparency ...
	etk::Color<float> colorBg(0.5, 0.5, 0.5, 0.5);
	if (m_resourceColor != nullptr) {
		colorBg = m_resourceColor->get(m_colorBg);
	}
	gale::openGL::clearColor(colorBg);
	gale::openGL::clear(   uint32_t(gale::openGL::clearFlag_colorBuffer)
	                     | uint32_t(gale::openGL::clearFlag_depthBuffer));
	#ifdef TEST_PERFO_WINDOWS
	float ___localTime0 = (float)(ewol::getTime() - ___startTime0) / 1000.0f;
	EWOL_ERROR("      Windows000  : " << ___localTime0 << "ms ");
	int64_t ___startTime1 = ewol::getTime();
	#endif
	//EWOL_WARNING(" WINDOWS draw on " << m_currentDrawId);
	// first display the windows on the display
	if (m_subWidget != nullptr) {
		m_subWidget->systemDraw(_displayProp);
		//EWOL_DEBUG("Draw Windows");
	}
	#ifdef TEST_PERFO_WINDOWS
	float ___localTime1 = (float)(ewol::getTime() - ___startTime1) / 1000.0f;
	EWOL_ERROR("      Windows111  : " << ___localTime1 << "ms ");
	int64_t ___startTime2 = ewol::getTime();
	#endif
	// second display the pop-up
	for (auto &it : m_popUpWidgetList) {
		if (it != nullptr) {
			it->systemDraw(_displayProp);
			//EWOL_DEBUG("Draw Pop-up");
		}
	}
	#ifdef TEST_PERFO_WINDOWS
	float ___localTime2 = (float)(ewol::getTime() - ___startTime2) / 1000.0f;
	EWOL_ERROR("      Windows222  : " << ___localTime2 << "ms ");
	#endif
}

void ewol::widget::Windows::setSubWidget(ewol::WidgetShared _widget) {
	if (m_subWidget != nullptr) {
		EWOL_INFO("Remove current main windows Widget...");
		m_subWidget->removeParent();
		m_subWidget.reset();
	}
	if (_widget != nullptr) {
		m_subWidget = _widget;
		m_subWidget->setParent(sharedFromThis());
	}
	
	// Regenerate the size calculation :
	onChangeSize();
}

void ewol::widget::Windows::popUpWidgetPush(ewol::WidgetShared _widget) {
	if (_widget == nullptr) {
		// nothing to do an error appear :
		EWOL_ERROR("can not set widget pop-up (null pointer)");
		return;
	}
	m_popUpWidgetList.push_back(_widget);
	_widget->setParent(sharedFromThis());
	// force the focus on the basic widget ==> this remove many time the virual keyboard area
	_widget->keepFocus();
	// Regenerate the size calculation :
	onChangeSize();
	// TODO : it is dangerous to access directly to the system ...
	getContext().resetIOEvent();
}

void ewol::widget::Windows::popUpWidgetPop() {
	if (m_popUpWidgetList.size() == 0) {
		return;
	}
	m_popUpWidgetList.pop_back();
}

void ewol::widget::Windows::onChangePropertyColor() {
	m_resourceColor = ewol::resource::ColorFile::create(*propertyColorConfiguration);
	if (m_resourceColor != nullptr) {
		m_colorBg = m_resourceColor->request("background");
	}
}

void ewol::widget::Windows::onChangePropertyTitle() {
	ewol::Context& context = getContext();
	if (context.getWindows() == sharedFromThis()) {
		context.setTitle(*propertyTitle);
	} else {
		EWOL_INFO("Set title is delayed ...");
	}
}

void ewol::widget::Windows::requestDestroyFromChild(const ewol::ObjectShared& _child) {
	EWOL_VERBOSE("A child has been removed");
	auto it = m_popUpWidgetList.begin();
	while (it != m_popUpWidgetList.end()) {
		if (*it == _child) {
			EWOL_VERBOSE("    Find it ...");
			if (*it == nullptr) {
				m_popUpWidgetList.erase(it);
				it = m_popUpWidgetList.begin();
				continue;
			}
			(*it)->removeParent();
			(*it).reset();
			m_popUpWidgetList.erase(it);
			it = m_popUpWidgetList.begin();
			markToRedraw();
			continue;
		}
		++it;
	}
	if (m_subWidget == _child) {
		EWOL_VERBOSE("    Find it ... 2");
		if (m_subWidget == nullptr) {
			return;
		}
		m_subWidget->removeParent();
		m_subWidget.reset();
		markToRedraw();
	}
}

ewol::ObjectShared ewol::widget::Windows::getSubObjectNamed(const std::string& _objectName) {
	ewol::ObjectShared tmpObject = ewol::Widget::getSubObjectNamed(_objectName);
	if (tmpObject != nullptr) {
		return tmpObject;
	}
	// check direct subwidget
	if (m_subWidget != nullptr) {
		tmpObject = m_subWidget->getSubObjectNamed(_objectName);
		if (tmpObject != nullptr) {
			return tmpObject;
		}
	}
	// get all subwidget "pop-up"
	for (auto &it : m_popUpWidgetList) {
		if (it != nullptr) {
			tmpObject = it->getSubObjectNamed(_objectName);
			if (tmpObject != nullptr) {
				return tmpObject;
			}
		}
	}
	// not find ...
	return nullptr;
}


