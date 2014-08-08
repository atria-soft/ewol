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

const char* const ewol::Widget::configFill = "fill";
const char* const ewol::Widget::configExpand = "expand";
const char* const ewol::Widget::configHide = "hide";
const char* const ewol::Widget::configFocus = "focus";
const char* const ewol::Widget::configMinSize = "min-size";
const char* const ewol::Widget::configMaxSize = "max-size";
const char* const ewol::Widget::configGravity = "gravity";

// configuration :
const char* const ewol::Widget::configAnnimationAddType = "annimation-start-type";
const char* const ewol::Widget::configAnnimationAddTime = "annimation-start-time";
const char* const ewol::Widget::configAnnimationRemoveType = "annimation-remove-type";
const char* const ewol::Widget::configAnnimationRemoveTime = "annimation-remove-time";
// event generated :
const char* const ewol::Widget::eventAnnimationStart = "annimation-start";
const char* const ewol::Widget::eventAnnimationRatio = "annimation-ratio";
const char* const ewol::Widget::eventAnnimationStop = "annimation-stop";

ewol::Widget::Widget() :
  m_size(10,10),
  m_minSize(0,0),
  m_maxSize(vec2(ULTIMATE_MAX_SIZE,ULTIMATE_MAX_SIZE)),
  m_offset(0,0),
  m_zoom(1.0f),
  m_origin(0,0),
  m_userMinSize(vec2(0,0),ewol::Dimension::Pixel),
  m_userMaxSize(vec2(ULTIMATE_MAX_SIZE,ULTIMATE_MAX_SIZE),ewol::Dimension::Pixel),
  m_userExpand(false,false),
  m_userFill(false,false),
  m_hide(false),
  m_gravity(ewol::gravityButtomLeft),
  m_hasFocus(false),
  m_canFocus(false),
  m_limitMouseEvent(3),
  m_allowRepeateKeyboardEvent(true),
  m_periodicCallDeltaTime(-1),
  m_periodicCallTime(0),
  m_needRegenerateDisplay(true),
  m_grabCursor(false),
  m_cursorDisplay(ewol::context::cursorArrow),
  m_annimationMode(annimationModeDisable),
  m_annimationratio(0.0f) {
	m_annimationType[0] = nullptr;
	m_annimationType[1] = nullptr;
	m_annimationTime[0] = 0.1f; // annimation will be 100ms at the first state
	m_annimationTime[1] = 0.1f; // annimation will be 100ms at the first state
	addObjectType("ewol::Widget");
	// set all the config in the list :
	registerConfig(ewol::Widget::configFill, "bvec2", nullptr, "Fill the widget available size");
	registerConfig(ewol::Widget::configExpand, "bvec2", nullptr, "Request the widget Expand size wile space is available");
	registerConfig(ewol::Widget::configHide, "bool", nullptr, "The widget start hided");
	registerConfig(ewol::Widget::configFocus, "bool", nullptr, "The widget request focus");
	registerConfig(ewol::Widget::configMinSize, "dimension", nullptr, "User minimum size");
	registerConfig(ewol::Widget::configMaxSize, "dimension", nullptr, "User maximum size");
	registerConfig(ewol::Widget::configGravity, "list", "center;top-left;top;top-right;right;buttom-right;buttom;buttom-left;left", "User maximum size");
	registerConfig(ewol::Widget::configAnnimationAddType, "list", nullptr /* no control */, "Annimation type, when adding/show a widget");
	registerConfig(ewol::Widget::configAnnimationAddTime, "float", nullptr /* no control */, "Annimation time in second, when adding/show a widget");
	registerConfig(ewol::Widget::configAnnimationRemoveType, "list", nullptr /* no control */, "Annimation type, when removing/hide a widget");
	registerConfig(ewol::Widget::configAnnimationRemoveTime, "float", nullptr /* no control */, "Annimation time in second, when removing/hide a widget");
	addEventId(eventAnnimationStart);
	addEventId(eventAnnimationRatio);
	addEventId(eventAnnimationStop);
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

void ewol::Widget::hide() {
	if (m_hide == false) {
		EWOL_WARNING("HIDE widget: '" << getName() << "'");
		m_hide = true;
		markToRedraw();
		requestUpdateSize();
	}
}

void ewol::Widget::show() {
	if (m_hide == true) {
		EWOL_WARNING("SHOW widget: '" << getName() << "'");
		m_hide = false;
		markToRedraw();
		requestUpdateSize();
	}
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

void ewol::Widget::setCanHaveFocus(bool _canFocusState) {
	if (m_canFocus != _canFocusState) {
		m_canFocus = _canFocusState;
		if (m_hasFocus == true) {
			rmFocus();
		}
	}
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
	m_minSize = m_userMinSize.getPixel();
	//EWOL_ERROR("[" << getId() << "] convert in min size : " << m_userMinSize << " out=" << m_minSize);
	m_maxSize = m_userMaxSize.getPixel();
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

void ewol::Widget::setMinSize(const ewol::Dimension& _size) {
	vec2 pixelMin = _size.getPixel();
	vec2 pixelMax = m_userMaxSize.getPixel();
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
		return;
	}
	m_userMinSize = _size;
	requestUpdateSize();
}

void ewol::Widget::setNoMinSize() {
	m_userMinSize.set(vec2(0,0),ewol::Dimension::Pixel);
}

void ewol::Widget::checkMinSize() {
	vec2 pixelSize = m_userMinSize.getPixel();
	m_minSize.setX(std::max(m_minSize.x(), pixelSize.x()));
	m_minSize.setY(std::max(m_minSize.y(), pixelSize.y()));
}

void ewol::Widget::setMaxSize(const ewol::Dimension& _size) {
	vec2 pixelMin = m_userMinSize.getPixel();
	vec2 pixelMax = _size.getPixel();
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
		return;
	}
	m_userMaxSize = _size;
	requestUpdateSize();
}

void ewol::Widget::setNoMaxSize() {
	m_userMaxSize.set(vec2(ULTIMATE_MAX_SIZE,ULTIMATE_MAX_SIZE),ewol::Dimension::Pixel);
}

void ewol::Widget::checkMaxSize() {
	vec2 pixelSize = m_userMaxSize.getPixel();
	m_maxSize.setX(std::min(m_maxSize.x(), pixelSize.x()));
	m_maxSize.setY(std::min(m_maxSize.y(), pixelSize.y()));
}

vec2 ewol::Widget::getSize() {
	if (false == isHide()) {
		return m_size;
	}
	return vec2(0,0);
}

void ewol::Widget::setExpand(const bvec2& _newExpand) {
	if(    m_userExpand.x() != _newExpand.x()
	    || m_userExpand.y() != _newExpand.y()) {
		m_userExpand = _newExpand;
		requestUpdateSize();
		markToRedraw();
	}
}

bvec2 ewol::Widget::canExpand() {
	if (false == isHide()) {
		return m_userExpand;
	}
	return bvec2(false,false);
}

void ewol::Widget::setFill(const bvec2& _newFill) {
	if(    m_userFill.x() != _newFill.x()
	    || m_userFill.y() != _newFill.y()) {
		m_userFill = _newFill;
		requestUpdateSize();
		markToRedraw();
	}
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

void ewol::Widget::setGravity(enum gravity _gravity) {
	m_gravity = _gravity;
	markToRedraw();
}

bool ewol::Widget::onSetConfig(const ewol::object::Config& _conf) {
	if (true == ewol::Object::onSetConfig(_conf)) {
		return true;
	}
	if (_conf.getConfig() == ewol::Widget::configFill) {
		setFill(_conf.getData());
		return true;
	}
	if (_conf.getConfig() == ewol::Widget::configExpand) {
		setExpand(_conf.getData());
		return true;
	}
	if (_conf.getConfig() == ewol::Widget::configHide) {
		if(true == std::stob(_conf.getData())) {
			hide();
		} else {
			show();
		}
		return true;
	}
	if (_conf.getConfig() == ewol::Widget::configFocus) {
		if(true == std::stob(_conf.getData())) {
			keepFocus();
		} else {
			//nothing to do ...
		}
		return true;
	}
	if (_conf.getConfig() == ewol::Widget::configMinSize) {
		m_userMinSize = _conf.getData();
		return true;
	}
	if (_conf.getConfig() == ewol::Widget::configMaxSize) {
		m_userMaxSize = _conf.getData();
		return true;
	}
	if (_conf.getConfig() == ewol::Widget::configGravity) {
		m_gravity = stringToGravity(_conf.getData());
		return true;
	}
	if (_conf.getConfig() == ewol::Widget::configAnnimationAddType) {
		setAnnimationType(ewol::Widget::annimationModeEnableAdd, _conf.getData());
		return true;
	}
	if (_conf.getConfig() == ewol::Widget::configAnnimationAddTime) {
		setAnnimationTime(ewol::Widget::annimationModeEnableAdd, std::stof(_conf.getData()));
		return true;
	}
	if (_conf.getConfig() == ewol::Widget::configAnnimationRemoveType) {
		setAnnimationType(ewol::Widget::annimationModeEnableRemove, _conf.getData());
		return true;
	}
	if (_conf.getConfig() == ewol::Widget::configAnnimationRemoveTime) {
		setAnnimationTime(ewol::Widget::annimationModeEnableRemove, std::stof(_conf.getData()));
		return true;
	}
	return false;
}

bool ewol::Widget::onGetConfig(const char* _config, std::string& _result) const {
	if (true == ewol::Object::onGetConfig(_config, _result)) {
		return true;
	}
	if (_config == ewol::Widget::configFill) {
		_result = m_userFill;
		return true;
	}
	if (_config == ewol::Widget::configExpand) {
		_result = m_userExpand;
		return true;
	}
	if (_config == ewol::Widget::configHide) {
		_result = std::to_string(m_hide);
		return true;
	}
	if (_config == ewol::Widget::configMinSize) {
		_result = m_userMinSize;
		return true;
	}
	if (_config == ewol::Widget::configMaxSize) {
		_result = m_userMaxSize;
		return true;
	}
	if (_config == ewol::Widget::configGravity) {
		_result = gravityToString(m_gravity);
		return true;
	}
	if (_config == ewol::Widget::configAnnimationAddType) {
		const char* type = m_annimationType[ewol::Widget::annimationModeEnableAdd];
		if (type == nullptr) {
			_result = "";
		} else {
			_result = type;
		}
		return true;
	}
	if (_config == ewol::Widget::configAnnimationAddTime) {
		_result = std::to_string(m_annimationType[ewol::Widget::annimationModeEnableAdd]);
		return true;
	}
	if (_config == ewol::Widget::configAnnimationRemoveType) {
		const char* type = m_annimationType[ewol::Widget::annimationModeEnableRemove];
		if (type == nullptr) {
			_result = "";
		} else {
			_result = type;
		}
		return true;
	}
	if (_config == ewol::Widget::configAnnimationRemoveTime) {
		_result = std::to_string(m_annimationType[ewol::Widget::annimationModeEnableRemove]);
		return true;
	}
	return false;
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
	for (size_t iii = 0; iii < m_annimationList[_mode].size(); ++iii) {
		if (m_annimationList[_mode][iii] == _type) {
			return;
		}
	}
	m_annimationList[_mode].push_back(_type);
}

void ewol::Widget::setAnnimationType(enum ewol::Widget::annimationMode _mode, const std::string& _type) {
	if (_mode == ewol::Widget::annimationModeDisable) {
		EWOL_CRITICAL("Not suported mode ==> only for internal properties");
		return;
	}
	for (size_t iii = 0; iii < m_annimationList[_mode].size(); ++iii) {
		if (compare_no_case(m_annimationList[_mode][iii], _type) == true) {
			m_annimationType[_mode] = m_annimationList[_mode][iii];
			return;
		}
	}
	EWOL_ERROR("Can not find annimation type='" << _type << "'");
}

void ewol::Widget::setAnnimationTime(enum ewol::Widget::annimationMode _mode, float _time) {
	if (_mode == ewol::Widget::annimationModeDisable) {
		EWOL_CRITICAL("Not suported mode ==> only for internal properties");
		return;
	}
	m_annimationTime[_mode] = _time;
	if (m_annimationTime[_mode] > 36000.0f) {
		EWOL_WARNING("Are you kidding ? Your annimation time : " << m_annimationTime[_mode] << " s is greater than 10 hours");
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