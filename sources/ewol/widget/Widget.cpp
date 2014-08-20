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
#include <ewol/openGL/openGL.h>
#include <ewol/context/Context.h>

#undef __class__
#define __class__ "DrawProperty"

std::ostream& ewol::operator <<(std::ostream& _os, const ewol::DrawProperty& _obj) {
	_os << "{ windowsSize=" << _obj.m_windowsSize << " start=" << _obj.m_origin << " stop=" << (_obj.m_origin+_obj.m_size) << "}";
	return _os;
}

void ewol::DrawProperty::limit(const vec2& _origin, const vec2& _size) {
	m_size += m_origin;
	m_origin.setMax(_origin);
	m_size.setMin(_origin+_size);
	m_size -= m_origin;
}

#undef __class__
#define __class__ "gravity"

std::string ewol::gravityToString(const enum ewol::gravity _obj) {
	switch(_obj) {
		case ewol::gravityCenter:
			return "center";
		case ewol::gravityTopLeft:
			return "top-left";
		case ewol::gravityTop:
			return "top";
		case ewol::gravityTopRight:
			return "top-right";
		case ewol::gravityRight:
			return "right";
		case ewol::gravityButtomRight:
			return "buttom-right";
		case ewol::gravityButtom:
			return "buttom";
		case ewol::gravityButtomLeft:
			return "buttom-left";
		case ewol::gravityLeft:
			return "left";
	}
	return "unknow";
}

enum ewol::gravity ewol::stringToGravity(const std::string& _obj) {
	if (_obj == "center") {
		return ewol::gravityCenter;
	} else if (_obj == "top-left") {
		return ewol::gravityTopLeft;
	} else if (_obj == "top") {
		return ewol::gravityTop;
	} else if (_obj == "top-right") {
		return ewol::gravityTopRight;
	} else if (_obj == "right") {
		return ewol::gravityRight;
	} else if (_obj == "buttom-right") {
		return ewol::gravityButtomRight;
	} else if (_obj == "buttom") {
		return ewol::gravityButtom;
	} else if (_obj == "buttom-left") {
		return ewol::gravityButtomLeft;
	} else if (_obj == "left") {
		return ewol::gravityLeft;
	}
	return ewol::gravityCenter;
}

std::ostream& ewol::operator <<(std::ostream& _os, const enum ewol::gravity _obj) {
	_os << ewol::gravityToString(_obj);
	return _os;
}

#undef __class__
#define __class__ "Widget"

ewol::Widget::Widget() :
  m_size(10,10),
  m_minSize(0,0),
  m_maxSize(vec2(ULTIMATE_MAX_SIZE,ULTIMATE_MAX_SIZE)),
  m_offset(0,0),
  m_zoom(1.0f),
  m_origin(0,0),
  m_userMinSize(*this, "min-size", ewol::Dimension(vec2(0,0),ewol::Dimension::Pixel), "User minimum size"),
  m_userMaxSize(*this, "max-size", ewol::Dimension(vec2(ULTIMATE_MAX_SIZE,ULTIMATE_MAX_SIZE),ewol::Dimension::Pixel), "User maximum size"),
  m_userExpand(*this, "expand", bvec2(false,false), "Request the widget Expand size wile space is available"),
  m_userFill(*this, "fill", bvec2(false,false), "Fill the widget available size"),
  m_hide(*this, "hide", false, "The widget start hided"),
  m_gravity(*this, "gravity", ewol::gravityButtomLeft, "Gravity orientation"),
  m_hasFocus(false),
  m_canFocus(*this, "focus", false, "enable the widget to have the focus capacity"), // TODO : je pense que c'est une erreur, c'st surement un event to get the cocus ...
  m_limitMouseEvent(3),
  m_allowRepeateKeyboardEvent(true),
  m_periodicCallDeltaTime(-1),
  m_periodicCallTime(0),
  m_needRegenerateDisplay(true),
  m_grabCursor(false),
  m_cursorDisplay(ewol::context::cursorArrow),
  signalAnnimationStart(*this, "annimation-start"),
  signalAnnimationRatio(*this, "annimation-ratio"),
  signalAnnimationStop(*this, "annimation-stop"),
  m_annimationMode(annimationModeDisable),
  m_annimationratio(0.0f),
  m_annimationTypeStart(*this, "annimation-start-type", 0, "Annimation type, when adding/show a widget"),
  m_annimationTimeStart(*this, "annimation-start-time", 0.1f, 0.0f, 200.0f, "Annimation time in second, when adding/show a widget"),
  m_annimationTypeStop(*this, "annimation-stop-type", 0, "Annimation type, when removing/hide a widget"),
  m_annimationTimeStop(*this, "annimation-stop-time", 0.1f, 0.0f, 200.0f, "Annimation time in second, when removing/hide a widget"){
	addObjectType("ewol::Widget");
	
	// TODO : Set a static interface for list ==> this methode create a multiple allocation
	m_gravity.add(ewol::gravityCenter, "center");
	m_gravity.add(ewol::gravityTopLeft, "top-left");
	m_gravity.add(ewol::gravityTop, "top");
	m_gravity.add(ewol::gravityTopRight, "top-right");
	m_gravity.add(ewol::gravityRight, "right");
	m_gravity.add(ewol::gravityButtomRight, "buttom-right");
	m_gravity.add(ewol::gravityButtom, "buttom");
	m_gravity.add(ewol::gravityButtomLeft, "buttom-left");
	m_gravity.add(ewol::gravityLeft, "left");
	m_annimationTypeStart.add(0, "none");
	m_annimationTypeStop.add(0, "none");
}

void ewol::Widget::init() {
	ewol::Object::init();
}

void ewol::Widget::init(const std::string& _name) {
	ewol::Object::init(_name);
}

ewol::Widget::~Widget() {
	// clean all the short-cut ...
	shortCutClean();
}

void ewol::Widget::calculateSize(const vec2& _available) {
	m_size = _available;
	m_size.setMax(m_minSize);
	markToRedraw();
}

bool ewol::Widget::setFocus() {
	if (m_canFocus == true) {
		if (m_hasFocus == false) {
			m_hasFocus = true;
			onGetFocus();
		}
		return true;
	}
	return false;
}

bool ewol::Widget::rmFocus() {
	if (m_canFocus == true) {
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
	if (true == m_hide){
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
	
	ewol::openGL::push();
	// set internal matrix system :
	ewol::openGL::setMatrix(tmpMat);
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
		ewol::openGL::setMatrix(tmpMat);
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
		ewol::openGL::setMatrix(tmpMat);
		//int64_t ___startTime = ewol::getTime();
		onDraw();
		//float ___localTime = (float)(ewol::getTime() - ___startTime) / 1000.0f;
		//EWOL_DEBUG("      Widget2  : " << ___localTime << "ms ");
	}
	#endif
	ewol::openGL::pop();
	return;
}

void ewol::Widget::periodicCallDisable() {
	m_periodicCallDeltaTime=0;
	m_periodicCallTime=-1;
	getWidgetManager().periodicCallRm(std::dynamic_pointer_cast<ewol::Widget>(shared_from_this()));
}

void ewol::Widget::periodicCallEnable(float _callInSecond) {
	if (_callInSecond < 0) {
		periodicCallDisable();
	} else {
		getWidgetManager().periodicCallAdd(std::dynamic_pointer_cast<ewol::Widget>(shared_from_this()));
		m_periodicCallDeltaTime = _callInSecond*1000000.0;
		m_periodicCallTime = ewol::getTime();
	}
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
	m_minSize = m_userMinSize->getPixel();
	//EWOL_ERROR("[" << getId() << "] convert in min size : " << m_userMinSize << " out=" << m_minSize);
	m_maxSize = m_userMaxSize->getPixel();
	markToRedraw();
}

vec2 ewol::Widget::getCalculateMinSize() {
	if (false == isHide()) {
		return m_minSize;
	}
	return vec2(0,0);
}

vec2 ewol::Widget::getCalculateMaxSize() {
	if (false == isHide()) {
		return m_maxSize;
	}
	return vec2(ULTIMATE_MAX_SIZE,ULTIMATE_MAX_SIZE);
}

void ewol::Widget::setNoMinSize() {
	m_userMinSize.set(ewol::Dimension(vec2(0,0),ewol::Dimension::Pixel));
}

void ewol::Widget::checkMinSize() {
	vec2 pixelSize = m_userMinSize->getPixel();
	m_minSize.setX(std::max(m_minSize.x(), pixelSize.x()));
	m_minSize.setY(std::max(m_minSize.y(), pixelSize.y()));
}

void ewol::Widget::setNoMaxSize() {
	m_userMaxSize.set(ewol::Dimension(vec2(ULTIMATE_MAX_SIZE,ULTIMATE_MAX_SIZE),ewol::Dimension::Pixel));
}

void ewol::Widget::checkMaxSize() {
	vec2 pixelSize = m_userMaxSize->getPixel();
	m_maxSize.setX(std::min(m_maxSize.x(), pixelSize.x()));
	m_maxSize.setY(std::min(m_maxSize.y(), pixelSize.y()));
}

vec2 ewol::Widget::getSize() {
	if (false == isHide()) {
		return m_size;
	}
	return vec2(0,0);
}

bvec2 ewol::Widget::canExpand() {
	if (false == isHide()) {
		return m_userExpand;
	}
	return bvec2(false,false);
}

const bvec2& ewol::Widget::canFill() {
	return m_userFill;
}

// ----------------------------------------------------------------------------------------------------------------
// -- Shortcut : management of the shortcut
// ----------------------------------------------------------------------------------------------------------------

void ewol::Widget::shortCutAdd(const char * _descriptiveString,
                               const char * _generateEventId,
                               std::string _data,
                               bool _broadcast) {
	if (    _descriptiveString == nullptr
	     || strlen(_descriptiveString) == 0) {
		EWOL_ERROR("try to add shortcut with no descriptive string ...");
		return;
	}
	EventShortCut* tmpElement = new EventShortCut();
	if (nullptr == tmpElement) {
		EWOL_ERROR("allocation error ... Memory error ...");
		return;
	}
	tmpElement->broadcastEvent = _broadcast;
	tmpElement->generateEventId = _generateEventId;
	tmpElement->eventData = _data;
	// parsing of the string :
	//"ctrl+shift+alt+meta+s"
	const char * tmp = strstr(_descriptiveString, "ctrl");
	if(nullptr != tmp) {
		tmpElement->specialKey.setCtrl(true);
	}
	tmp = strstr(_descriptiveString, "shift");
	if(nullptr != tmp) {
		tmpElement->specialKey.setShift(true);
	}
	tmp = strstr(_descriptiveString, "alt");
	if(nullptr != tmp) {
		tmpElement->specialKey.setAlt(true);
	}
	tmp = strstr(_descriptiveString, "meta");
	if(nullptr != tmp) {
		tmpElement->specialKey.setMeta(true);
	}
	if(nullptr != strstr(_descriptiveString, "F12") ) {
		tmpElement->keyboardMoveValue = ewol::key::keyboardF12;
	} else if(nullptr != strstr(_descriptiveString, "F11") ) {
		tmpElement->keyboardMoveValue = ewol::key::keyboardF11;
	} else if(nullptr != strstr(_descriptiveString, "F10") ) {
		tmpElement->keyboardMoveValue = ewol::key::keyboardF10;
	} else if(nullptr != strstr(_descriptiveString, "F9") ) {
		tmpElement->keyboardMoveValue = ewol::key::keyboardF9;
	} else if(nullptr != strstr(_descriptiveString, "F8") ) {
		tmpElement->keyboardMoveValue = ewol::key::keyboardF8;
	} else if(nullptr != strstr(_descriptiveString, "F7") ) {
		tmpElement->keyboardMoveValue = ewol::key::keyboardF7;
	} else if(nullptr != strstr(_descriptiveString, "F6") ) {
		tmpElement->keyboardMoveValue = ewol::key::keyboardF6;
	} else if(nullptr != strstr(_descriptiveString, "F5") ) {
		tmpElement->keyboardMoveValue = ewol::key::keyboardF5;
	} else if(nullptr != strstr(_descriptiveString, "F4") ) {
		tmpElement->keyboardMoveValue = ewol::key::keyboardF4;
	} else if(nullptr != strstr(_descriptiveString, "F3") ) {
		tmpElement->keyboardMoveValue = ewol::key::keyboardF3;
	} else if(nullptr != strstr(_descriptiveString, "F2") ) {
		tmpElement->keyboardMoveValue = ewol::key::keyboardF2;
	} else if(nullptr != strstr(_descriptiveString, "F1") ) {
		tmpElement->keyboardMoveValue = ewol::key::keyboardF1;
	} else if(nullptr != strstr(_descriptiveString, "LEFT") ) {
		tmpElement->keyboardMoveValue = ewol::key::keyboardLeft;
	} else if(nullptr != strstr(_descriptiveString, "RIGHT") ) {
		tmpElement->keyboardMoveValue = ewol::key::keyboardRight;
	} else if(nullptr != strstr(_descriptiveString, "UP") ) {
		tmpElement->keyboardMoveValue = ewol::key::keyboardUp;
	} else if(nullptr != strstr(_descriptiveString, "DOWN") ) {
		tmpElement->keyboardMoveValue = ewol::key::keyboardDown;
	} else if(nullptr != strstr(_descriptiveString, "PAGE_UP") ) {
		tmpElement->keyboardMoveValue = ewol::key::keyboardPageUp;
	} else if(nullptr != strstr(_descriptiveString, "PAGE_DOWN") ) {
		tmpElement->keyboardMoveValue = ewol::key::keyboardPageDown;
	} else if(nullptr != strstr(_descriptiveString, "START") ) {
		tmpElement->keyboardMoveValue = ewol::key::keyboardStart;
	} else if(nullptr != strstr(_descriptiveString, "END") ) {
		tmpElement->keyboardMoveValue = ewol::key::keyboardEnd;
	} else if(nullptr != strstr(_descriptiveString, "PRINT") ) {
		tmpElement->keyboardMoveValue = ewol::key::keyboardPrint;
	} else if(nullptr != strstr(_descriptiveString, "ARRET_DEFIL") ) {
		tmpElement->keyboardMoveValue = ewol::key::keyboardStopDefil;
	} else if(nullptr != strstr(_descriptiveString, "WAIT") ) {
		tmpElement->keyboardMoveValue = ewol::key::keyboardWait;
	} else if(nullptr != strstr(_descriptiveString, "INSERT") ) {
		tmpElement->keyboardMoveValue = ewol::key::keyboardInsert;
	} else if(nullptr != strstr(_descriptiveString, "CAPLOCK") ) {
		tmpElement->keyboardMoveValue = ewol::key::keyboardCapLock;
	} else if(nullptr != strstr(_descriptiveString, "CONTEXT_MENU") ) {
		tmpElement->keyboardMoveValue = ewol::key::keyboardContextMenu;
	} else if(nullptr != strstr(_descriptiveString, "NUM_LOCK") ) {
		tmpElement->keyboardMoveValue = ewol::key::keyboardNumLock;
	} else {
		tmpElement->unicodeValue = _descriptiveString[strlen(_descriptiveString) -1];
	}
	// add it on the List ...
	m_localShortcut.push_back(tmpElement);
}

void ewol::Widget::shortCutClean() {
	for (size_t iii=0; iii<m_localShortcut.size(); iii++) {
		if (nullptr != m_localShortcut[iii]) {
			delete(m_localShortcut[iii]);
			m_localShortcut[iii]=nullptr;
		}
	}
	m_localShortcut.clear();
}

bool ewol::Widget::onEventShortCut(ewol::key::Special& _special,
                                   char32_t _unicodeValue,
                                   enum ewol::key::keyboard _kbMove,
                                   bool _isDown) {
	if (_unicodeValue >= 'A' && _unicodeValue  <= 'Z') {
		_unicodeValue += 'a' - 'A';
	}
	//EWOL_INFO("Try to find generic shortcut ...");
	for(int32_t iii=m_localShortcut.size()-1; iii >= 0; iii--) {
		if(nullptr != m_localShortcut[iii]) {
			if(    m_localShortcut[iii]->specialKey.getShift() == _special.getShift()
			    && m_localShortcut[iii]->specialKey.getCtrl()  == _special.getCtrl()
			    && m_localShortcut[iii]->specialKey.getAlt()   == _special.getAlt()
			    && m_localShortcut[iii]->specialKey.getMeta()  == _special.getMeta()
			    && (    (    m_localShortcut[iii]->keyboardMoveValue == ewol::key::keyboardUnknow
			              && m_localShortcut[iii]->unicodeValue == _unicodeValue)
			         || (    m_localShortcut[iii]->keyboardMoveValue == _kbMove
			              && m_localShortcut[iii]->unicodeValue == 0)
			       ) ) {
				if (_isDown) {
					if (true == m_localShortcut[iii]->broadcastEvent) {
						// send message at all the widget (exepted this one)
						sendMultiCast(m_localShortcut[iii]->generateEventId, m_localShortcut[iii]->eventData);
					}
					// send message direct to the current widget (in every case, really useful for some generic windows shortcut)
					ewol::object::Message tmpMsg(shared_from_this(), m_localShortcut[iii]->generateEventId, m_localShortcut[iii]->eventData);
					onReceiveMessage(tmpMsg);
				} // no else
				return true;
			}
		}
	}
	return false;
}


void ewol::Widget::grabCursor() {
	if (false == m_grabCursor) {
		getContext().inputEventGrabPointer(std::dynamic_pointer_cast<ewol::Widget>(shared_from_this()));
		m_grabCursor = true;
	}
}

void ewol::Widget::unGrabCursor() {
	if (true == m_grabCursor) {
		getContext().inputEventUnGrabPointer();
		m_grabCursor = false;
	}
}

bool ewol::Widget::getGrabStatus() {
	return m_grabCursor;
}

void ewol::Widget::setCursor(enum ewol::context::cursorDisplay _newCursor) {
	EWOL_DEBUG("Change Cursor in " << _newCursor);
	m_cursorDisplay = _newCursor;
	getContext().setCursor(m_cursorDisplay);
}

enum ewol::context::cursorDisplay ewol::Widget::getCursor() {
	return m_cursorDisplay;
}

bool ewol::Widget::loadXML(exml::Element* _node) {
	// Call EObject basic parser
	ewol::Object::loadXML(_node); // note : load standard parameters (attribute in XML)
	markToRedraw();
	return true;
}

std::shared_ptr<ewol::Widget> ewol::Widget::getWidgetNamed(const std::string& _widgetName) {
	EWOL_VERBOSE("[" << getId() << "] {" << getObjectType() << "} compare : " << getName() << " == " << _widgetName );
	if (getName() == _widgetName) {
		return std::dynamic_pointer_cast<ewol::Widget>(shared_from_this());
	}
	return nullptr;
}

bool ewol::Widget::systemEventEntry(ewol::event::EntrySystem& _event) {
	std::shared_ptr<ewol::Widget> up = std::dynamic_pointer_cast<ewol::Widget>(m_parent.lock());
	if (up != nullptr) {
		if (up->systemEventEntry(_event) == true) {
			return true;
		}
	}
	return onEventEntry(_event.m_event);
}

bool ewol::Widget::systemEventInput(ewol::event::InputSystem& _event) {
	std::shared_ptr<ewol::Widget> up = std::dynamic_pointer_cast<ewol::Widget>(m_parent.lock());
	if (up != nullptr) {
		if (up->systemEventInput(_event) == true) {
			return true;
		}
	}
	return onEventInput(_event.m_event);
}

void ewol::Widget::onParameterChangeValue(const ewol::object::ParameterRef& _paramPointer) {
	ewol::Object::onParameterChangeValue(_paramPointer);
	if (_paramPointer == m_canFocus) {
		if (m_hasFocus == true) {
			rmFocus();
		}
	} else if (_paramPointer == m_gravity) {
		markToRedraw();
	} else if (_paramPointer == m_hide) {
		markToRedraw();
		requestUpdateSize();
	} else if (_paramPointer == m_userFill) {
		markToRedraw();
		requestUpdateSize();
	} else if (_paramPointer == m_userExpand) {
		requestUpdateSize();
		markToRedraw();
	} else if (_paramPointer == m_userMaxSize) {
		vec2 pixelMin = m_userMinSize->getPixel();
		vec2 pixelMax = m_userMaxSize->getPixel();
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
			m_userMaxSize = ewol::Dimension(vec2(ULTIMATE_MAX_SIZE,ULTIMATE_MAX_SIZE),ewol::Dimension::Pixel);
		}
		requestUpdateSize();
	} else if (_paramPointer == m_userMinSize) {
		vec2 pixelMin = m_userMinSize->getPixel();
		vec2 pixelMax = m_userMaxSize->getPixel();
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
			m_userMinSize = ewol::Dimension(vec2(0,0),ewol::Dimension::Pixel);
		}
		requestUpdateSize();
	} else if (_paramPointer == m_annimationTypeStart) {
		
	} else if (_paramPointer == m_annimationTimeStart) {
		
	} else if (_paramPointer == m_annimationTypeStop) {
		
	} else if (_paramPointer == m_annimationTimeStop) {
		
	}
}

void ewol::Widget::requestUpdateSize() {
	getContext().requestUpdateSize();
}

ewol::widget::Manager& ewol::Widget::getWidgetManager() {
	return getContext().getWidgetManager();
}

std::shared_ptr<ewol::widget::Windows> ewol::Widget::getWindows() {
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
	if (_mode==0) {
		m_annimationTypeStart.setString(_type);
	} else {
		m_annimationTypeStop.setString(_type);
	}
}

void ewol::Widget::setAnnimationTime(enum ewol::Widget::annimationMode _mode, float _time) {
	if (_mode==0) {
		m_annimationTimeStart.set(_time);
	} else {
		m_annimationTimeStop.set(_time);
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