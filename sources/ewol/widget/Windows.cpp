/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <etk/types.h>
#include <etk/UString.h>
#include <ewol/ewol.h>
#include <ewol/renderer/openGL.h>
#include <ewol/renderer/eContext.h>
#include <ewol/widget/Widget.h>
#include <ewol/widget/Windows.h>
#include <ewol/widget/WidgetManager.h>
#include <ewol/widget/meta/StdPopUp.h>

#undef __class__
#define __class__ "Windows"

//list of local events : 
extern const char * const ewolEventWindowsHideKeyboard   = "ewol Windows hideKeyboard";


ewol::Windows::Windows(void) :
  m_backgroundColor(0.750, 0.750, 0.750, 0.5) {
	setCanHaveFocus(true);
	m_subWidget = NULL;
	setDecorationDisable();
	//KeyboardShow(KEYBOARD_MODE_CODE);
}

ewol::Windows::~Windows(void) {
	if (NULL != m_subWidget) {
		delete(m_subWidget);
		m_subWidget=NULL;
	}
	for(int32_t iii=0; iii<m_popUpWidgetList.size(); iii++) {
		if (NULL != m_popUpWidgetList[iii]) {
			delete(m_popUpWidgetList[iii]);
			m_popUpWidgetList[iii]=NULL;
		}
	}
	m_popUpWidgetList.clear();
}

void ewol::Windows::calculateSize(const vec2& _availlable) {
	//EWOL_DEBUG(" _availlable : " << _availlable);
	m_size = _availlable;
	if (NULL != m_subWidget) {
		m_subWidget->calculateMinMaxSize();
		// TODO : Check if min size is possible ...
		// TODO : Herited from MinSize .. and expand ???
		m_subWidget->calculateSize(m_size);
	}
	for(int32_t iii=0; iii<m_popUpWidgetList.size(); iii++) {
		if (NULL != m_popUpWidgetList[iii]) {
			m_popUpWidgetList[iii]->calculateMinMaxSize();
			m_popUpWidgetList[iii]->calculateSize(m_size);
		}
	}
}

ewol::Widget * ewol::Windows::getWidgetAtPos(const vec2& _pos) {
	// calculate relative position
	vec2 relativePos = relativePosition(_pos);
	// event go directly on the pop-up
	if (0 < m_popUpWidgetList.size()) {
		if (NULL == m_popUpWidgetList[m_popUpWidgetList.size()-1]) {
			m_popUpWidgetList.popBack();
		} else {
			return m_popUpWidgetList[m_popUpWidgetList.size()-1]->getWidgetAtPos(_pos);
		}
	// otherwise in the normal windows
	} else if (NULL != m_subWidget) {
		return m_subWidget->getWidgetAtPos(_pos);
	}
	// otherwise the event go to this widget ...
	return this;
}

void ewol::Windows::sysDraw(void) {
	//EWOL_DEBUG("Drow on (" << m_size.x << "," << m_size.y << ")");
	// set the size of the open GL system
	glViewport(0,0,m_size.x(),m_size.y());
	
	ewol::openGL::disable(ewol::openGL::FLAG_DITHER);
	//ewol::openGL::disable(ewol::openGL::FLAG_BLEND);
	ewol::openGL::disable(ewol::openGL::FLAG_STENCIL_TEST);
	ewol::openGL::disable(ewol::openGL::FLAG_ALPHA_TEST);
	ewol::openGL::disable(ewol::openGL::FLAG_FOG);
	#ifndef __TARGET_OS__Android
		glPixelZoom(1.0,1.0);
	#endif
	ewol::openGL::disable(ewol::openGL::FLAG_TEXTURE_2D);
	ewol::openGL::disable(ewol::openGL::FLAG_DEPTH_TEST);
	
	ewol::openGL::enable(ewol::openGL::FLAG_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	// clear the matrix system :
	mat4 newOne;
	ewol::openGL::setBasicMatrix(newOne);
	
	ewol::DrawProperty displayProp;
	displayProp.m_windowsSize = m_size;
	displayProp.m_origin.setValue(0,0);
	displayProp.m_size = m_size;
	systemDraw(displayProp);
	ewol::openGL::disable(ewol::openGL::FLAG_BLEND);
	return;
}

void ewol::Windows::onRegenerateDisplay(void) {
	if (NULL != m_subWidget) {
		m_subWidget->onRegenerateDisplay();
	}
	for(int32_t iii=0; iii<m_popUpWidgetList.size(); iii++) {
		if (NULL != m_popUpWidgetList[iii]) {
			m_popUpWidgetList[iii]->onRegenerateDisplay();
		}
	}
}

//#define TEST_PERFO_WINDOWS

void ewol::Windows::systemDraw(const ewol::DrawProperty& _displayProp) {
	ewol::Widget::systemDraw(_displayProp);
	#ifdef TEST_PERFO_WINDOWS
	int64_t ___startTime0 = ewol::getTime();
	#endif
	
	// clear the screen with transparency ...
	glClearColor(m_backgroundColor.r(),
	             m_backgroundColor.g(),
	             m_backgroundColor.b(),
	             m_backgroundColor.a());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	#ifdef TEST_PERFO_WINDOWS
	float ___localTime0 = (float)(ewol::getTime() - ___startTime0) / 1000.0f;
	EWOL_ERROR("      Windows000  : " << ___localTime0 << "ms ");
	
	int64_t ___startTime1 = ewol::getTime();
	#endif
	//EWOL_WARNING(" WINDOWS draw on " << m_currentDrawId);
	// first display the windows on the display
	if (NULL != m_subWidget) {
		m_subWidget->systemDraw(_displayProp);
		//EWOL_DEBUG("Draw Windows");
	}
	#ifdef TEST_PERFO_WINDOWS
	float ___localTime1 = (float)(ewol::getTime() - ___startTime1) / 1000.0f;
	EWOL_ERROR("      Windows111  : " << ___localTime1 << "ms ");
	
	int64_t ___startTime2 = ewol::getTime();
	#endif
	// second display the pop-up
	for(int32_t iii=0; iii<m_popUpWidgetList.size(); iii++) {
		if (NULL != m_popUpWidgetList[iii]) {
			m_popUpWidgetList[iii]->systemDraw(_displayProp);
			//EWOL_DEBUG("Draw Pop-up");
		}
	}
	#ifdef TEST_PERFO_WINDOWS
	float ___localTime2 = (float)(ewol::getTime() - ___startTime2) / 1000.0f;
	EWOL_ERROR("      Windows222  : " << ___localTime2 << "ms ");
	#endif
}

void ewol::Windows::setSubWidget(ewol::Widget* _widget) {
	if (NULL != m_subWidget) {
		EWOL_INFO("Remove current main windows Widget...");
		delete(m_subWidget);
		m_subWidget = NULL;
	}
	m_subWidget = _widget;
	// Regenerate the size calculation :
	calculateSize(m_size);
}

void ewol::Windows::popUpWidgetPush(ewol::Widget* _widget) {
	m_popUpWidgetList.push_back(_widget);
	// Regenerate the size calculation :
	calculateSize(m_size);
	// TODO : it is dansgerous to access directly to the system ...
	getContext().resetIOEvent();
}

void ewol::Windows::onObjectRemove(ewol::EObject* _removeObject) {
	// First step call parrent : 
	ewol::Widget::onObjectRemove(_removeObject);
	// second step find if in all the elements ...
	
	if (m_subWidget == _removeObject) {
		EWOL_DEBUG("Remove main element of the windows  == > destroyed object");
		m_subWidget = NULL;
	}
	for(int32_t iii=m_popUpWidgetList.size()-1; iii >= 0; --iii) {
		if(m_popUpWidgetList[iii] == _removeObject) {
			EWOL_DEBUG("Remove Pop-up [" << iii << "] element of the windows  == > destroyed object");
			m_popUpWidgetList[iii] = NULL;
			m_popUpWidgetList.erase(iii);
		}
	}
}

void ewol::Windows::setBackgroundColor(const etk::Color<float>& _color) {
	if (m_backgroundColor != _color) {
		m_backgroundColor = _color;
		markToRedraw();
	}
}

void ewol::Windows::setTitle(const etk::UString& _title) {
	// TODO : remove this ...
	etk::UString title = _title;
	getContext().setTitle(title);
}


void ewol::Windows::createPopUpMessage(enum popUpMessageType _type, const etk::UString& _message)
{
	widget::StdPopUp* tmpPopUp = new widget::StdPopUp();
	if (tmpPopUp == NULL) {
		EWOL_ERROR("Can not create a simple pop-up");
		return;
	}
	switch(_type) {
		case messageTypeInfo:
			tmpPopUp->setTitle("<bold>Info</bold>");
			break;
		case messageTypeWarning:
			tmpPopUp->setTitle("<bold><font color=\"orange\">Warning</font></bold>");
			break;
		case messageTypeError:
			tmpPopUp->setTitle("<bold><font color=\"red\">Error</font></bold>");
			break;
		case messageTypeCritical:
			tmpPopUp->setTitle("<bold><font colorBg=\"red\">Critical</font></bold>");
			break;
	}
	tmpPopUp->setComment(_message);
	tmpPopUp->addButton("close", true);
	tmpPopUp->setRemoveOnExternClick(true);
	popUpWidgetPush(tmpPopUp);
}
