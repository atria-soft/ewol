/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ewol/widget/Widget.hpp>
#include <ewol/object/Manager.hpp>
#include <ewol/widget/Manager.hpp>
#include <ewol/ewol.hpp>
#include <gale/renderer/openGL/openGL.hpp>
#include <gale/renderer/openGL/openGL-include.hpp>
#include <ewol/context/Context.hpp>
#include <etk/typeInfo.hpp>
ETK_DECLARE_TYPE(ewol::Widget);

ewol::Widget::Widget() :
  propertyMinSize(this, "min-size",
                        gale::Dimension(vec2(0,0),gale::distance::pixel),
                        "User minimum size",
                        &ewol::Widget::onChangePropertyMinSize),
  propertyMaxSize(this, "max-size",
                        gale::Dimension(vec2(ULTIMATE_MAX_SIZE,ULTIMATE_MAX_SIZE),gale::distance::pixel),
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
  m_allowRepeatKeyboardEvent(true),
  signalShortcut(this, "shortcut", ""),
  m_needRegenerateDisplay(true),
  m_grabCursor(false),
  m_cursorDisplay(gale::context::cursor::arrow){
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
	getWidgetManager().focusKeep(ememory::dynamicPointerCast<ewol::Widget>(sharedFromThis()));
}

void ewol::Widget::setOffset(const vec2& _newVal) {
	EWOL_INFO("Set offset: " << _newVal);
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
	//EWOL_INFO("[" << getId() << "] Draw : [" << propertyName << "] t=" << getObjectType() << " o=" << m_origin << "  s=" << m_size << " hide=" << propertyHide);
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
		int32_t tmpOriginX = etk::max(_displayProp.m_origin.x(), m_origin.x());
		int32_t tmppp1 = _displayProp.m_origin.x() + _displayProp.m_size.x();
		int32_t tmppp2 = m_origin.x() + m_size.x();
		int32_t tmpclipX = etk::min(tmppp1, tmppp2) - tmpOriginX;
		
		int32_t tmpOriginY = etk::max(_displayProp.m_origin.y(), m_origin.y());
		tmppp1 = _displayProp.m_origin.y() + _displayProp.m_size.y();
		tmppp2 = m_origin.y() + m_size.y();
		//int32_t tmpclipY = etk::min(tmppp1, tmppp2) - tmpOriginX;
		
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
	m_zoom = etk::avg(0.0000001f,_newVal,1000000.0f);
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
	propertyMinSize.set(gale::Dimension(vec2(0,0),gale::distance::pixel));
}

void ewol::Widget::checkMinSize() {
	vec2 pixelSize = propertyMinSize->getPixel();
	m_minSize.setX(etk::max(m_minSize.x(), pixelSize.x()));
	m_minSize.setY(etk::max(m_minSize.y(), pixelSize.y()));
}

void ewol::Widget::setNoMaxSize() {
	propertyMaxSize.set(gale::Dimension(vec2(ULTIMATE_MAX_SIZE,ULTIMATE_MAX_SIZE),gale::distance::pixel));
}

void ewol::Widget::checkMaxSize() {
	vec2 pixelSize = propertyMaxSize->getPixel();
	m_maxSize.setX(etk::min(m_maxSize.x(), pixelSize.x()));
	m_maxSize.setY(etk::min(m_maxSize.y(), pixelSize.y()));
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

void ewol::Widget::shortCutAdd(const etk::String& _descriptiveString, const etk::String& _message) {
	if (_descriptiveString.size() == 0) {
		EWOL_ERROR("try to add shortcut with no descriptive string ...");
		return;
	}
	EventShortCut tmpElement;
	if (_message.size() == 0) {
		tmpElement.message = _descriptiveString;
	} else {
		tmpElement.message = _message;
	}
	// parsing of the string:
	//"ctrl+shift+alt+meta+s"
	if(_descriptiveString.find("ctrl") != etk::String::npos) {
		tmpElement.specialKey.setCtrlLeft(true);
	}
	if(_descriptiveString.find("shift") != etk::String::npos) {
		tmpElement.specialKey.setShiftLeft(true);
	}
	if(_descriptiveString.find("alt") != etk::String::npos) {
		tmpElement.specialKey.setAltLeft(true);
	}
	if(_descriptiveString.find("meta") != etk::String::npos) {
		tmpElement.specialKey.setMetaLeft(true);
	}
	if(_descriptiveString.find("F12") != etk::String::npos) {
		tmpElement.keyboardMoveValue = gale::key::keyboard::f12;
	} else if(_descriptiveString.find("F11") != etk::String::npos) {
		tmpElement.keyboardMoveValue = gale::key::keyboard::f11;
	} else if(_descriptiveString.find("F10") != etk::String::npos) {
		tmpElement.keyboardMoveValue = gale::key::keyboard::f10;
	} else if(_descriptiveString.find("F9") != etk::String::npos) {
		tmpElement.keyboardMoveValue = gale::key::keyboard::f9;
	} else if(_descriptiveString.find("F8") != etk::String::npos) {
		tmpElement.keyboardMoveValue = gale::key::keyboard::f8;
	} else if(_descriptiveString.find("F7") != etk::String::npos) {
		tmpElement.keyboardMoveValue = gale::key::keyboard::f7;
	} else if(_descriptiveString.find("F6") != etk::String::npos) {
		tmpElement.keyboardMoveValue = gale::key::keyboard::f6;
	} else if(_descriptiveString.find("F5") != etk::String::npos) {
		tmpElement.keyboardMoveValue = gale::key::keyboard::f5;
	} else if(_descriptiveString.find("F4") != etk::String::npos) {
		tmpElement.keyboardMoveValue = gale::key::keyboard::f4;
	} else if(_descriptiveString.find("F3") != etk::String::npos) {
		tmpElement.keyboardMoveValue = gale::key::keyboard::f3;
	} else if(_descriptiveString.find("F2") != etk::String::npos) {
		tmpElement.keyboardMoveValue = gale::key::keyboard::f2;
	} else if(_descriptiveString.find("F1") != etk::String::npos) {
		tmpElement.keyboardMoveValue = gale::key::keyboard::f1;
	} else if(_descriptiveString.find("LEFT") != etk::String::npos) {
		tmpElement.keyboardMoveValue = gale::key::keyboard::left;
	} else if(_descriptiveString.find("RIGHT") != etk::String::npos) {
		tmpElement.keyboardMoveValue = gale::key::keyboard::right;
	} else if(_descriptiveString.find("UP") != etk::String::npos) {
		tmpElement.keyboardMoveValue = gale::key::keyboard::up;
	} else if(_descriptiveString.find("DOWN") != etk::String::npos) {
		tmpElement.keyboardMoveValue = gale::key::keyboard::down;
	} else if(_descriptiveString.find("PAGE_UP") != etk::String::npos) {
		tmpElement.keyboardMoveValue = gale::key::keyboard::pageUp;
	} else if(_descriptiveString.find("PAGE_DOWN") != etk::String::npos) {
		tmpElement.keyboardMoveValue = gale::key::keyboard::pageDown;
	} else if(_descriptiveString.find("START") != etk::String::npos) {
		tmpElement.keyboardMoveValue = gale::key::keyboard::start;
	} else if(_descriptiveString.find("END") != etk::String::npos) {
		tmpElement.keyboardMoveValue = gale::key::keyboard::end;
	} else if(_descriptiveString.find("PRINT") != etk::String::npos) {
		tmpElement.keyboardMoveValue = gale::key::keyboard::print;
	} else if(_descriptiveString.find("ARRET_DEFIL") != etk::String::npos) {
		tmpElement.keyboardMoveValue = gale::key::keyboard::stopDefil;
	} else if(_descriptiveString.find("WAIT") != etk::String::npos) {
		tmpElement.keyboardMoveValue = gale::key::keyboard::wait;
	} else if(_descriptiveString.find("INSERT") != etk::String::npos) {
		tmpElement.keyboardMoveValue = gale::key::keyboard::insert;
	} else if(_descriptiveString.find("CAPLOCK") != etk::String::npos) {
		tmpElement.keyboardMoveValue = gale::key::keyboard::capLock;
	} else if(_descriptiveString.find("CONTEXT_MENU") != etk::String::npos) {
		tmpElement.keyboardMoveValue = gale::key::keyboard::contextMenu;
	} else if(_descriptiveString.find("NUM_LOCK") != etk::String::npos) {
		tmpElement.keyboardMoveValue = gale::key::keyboard::numLock;
	} else {
		tmpElement.unicodeValue = _descriptiveString[_descriptiveString.size() -1];
	}
	// add it on the List ...
	m_localShortcut.pushBack(etk::move(tmpElement));
}

void ewol::Widget::shortCutRemove(const etk::String& _message) {
	auto it(m_localShortcut.begin());
	while(it != m_localShortcut.end()) {
		if (it->message != _message) {
			++it;
			continue;
		}
		m_localShortcut.erase(it);
		it = m_localShortcut.begin();
	}
}

void ewol::Widget::shortCutClean() {
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
	EWOL_VERBOSE("check shortcut...." << _special << " " << _unicodeValue << " " << _kbMove << " " << (_isDown?"DOWN":"UP") << " nb shortcut:" << m_localShortcut.size());
	// Remove the up event of the shortcut...
	if (_isDown == false) {
		for (int32_t iii=m_localShortcut.size()-1; iii >= 0; iii--) {
			if (m_localShortcut[iii].isActive == false) {
				continue;
			}
			if (    (    m_localShortcut[iii].keyboardMoveValue == gale::key::keyboard::unknow
			               && m_localShortcut[iii].unicodeValue == _unicodeValue)
			          || (    m_localShortcut[iii].keyboardMoveValue == _kbMove
			               && m_localShortcut[iii].unicodeValue == 0)
			        ) {
				// In this case we grap the event in case of an error can occured ...
				m_localShortcut[iii].isActive = false;
				EWOL_VERBOSE("detect up of a shortcut");
				return true;
			}
		}
	}
	//EWOL_INFO("Try to find generic shortcut ...");
	for (int32_t iii=m_localShortcut.size()-1; iii >= 0; iii--) {
		if (    m_localShortcut[iii].specialKey.getShift() == _special.getShift()
		     && m_localShortcut[iii].specialKey.getCtrl()  == _special.getCtrl()
		     && m_localShortcut[iii].specialKey.getAlt()   == _special.getAlt()
		     && m_localShortcut[iii].specialKey.getMeta()  == _special.getMeta()
		     && (    (    m_localShortcut[iii].keyboardMoveValue == gale::key::keyboard::unknow
		               && m_localShortcut[iii].unicodeValue == _unicodeValue)
		          || (    m_localShortcut[iii].keyboardMoveValue == _kbMove
		               && m_localShortcut[iii].unicodeValue == 0)
		        )
		   ) {
			if (_isDown == true) {
				m_localShortcut[iii].isActive = true;
				EWOL_VERBOSE("Generate shortCut: " << m_localShortcut[iii].message);
				signalShortcut.emit(m_localShortcut[iii].message);
			}
			return true;
		}
	}
	return false;
}


void ewol::Widget::grabCursor() {
	if (m_grabCursor == false) {
		getContext().inputEventGrabPointer(ememory::dynamicPointerCast<ewol::Widget>(sharedFromThis()));
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

bool ewol::Widget::loadXML(const exml::Element& _node) {
	ewol::Object::loadXML(_node);
	markToRedraw();
	return true;
}

bool ewol::Widget::systemEventEntry(ewol::event::EntrySystem& _event) {
	ewol::WidgetShared up = ememory::dynamicPointerCast<ewol::Widget>(m_parent.lock());
	if (up != null) {
		if (up->systemEventEntry(_event) == true) {
			return true;
		}
	}
	return onEventEntry(_event.m_event);
}

bool ewol::Widget::systemEventInput(ewol::event::InputSystem& _event) {
	ewol::WidgetShared up = ememory::dynamicPointerCast<ewol::Widget>(m_parent.lock());
	if (up != null) {
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
		propertyMaxSize.setDirect(gale::Dimension(vec2(ULTIMATE_MAX_SIZE,ULTIMATE_MAX_SIZE),gale::distance::pixel));
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
		propertyMinSize.setDirect(gale::Dimension(vec2(0,0),gale::distance::pixel));
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

void ewol::Widget::drawWidgetTree(int32_t _level) {
	etk::String space;
	for (int32_t iii=0; iii<_level; ++iii) {
		space += "    ";
	}
	EWOL_PRINT(space << "[" << getId() << "] name='" << propertyName << "' type=" << getObjectType() << " o=" << m_origin << "  s=" << m_size << " hide=" << propertyHide);
}