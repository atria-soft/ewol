/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#include <ewol/widget/Widget.h>
#include <ewol/object/Manager.h>
#include <ewol/widget/Manager.h>
#include <ewol/ewol.h>
#include <gale/renderer/openGL/openGL.h>
#include <gale/renderer/openGL/openGL-include.h>
#include <ewol/context/Context.h>

#undef __class__
#define __class__ "Widget"

ewol::Widget::Widget() :
  propertyMinSize(this, "min-size",
                        gale::Dimension(vec2(0,0),gale::Dimension::Pixel),
                        "User minimum size",
                        &ewol::Widget::onChangePropertyMinSize),
  propertyMaxSize(this, "max-size",
                        gale::Dimension(vec2(ULTIMATE_MAX_SIZE,ULTIMATE_MAX_SIZE),gale::Dimension::Pixel),
                        "User maximum size",
                        &ewol::Widget::onChangePropertyMaxSize),
  propertyExpand(this, "expand",
                       bvec2(false,false),
                       "Request the widget Expand size wile space is available",
                       &ewol::Widget::onChangePropertyExpand),
  propertyFill(this, "fill",
                     bvec2(true,true),
                     "Fill the widget available size",
                     &ewol::Widget::onChangePropertyFill),
  propertyHide(this, "hide",
                     false,
                     "The widget start hided",
                     &ewol::Widget::onChangePropertyHide),
  propertyGravity(this, "gravity",
                        ewol::gravity_buttomLeft,
                        "Gravity orientation",
                        &ewol::Widget::onChangePropertyGravity),
  // TODO : je pense que c'est une erreur, c'est surement un event to get the cocus ...
  propertyCanFocus(this, "focus",
                         false,
                         "enable the widget to have the focus capacity",
                         &ewol::Widget::onChangePropertyCanFocus),
  m_size(10,10),
  m_minSize(0,0),
  m_maxSize(vec2(ULTIMATE_MAX_SIZE,ULTIMATE_MAX_SIZE)),
  m_offset(0,0),
  m_zoom(1.0f),
  m_origin(0,0),
  m_hasFocus(false),
  m_limitMouseEvent(3),
  m_allowRepeateKeyboardEvent(true),
  signalShortcut(this, "shortcut", ""),
  m_needRegenerateDisplay(true),
  m_grabCursor(false),
  m_cursorDisplay(gale::context::cursor_arrow),
  signalAnnimationStart(this, "annimation-start", ""),
  signalAnnimationRatio(this, "annimation-ratio", ""),
  signalAnnimationStop(this, "annimation-stop", ""),
  m_annimationMode(annimationModeDisable),
  m_annimationratio(0.0f),
  propertyAnnimationTypeStart(this, "annimation-start-type", 0, "Annimation type, when adding/show a widget"),
  propertyAnnimationTimeStart(this, "annimation-start-time", 0.1f, 0.0f, 200.0f, "Annimation time in second, when adding/show a widget"),
  propertyAnnimationTypeStop(this, "annimation-stop-type", 0, "Annimation type, when removing/hide a widget"),
  propertyAnnimationTimeStop(this, "annimation-stop-time", 0.1f, 0.0f, 200.0f, "Annimation time in second, when removing/hide a widget"){
	addObjectType("ewol::Widget");
	
	// TODO : Set a static interface for list ==> this methode create a multiple allocation
	propertyGravity.add(ewol::gravity_center, "center");
	propertyGravity.add(ewol::gravity_topLeft, "top-left");
	propertyGravity.add(ewol::gravity_top, "top");
	propertyGravity.add(ewol::gravity_topRight, "top-right");
	propertyGravity.add(ewol::gravity_right, "right");
	propertyGravity.add(ewol::gravity_buttomRight, "buttom-right");
	propertyGravity.add(ewol::gravity_buttom, "buttom");
	propertyGravity.add(ewol::gravity_buttomLeft, "buttom-left");
	propertyGravity.add(ewol::gravity_left, "left");
	propertyAnnimationTypeStart.add(0, "none");
	propertyAnnimationTypeStop.add(0, "none");
}

ewol::Widget::~Widget() {
	// clean all the short-cut ...
	shortCutClean();
}

void ewol::Widget::onChangeSize() {
	EWOL_VERBOSE("[" << getId() << "] {" << getObjectType() << "} update size : " << m_size);
	markToRedraw();
}

bool ewol::Widget::setFocus() {
	EWOL_VERBOSE("set focus (start) *propertyCanFocus=" << *propertyCanFocus << " m_hasFocus=" << m_hasFocus);
	if (*propertyCanFocus == true) {
		if (m_hasFocus == false) {
			m_hasFocus = true;
			onGetFocus();
		}
		EWOL_VERBOSE("set focus (stop) ret true");
		return true;
	}
	EWOL_VERBOSE("set focus (stop) ret false");
	return false;
}

bool ewol::Widget::rmFocus() {
	if (*propertyCanFocus == true) {
		if (m_hasFocus == true) {
			m_hasFocus = false;
			onLostFocus();
		}
		return true;
	}
	return false;
}

void ewol::Widget::keepFocus() {
	getWidgetManager().focusKeep(std::dynamic_pointer_cast<ewol::Widget>(shared_from_this()));
}

void ewol::Widget::setOffset(const vec2& _newVal) {
	if (m_offset != _newVal) {
		m_offset = _newVal;
		markToRedraw();
	}
}

/*
                                                              /--> _displayProp.m_windowsSize
      *------------------------------------------------------*
      |                                                      |
      |                                              m_size  |
      |                                                 /    |
      |                        *-----------------------*     |
      |                        '                       '     |
      |                        '   _displayProp.m_size '     |
      |              Viewport  '          /            '     |
      |              o---------'---------o             '     |
      |              |         '         |             '     |
      |              |         '         |             '     |
      |              |         '         |             '     |
      |              |         '         |             '     |
      |              |         *-----------------------*     |
      |              |        /          |                   |
      |              |   m_offset        |                   |
      |              |                   |                   |
      |              o-------------------o                   |
      |             /                                        |
      |  _displayProp.m_origin                               |
      |                                                      |
      *------------------------------------------------------*
     /
   (0,0)
*/
void ewol::Widget::systemDraw(const ewol::DrawProperty& _displayProp) {
	if (*propertyHide == true){
		// widget is hidden ...
		return;
	}
	vec2 displayOrigin = m_origin + m_offset;
	
	// check if the element is displayable in the windows : 
	if(    _displayProp.m_windowsSize.x() < m_origin.x()
	    || _displayProp.m_windowsSize.y() < m_origin.y() ) {
		// out of the windows  == > nothing to display ...
		return;
	}
	
	ewol::DrawProperty tmpSize = _displayProp;
	tmpSize.limit(m_origin, m_size);
	if (tmpSize.m_size.x() <= 0 || tmpSize.m_size.y() <= 0) {
		return;
	}
	
	glViewport( (int32_t)tmpSize.m_origin.x(),
	            (int32_t)tmpSize.m_origin.y(),
	            (int32_t)tmpSize.m_size.x(),
	            (int32_t)tmpSize.m_size.y());
	// special case, when origin < display origin, we need to cut the display :
	ivec2 downOffset = m_origin - tmpSize.m_origin;
	downOffset.setMin(ivec2(0,0));
	
	mat4 tmpTranslate = etk::matTranslate(vec3ClipInt32(vec3(-tmpSize.m_size.x()/2+m_offset.x() + downOffset.x(),
	                                                         -tmpSize.m_size.y()/2+m_offset.y() + downOffset.y(),
	                                                         -1.0f)));
	mat4 tmpScale = etk::matScale(vec3(m_zoom, m_zoom, 1.0f));
	mat4 tmpProjection = etk::matOrtho((int32_t)(-tmpSize.m_size.x())>>1,
	                                   (int32_t)( tmpSize.m_size.x())>>1,
	                                   (int32_t)(-tmpSize.m_size.y())>>1,
	                                   (int32_t)( tmpSize.m_size.y())>>1,
	                                   (int32_t)(-1),
	                                   (int32_t)( 1));
	mat4 tmpMat = tmpProjection * tmpScale * tmpTranslate;
	
	gale::openGL::push();
	// set internal matrix system :
	gale::openGL::setMatrix(tmpMat);
	//int64_t ___startTime = ewol::getTime();
	onDraw();
	
	#ifdef old_PLOP
	if(    (_displayProp.m_origin.x() > m_origin.x())
	    || (_displayProp.m_origin.x() + _displayProp.m_size.x() < m_size.x() + m_origin.x()) ) {
		// here we invert the reference of the standard openGl view because the reference in the common display is Top left and not buttom left
		int32_t tmpOriginX = std::max(_displayProp.m_origin.x(), m_origin.x());
		int32_t tmppp1 = _displayProp.m_origin.x() + _displayProp.m_size.x();
		int32_t tmppp2 = m_origin.x() + m_size.x();
		int32_t tmpclipX = std::min(tmppp1, tmppp2) - tmpOriginX;
		
		int32_t tmpOriginY = std::max(_displayProp.m_origin.y(), m_origin.y());
		tmppp1 = _displayProp.m_origin.y() + _displayProp.m_size.y();
		tmppp2 = m_origin.y() + m_size.y();
		//int32_t tmpclipY = std::min(tmppp1, tmppp2) - tmpOriginX;
		
		glViewport( tmpOriginX,
		            tmpOriginY,
		            tmpclipX,
		            m_size.y());
		mat4 tmpTranslate = etk::matTranslate(vec3((float)(-tmpclipX/2 - (tmpOriginX-m_origin.x())), (float)(-m_size.y()/2.0), -1.0f));
		mat4 tmpScale = etk::matScale(vec3(m_zoom, m_zoom, 1));
		mat4 tmpProjection = etk::matOrtho(-tmpclipX/2, tmpclipX/2, -m_size.y()/2, m_size.y()/2, -1, 1);
		mat4 tmpMat = tmpProjection * tmpScale * tmpTranslate;
		// set internal matrix system :
		gale::openGL::setMatrix(tmpMat);
		//int64_t ___startTime = ewol::getTime();
		onDraw();
		//float ___localTime = (float)(ewol::getTime() - ___startTime) / 1000.0f;
		//EWOL_DEBUG("      Widget1  : " << ___localTime << "ms ");
	} else {
		EWOL_DEBUG("rasta..");
		glViewport( m_origin.x(),
		            m_origin.y(),
		            m_size.x(),
		            m_size.y());
		mat4 tmpTranslate = etk::matTranslate(vec3(-m_size.x()/2, -m_size.y()/2, -1.0f));
		mat4 tmpScale = etk::matScale(vec3(m_zoom, m_zoom, 1.0f));
		mat4 tmpProjection = etk::matOrtho(-m_size.x()/2, m_size.x()/2, -m_size.y()/2, m_size.y()/2, -1, 1);
		mat4 tmpMat = tmpProjection * tmpScale * tmpTranslate;
		// set internal matrix system :
		gale::openGL::setMatrix(tmpMat);
		//int64_t ___startTime = ewol::getTime();
		onDraw();
		//float ___localTime = (float)(ewol::getTime() - ___startTime) / 1000.0f;
		//EWOL_DEBUG("      Widget2  : " << ___localTime << "ms ");
	}
	#endif
	gale::openGL::pop();
	return;
}

void ewol::Widget::periodicCallDisable() {
	EWOL_VERBOSE("Perodic call disable " << propertyName);
	m_periodicCallHandle.disconnect();
}

void ewol::Widget::periodicCallEnable() {
	if (m_periodicCallHandle.isConnected() == true) {
		EWOL_VERBOSE("Perodic call enable " << propertyName << " ==> rejected");
		return;
	} else {
		EWOL_VERBOSE("Perodic call enable " << propertyName);
	}
	m_periodicCallHandle = getObjectManager().periodicCall.connect(this, &ewol::Widget::periodicCall);
}

void ewol::Widget::markToRedraw() {
	if (m_needRegenerateDisplay == true) {
		return;
	}
	m_needRegenerateDisplay = true;
	getWidgetManager().markDrawingIsNeeded();
}

void ewol::Widget::setZoom(float _newVal) {
	if (m_zoom == _newVal) {
		return;
	}
	m_zoom = std::avg(0.0000001f,_newVal,1000000.0f);
	markToRedraw();
}

float ewol::Widget::getZoom() {
	return m_zoom;
}

void ewol::Widget::setOrigin(const vec2& _pos) {
	#if DEBUG_LEVEL > 2
		if(    m_origin.x() < -5000
		    || m_origin.y() < -5000) {
			EWOL_WARNING("[" << getId() << "] set origin < 5000 : " << m_origin);
		}
	#endif
	m_origin = _pos;
}

vec2 ewol::Widget::getOrigin() {
	return m_origin;
}

vec2 ewol::Widget::relativePosition(const vec2& _pos) {
	return _pos - m_origin;
}

void ewol::Widget::calculateMinMaxSize() {
	m_minSize = propertyMinSize->getPixel();
	//EWOL_ERROR("[" << getId() << "] convert in min size : " << propertyMinSize << " out=" << m_minSize);
	m_maxSize = propertyMaxSize->getPixel();
	markToRedraw();
}

vec2 ewol::Widget::getCalculateMinSize() {
	if (*propertyHide == false) {
		return m_minSize;
	}
	return vec2(0,0);
}

vec2 ewol::Widget::getCalculateMaxSize() {
	if (*propertyHide == false) {
		return m_maxSize;
	}
	return vec2(ULTIMATE_MAX_SIZE,ULTIMATE_MAX_SIZE);
}

void ewol::Widget::setNoMinSize() {
	propertyMinSize.set(gale::Dimension(vec2(0,0),gale::Dimension::Pixel));
}

void ewol::Widget::checkMinSize() {
	vec2 pixelSize = propertyMinSize->getPixel();
	m_minSize.setX(std::max(m_minSize.x(), pixelSize.x()));
	m_minSize.setY(std::max(m_minSize.y(), pixelSize.y()));
}

void ewol::Widget::setNoMaxSize() {
	propertyMaxSize.set(gale::Dimension(vec2(ULTIMATE_MAX_SIZE,ULTIMATE_MAX_SIZE),gale::Dimension::Pixel));
}

void ewol::Widget::checkMaxSize() {
	vec2 pixelSize = propertyMaxSize->getPixel();
	m_maxSize.setX(std::min(m_maxSize.x(), pixelSize.x()));
	m_maxSize.setY(std::min(m_maxSize.y(), pixelSize.y()));
}

vec2 ewol::Widget::getSize() {
	if (*propertyHide == false) {
		return m_size;
	}
	return vec2(0,0);
}

bvec2 ewol::Widget::canExpand() {
	if (*propertyHide == false) {
		return *propertyExpand;
	}
	return bvec2(false,false);
}

const bvec2& ewol::Widget::canFill() {
	return *propertyFill;
}

// ----------------------------------------------------------------------------------------------------------------
// -- Shortcut : management of the shortcut
// ----------------------------------------------------------------------------------------------------------------

void ewol::Widget::shortCutAdd(const std::string& _descriptiveString, const std::string& _message) {
	if (_descriptiveString.size() == 0) {
		EWOL_ERROR("try to add shortcut with no descriptive string ...");
		return;
	}
	EventShortCut* tmpElement = new EventShortCut();
	if (nullptr == tmpElement) {
		EWOL_ERROR("allocation error ... Memory error ...");
		return;
	}
	if (_message.size() == 0) {
		tmpElement->message = _descriptiveString;
	} else {
		tmpElement->message = _message;
	}
	// parsing of the string :
	//"ctrl+shift+alt+meta+s"
	if(_descriptiveString.find("ctrl") != std::string::npos) {
		tmpElement->specialKey.setCtrl(true);
	}
	if(_descriptiveString.find("shift") != std::string::npos) {
		tmpElement->specialKey.setShift(true);
	}
	if(_descriptiveString.find("alt") != std::string::npos) {
		tmpElement->specialKey.setAlt(true);
	}
	if(_descriptiveString.find("meta") != std::string::npos) {
		tmpElement->specialKey.setMeta(true);
	}
	if(_descriptiveString.find("F12") != std::string::npos) {
		tmpElement->keyboardMoveValue = gale::key::keyboard_f12;
	} else if(_descriptiveString.find("F11") != std::string::npos) {
		tmpElement->keyboardMoveValue = gale::key::keyboard_f11;
	} else if(_descriptiveString.find("F10") != std::string::npos) {
		tmpElement->keyboardMoveValue = gale::key::keyboard_f10;
	} else if(_descriptiveString.find("F9") != std::string::npos) {
		tmpElement->keyboardMoveValue = gale::key::keyboard_f9;
	} else if(_descriptiveString.find("F8") != std::string::npos) {
		tmpElement->keyboardMoveValue = gale::key::keyboard_f8;
	} else if(_descriptiveString.find("F7") != std::string::npos) {
		tmpElement->keyboardMoveValue = gale::key::keyboard_f7;
	} else if(_descriptiveString.find("F6") != std::string::npos) {
		tmpElement->keyboardMoveValue = gale::key::keyboard_f6;
	} else if(_descriptiveString.find("F5") != std::string::npos) {
		tmpElement->keyboardMoveValue = gale::key::keyboard_f5;
	} else if(_descriptiveString.find("F4") != std::string::npos) {
		tmpElement->keyboardMoveValue = gale::key::keyboard_f4;
	} else if(_descriptiveString.find("F3") != std::string::npos) {
		tmpElement->keyboardMoveValue = gale::key::keyboard_f3;
	} else if(_descriptiveString.find("F2") != std::string::npos) {
		tmpElement->keyboardMoveValue = gale::key::keyboard_f2;
	} else if(_descriptiveString.find("F1") != std::string::npos) {
		tmpElement->keyboardMoveValue = gale::key::keyboard_f1;
	} else if(_descriptiveString.find("LEFT") != std::string::npos) {
		tmpElement->keyboardMoveValue = gale::key::keyboard_left;
	} else if(_descriptiveString.find("RIGHT") != std::string::npos) {
		tmpElement->keyboardMoveValue = gale::key::keyboard_right;
	} else if(_descriptiveString.find("UP") != std::string::npos) {
		tmpElement->keyboardMoveValue = gale::key::keyboard_up;
	} else if(_descriptiveString.find("DOWN") != std::string::npos) {
		tmpElement->keyboardMoveValue = gale::key::keyboard_down;
	} else if(_descriptiveString.find("PAGE_UP") != std::string::npos) {
		tmpElement->keyboardMoveValue = gale::key::keyboard_pageUp;
	} else if(_descriptiveString.find("PAGE_DOWN") != std::string::npos) {
		tmpElement->keyboardMoveValue = gale::key::keyboard_pageDown;
	} else if(_descriptiveString.find("START") != std::string::npos) {
		tmpElement->keyboardMoveValue = gale::key::keyboard_start;
	} else if(_descriptiveString.find("END") != std::string::npos) {
		tmpElement->keyboardMoveValue = gale::key::keyboard_end;
	} else if(_descriptiveString.find("PRINT") != std::string::npos) {
		tmpElement->keyboardMoveValue = gale::key::keyboard_print;
	} else if(_descriptiveString.find("ARRET_DEFIL") != std::string::npos) {
		tmpElement->keyboardMoveValue = gale::key::keyboard_stopDefil;
	} else if(_descriptiveString.find("WAIT") != std::string::npos) {
		tmpElement->keyboardMoveValue = gale::key::keyboard_wait;
	} else if(_descriptiveString.find("INSERT") != std::string::npos) {
		tmpElement->keyboardMoveValue = gale::key::keyboard_insert;
	} else if(_descriptiveString.find("CAPLOCK") != std::string::npos) {
		tmpElement->keyboardMoveValue = gale::key::keyboard_capLock;
	} else if(_descriptiveString.find("CONTEXT_MENU") != std::string::npos) {
		tmpElement->keyboardMoveValue = gale::key::keyboard_contextMenu;
	} else if(_descriptiveString.find("NUM_LOCK") != std::string::npos) {
		tmpElement->keyboardMoveValue = gale::key::keyboard_numLock;
	} else {
		tmpElement->unicodeValue = _descriptiveString[_descriptiveString.size() -1];
	}
	// add it on the List ...
	m_localShortcut.push_back(tmpElement);
}

void ewol::Widget::shortCutRemove(const std::string& _message) {
	auto it(m_localShortcut.begin());
	while(it != m_localShortcut.end()) {
		if (    *it != nullptr
		     && (*it)->message != _message) {
			++it;
			continue;
		}
		delete(*it);
		*it = nullptr;
		m_localShortcut.erase(it);
		it = m_localShortcut.begin();
	}
}

void ewol::Widget::shortCutClean() {
	for (auto &it : m_localShortcut) {
		delete(it);
		it = nullptr;
	}
	m_localShortcut.clear();
}

bool ewol::Widget::onEventShortCut(const gale::key::Special& _special,
                                   char32_t _unicodeValue,
                                   enum gale::key::keyboard _kbMove,
                                   bool _isDown) {
	if (    _unicodeValue >= 'A'
	     && _unicodeValue <= 'Z') {
		_unicodeValue += 'a' - 'A';
	}
	//EWOL_INFO("Try to find generic shortcut ...");
	for (int32_t iii=m_localShortcut.size()-1; iii >= 0; iii--) {
		if (m_localShortcut[iii] != nullptr) {
			if (    m_localShortcut[iii]->specialKey.getShift() == _special.getShift()
			     && m_localShortcut[iii]->specialKey.getCtrl()  == _special.getCtrl()
			     && m_localShortcut[iii]->specialKey.getAlt()   == _special.getAlt()
			     && m_localShortcut[iii]->specialKey.getMeta()  == _special.getMeta()
			     && (    (    m_localShortcut[iii]->keyboardMoveValue == gale::key::keyboard_unknow
			               && m_localShortcut[iii]->unicodeValue == _unicodeValue)
			          || (    m_localShortcut[iii]->keyboardMoveValue == _kbMove
			               && m_localShortcut[iii]->unicodeValue == 0)
			        )
			   ) {
				if (_isDown) {
					signalShortcut.emit(m_localShortcut[iii]->message);
				}
				return true;
			}
		}
	}
	return false;
}


void ewol::Widget::grabCursor() {
	if (m_grabCursor == false) {
		getContext().inputEventGrabPointer(std::dynamic_pointer_cast<ewol::Widget>(shared_from_this()));
		m_grabCursor = true;
	}
}

void ewol::Widget::unGrabCursor() {
	if (m_grabCursor == true) {
		getContext().inputEventUnGrabPointer();
		m_grabCursor = false;
	}
}

bool ewol::Widget::getGrabStatus() {
	return m_grabCursor;
}

void ewol::Widget::setCursor(enum gale::context::cursor _newCursor) {
	EWOL_DEBUG("Change Cursor in " << _newCursor);
	m_cursorDisplay = _newCursor;
	getContext().setCursor(m_cursorDisplay);
}

enum gale::context::cursor ewol::Widget::getCursor() {
	return m_cursorDisplay;
}

bool ewol::Widget::loadXML(const std::shared_ptr<const exml::Element>& _node) {
	ewol::Object::loadXML(_node);
	markToRedraw();
	return true;
}

bool ewol::Widget::systemEventEntry(ewol::event::EntrySystem& _event) {
	ewol::WidgetShared up = std::dynamic_pointer_cast<ewol::Widget>(m_parent.lock());
	if (up != nullptr) {
		if (up->systemEventEntry(_event) == true) {
			return true;
		}
	}
	return onEventEntry(_event.m_event);
}

bool ewol::Widget::systemEventInput(ewol::event::InputSystem& _event) {
	ewol::WidgetShared up = std::dynamic_pointer_cast<ewol::Widget>(m_parent.lock());
	if (up != nullptr) {
		if (up->systemEventInput(_event) == true) {
			return true;
		}
	}
	return onEventInput(_event.m_event);
}

void ewol::Widget::onChangePropertyCanFocus() {
	if (m_hasFocus == true) {
		rmFocus();
	}
}

void ewol::Widget::onChangePropertyGravity() {
	markToRedraw();
	requestUpdateSize();
}

void ewol::Widget::onChangePropertyHide() {
	markToRedraw();
	requestUpdateSize();
}

void ewol::Widget::onChangePropertyFill() {
	markToRedraw();
	requestUpdateSize();
}

void ewol::Widget::onChangePropertyExpand() {
	requestUpdateSize();
	markToRedraw();
}

void ewol::Widget::onChangePropertyMaxSize() {
	vec2 pixelMin = propertyMinSize->getPixel();
	vec2 pixelMax = propertyMaxSize->getPixel();
	// check minimum & maximum compatibility :
	bool error=false;
	if (pixelMin.x()>pixelMax.x()) {
		error=true;
	}
	if (pixelMin.y()>pixelMax.y()) {
		error=true;
	}
	if (error == true) {
		EWOL_ERROR("Can not set a 'min size' > 'max size' reset to maximum ...");
		propertyMaxSize.setDirect(gale::Dimension(vec2(ULTIMATE_MAX_SIZE,ULTIMATE_MAX_SIZE),gale::Dimension::Pixel));
	}
	requestUpdateSize();
}

void ewol::Widget::onChangePropertyMinSize() {
	vec2 pixelMin = propertyMinSize->getPixel();
	vec2 pixelMax = propertyMaxSize->getPixel();
	// check minimum & maximum compatibility :
	bool error=false;
	if (pixelMin.x()>pixelMax.x()) {
		error=true;
	}
	if (pixelMin.y()>pixelMax.y()) {
		error=true;
	}
	if (error == true) {
		EWOL_ERROR("Can not set a 'min size' > 'max size' set nothing ...");
		propertyMinSize.setDirect(gale::Dimension(vec2(0,0),gale::Dimension::Pixel));
	}
	requestUpdateSize();
}

void ewol::Widget::requestUpdateSize() {
	getContext().requestUpdateSize();
}

ewol::widget::Manager& ewol::Widget::getWidgetManager() {
	return getContext().getWidgetManager();
}

ewol::widget::WindowsShared ewol::Widget::getWindows() {
	return getContext().getWindows();
}

void ewol::Widget::showKeyboard() {
	getContext().keyboardShow();
}

void ewol::Widget::hideKeyboard() {
	getContext().keyboardHide();
}

void ewol::Widget::addAnnimationType(enum ewol::Widget::annimationMode _mode, const char* _type) {
	if (_mode == ewol::Widget::annimationModeDisable) {
		EWOL_CRITICAL("Not suported mode ==> only for internal properties");
		return;
	}
	/*
	for (size_t iii = 0; iii < m_annimationList[_mode].size(); ++iii) {
		if (m_annimationList[_mode][iii] == _type) {
			return;
		}
	}
	m_annimationList[_mode].push_back(_type);
	*/
}

void ewol::Widget::setAnnimationType(enum ewol::Widget::annimationMode _mode, const std::string& _type) {
	if (_mode == 0) {
		propertyAnnimationTypeStart.setString(_type);
	} else {
		propertyAnnimationTypeStop.setString(_type);
	}
}

void ewol::Widget::setAnnimationTime(enum ewol::Widget::annimationMode _mode, float _time) {
	if (_mode == 0) {
		propertyAnnimationTimeStart.set(_time);
	} else {
		propertyAnnimationTimeStop.set(_time);
	}
}

bool ewol::Widget::startAnnimation(enum ewol::Widget::annimationMode _mode) {
	if (_mode == ewol::Widget::annimationModeDisable) {
		EWOL_CRITICAL("Not suported mode ==> only for internal properties");
		return false;
	}
	m_annimationMode = _mode;
	return onStartAnnimation(_mode);
}

bool ewol::Widget::stopAnnimation() {
	m_annimationMode = ewol::Widget::annimationModeDisable;
	onStopAnnimation();
	return true; // ???
}