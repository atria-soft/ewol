/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/widget/Widget.h>
#include <ewol/eObject/EObjectManager.h>
#include <ewol/widget/WidgetManager.h>
#include <ewol/ewol.h>
#include <ewol/renderer/openGL.h>
#include <ewol/renderer/os/eSystem.h>
#include <ewol/renderer/os/gui.h>

#undef __class__
#define __class__ "DrawProperty"

etk::CCout& ewol::operator <<(etk::CCout& _os, const ewol::DrawProperty& _obj)
{
	_os << "{ windowsSize=" << _obj.m_windowsSize << " start=" << _obj.m_origin << " stop=" << (_obj.m_origin+_obj.m_size) << "}";
	return _os;
}

void ewol::DrawProperty::Limit(const vec2& _origin, const vec2& _size)
{
	m_size += m_origin;
	m_origin.setMax(_origin);
	m_size.setMin(_origin+_size);
	m_size -= m_origin;
}

#undef __class__
#define __class__ "gravity"


etk::UString ewol::GravityToString(const ewol::gravity_te _obj)
{
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

ewol::gravity_te ewol::StringToGravity(const etk::UString& _obj)
{
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

etk::CCout& ewol::operator <<(etk::CCout& _os, const ewol::gravity_te _obj)
{
	_os << ewol::GravityToString(_obj);
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

ewol::Widget::Widget(void) :
	m_up(NULL),
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
	m_needRegenerateDisplay(true),
	m_grabCursor(false),
	m_cursorDisplay(ewol::cursorArrow)
{
	// set all the config in the list :
	RegisterConfig(ewol::Widget::configFill, "bvec2", NULL, "Fill the widget availlable size");
	RegisterConfig(ewol::Widget::configExpand, "bvec2", NULL, "Request the widget Expand size wile space is availlable");
	RegisterConfig(ewol::Widget::configHide, "bool", NULL, "The widget start hided");
	RegisterConfig(ewol::Widget::configFocus, "bool", NULL, "The widget request focus");
	RegisterConfig(ewol::Widget::configMinSize, "dimension", NULL, "User minimum size");
	RegisterConfig(ewol::Widget::configMaxSize, "dimension", NULL, "User maximum size");
	RegisterConfig(ewol::Widget::configGravity, "list", "center;top-left;top;top-right;right;buttom-right;buttom;buttom-left;left", "User maximum size");
}


ewol::Widget::~Widget(void)
{
	// Remove his own focus...
	ewol::widgetManager::Rm(this);
	// clean all the short-cut ...
	ShortCutClean();
}

void ewol::Widget::SetUpperWidget(ewol::Widget* _upper)
{
	if (NULL == _upper) {
		//just remove father :
		m_up = NULL;
		return;
	}
	if (NULL != m_up) {
		EWOL_WARNING("[" << GetId() << "] Replace upper widget of this one ...");
	}
	m_up = _upper;
}

void ewol::Widget::OnObjectRemove(ewol::EObject* _removeObject)
{
	if (_removeObject == m_up) {
		EWOL_WARNING("[" << GetId() << "] Remove upper widget befor removing this widget ...");
		m_up = NULL;
	}
}

void ewol::Widget::Hide(void)
{
	m_hide = true;
	MarkToRedraw();
	ewol::RequestUpdateSize();
}


void ewol::Widget::Show(void)
{
	m_hide = false;
	MarkToRedraw();
	ewol::RequestUpdateSize();
}


void ewol::Widget::CalculateSize(const vec2& _availlable)
{
	m_size = _availlable;
	m_size.setMax(m_minSize);
	MarkToRedraw();
}


bool ewol::Widget::SetFocus(void)
{
	if (true == m_canFocus) {
		m_hasFocus = true;
		OnGetFocus();
		return true;
	}
	return false;
}


bool ewol::Widget::RmFocus(void)
{
	if (true == m_canFocus) {
		m_hasFocus = false;
		OnLostFocus();
		return true;
	}
	return false;
}


void ewol::Widget::SetCanHaveFocus(bool _canFocusState)
{
	m_canFocus = _canFocusState;
	if (true == m_hasFocus) {
		(void)RmFocus();
	}
}


void ewol::Widget::KeepFocus(void)
{
	ewol::widgetManager::FocusKeep(this);
}

void ewol::Widget::SetOffset(const vec2& _newVal)
{
	if (m_offset != _newVal) {
		m_offset = _newVal;
		MarkToRedraw();
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
void ewol::Widget::SystemDraw(const DrawProperty& _displayProp)
{
	if (true==m_hide){
		// widget is hidden ...
		return;
	}
	vec2 displayOrigin = m_origin + m_offset;
	
	// check if the element is displayable in the windows : 
	if(    _displayProp.m_windowsSize.x() < m_origin.x()
	    || _displayProp.m_windowsSize.y() < m_origin.y() ) {
		// out of the windows ==> nothing to display ...
		return;
	}
	
	DrawProperty tmpSize = _displayProp;
	tmpSize.Limit(m_origin, m_size);
	if (tmpSize.m_size.x() <= 0 || tmpSize.m_size.y() <= 0) {
		return;
	}
	
	glViewport( tmpSize.m_origin.x(),
	            tmpSize.m_origin.y(),
	            tmpSize.m_size.x(),
	            tmpSize.m_size.y());
	// special case, when origin < display origin, we need to cut the display :
	ivec2 downOffset = m_origin - tmpSize.m_origin;
	downOffset.setMin(ivec2(0,0));
	
	mat4 tmpTranslate = etk::matTranslate(vec3(-tmpSize.m_size.x()/2+m_offset.x() + downOffset.x(),
	                                           -tmpSize.m_size.y()/2+m_offset.y() + downOffset.y(),
	                                           -1.0f));
	mat4 tmpScale = etk::matScale(vec3(m_zoom, m_zoom, 1.0f));
	mat4 tmpProjection = etk::matOrtho(-tmpSize.m_size.x()/2,
	                                    tmpSize.m_size.x()/2,
	                                   -tmpSize.m_size.y()/2,
	                                    tmpSize.m_size.y()/2,
	                                   -1,
	                                    1);
	mat4 tmpMat = tmpProjection * tmpScale * tmpTranslate;

	ewol::openGL::Push();
	// set internal matrix system :
	ewol::openGL::SetMatrix(tmpMat);
	//int64_t ___startTime = ewol::GetTime();
	OnDraw();
	
	#ifdef old_PLOP
	if(    (_displayProp.m_origin.x() > m_origin.x())
	    || (_displayProp.m_origin.x() + _displayewol::Widget::Prop.m_size.x() < m_size.x() + m_origin.x()) ) {
		EWOL_CRITICAL("plop");
		// here we invert the reference of the standard OpenGl view because the reference in the common display is Top left and not buttom left
		int32_t tmpOriginX = etk_max(_displayProp.m_origin.x(), m_origin.x());
		int32_t tmppp1 = _displayProp.m_origin.x() + _displayProp.m_size.x();
		int32_t tmppp2 = m_origin.x() + m_size.x();
		int32_t tmpclipX = etk_min(tmppp1, tmppp2) - tmpOriginX;
		
		int32_t tmpOriginY = etk_max(_displayProp.m_origin.y(), m_origin.y());
		tmppp1 = _displayProp.m_origin.y() + _displayProp.m_size.y();
		tmppp2 = m_origin.y() + m_size.y();
		//int32_t tmpclipY = etk_min(tmppp1, tmppp2) - tmpOriginX;
		
		glViewport( tmpOriginX,
		            tmpOriginY,
		            tmpclipX,
		            m_size.y());
		mat4 tmpTranslate = etk::matTranslate(vec3((float)(-tmpclipX/2 - (tmpOriginX-m_origin.x())), (float)(-m_size.y()/2.0), -1.0f));
		mat4 tmpScale = etk::matScale(vec3(m_zoom, m_zoom, 1));
		mat4 tmpProjection = etk::matOrtho(-tmpclipX/2, tmpclipX/2, -m_size.y()/2, m_size.y()/2, -1, 1);
		mat4 tmpMat = tmpProjection * tmpScale * tmpTranslate;
		// set internal matrix system :
		ewol::openGL::SetMatrix(tmpMat);
		// Call the widget drawing methode
		_displayProp.m_origin.setValue(tmpOriginX, tmpOriginY);
		_displayProp.m_size.setValue(tmpclipX, m_size.y());
		//int64_t ___startTime = ewol::GetTime();
		OnDraw(_displayProp);
		//float ___localTime = (float)(ewol::GetTime() - ___startTime) / 1000.0f;
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
		ewol::openGL::SetMatrix(tmpMat);
		// Call the widget drawing methode
		_displayProp.m_origin = m_origin;
		_displayProp.m_size = m_size;
		//int64_t ___startTime = ewol::GetTime();
		OnDraw(_displayProp);
		//float ___localTime = (float)(ewol::GetTime() - ___startTime) / 1000.0f;
		//EWOL_DEBUG("      Widget2  : " << ___localTime << "ms ");
	}
	#endif
	ewol::openGL::Pop();
	return;
}


void ewol::Widget::PeriodicCallSet(bool _statusToSet)
{
	if (true == _statusToSet) {
		ewol::widgetManager::PeriodicCallAdd(this);
	} else {
		ewol::widgetManager::PeriodicCallRm(this);
	}
}


void ewol::Widget::MarkToRedraw(void)
{
	m_needRegenerateDisplay = true;
	ewol::widgetManager::MarkDrawingIsNeeded();
}


void ewol::Widget::SetZoom(float _newVal)
{
	m_zoom = etk_avg(0.0000001,_newVal,1000000.0);
	MarkToRedraw();
}

float ewol::Widget::GetZoom(void)
{
	return m_zoom;
}

void ewol::Widget::SetOrigin(const vec2& _pos)
{
	#if DEBUG_LEVEL > 2
		if(    m_origin.x() < -5000
		    || m_origin.y() < -5000) {
			EWOL_WARNING("[" << GetId() << "] Set origin < 5000 : " << m_origin);
		}
	#endif
	m_origin = _pos;
}

vec2 ewol::Widget::GetOrigin(void)
{
	return m_origin;
}

vec2 ewol::Widget::RelativePosition(const vec2& _pos)
{
	return _pos - m_origin;
}

void ewol::Widget::CalculateMinMaxSize(void)
{
	m_minSize = m_userMinSize.GetPixel();
	EWOL_ERROR("[" << GetId() << "] convert in min size : " << m_userMinSize << " out=" << m_minSize);
	m_maxSize = m_userMaxSize.GetPixel();
	MarkToRedraw();
}

vec2 ewol::Widget::GetCalculateMinSize(void)
{
	if (false==IsHide()) {
		return m_minSize;
	}
	return vec2(0,0);
}

vec2 ewol::Widget::GetCalculateMaxSize(void)
{
	if (false==IsHide()) {
		return m_maxSize;
	}
	return vec2(ULTIMATE_MAX_SIZE,ULTIMATE_MAX_SIZE);
}

void ewol::Widget::SetMinSize(const ewol::Dimension& _size)
{
	vec2 pixelMin = _size.GetPixel();
	vec2 pixelMax = m_userMaxSize.GetPixel();
	// check minimum & maximum compatibility :
	bool error=false;
	if (pixelMin.x()>pixelMax.x()) {
		error=true;
	}
	if (pixelMin.y()>pixelMax.y()) {
		error=true;
	}
	if (error==true) {
		EWOL_ERROR("Can not set a 'min Size' > 'max size' set nothing ...");
		return;
	}
	m_userMinSize = _size;
	ewol::RequestUpdateSize();
}

void ewol::Widget::SetNoMinSize(void)
{
	m_userMinSize.Set(vec2(0,0),ewol::Dimension::Pixel);
}

void ewol::Widget::CheckMinSize(void)
{
	vec2 pixelSize = m_userMinSize.GetPixel();
	m_minSize.setX(etk_max(m_minSize.x(), pixelSize.x()));
	m_minSize.setY(etk_max(m_minSize.y(), pixelSize.y()));
}

void ewol::Widget::SetMaxSize(const ewol::Dimension& _size)
{
	vec2 pixelMin = m_userMinSize.GetPixel();
	vec2 pixelMax = _size.GetPixel();
	// check minimum & maximum compatibility :
	bool error=false;
	if (pixelMin.x()>pixelMax.x()) {
		error=true;
	}
	if (pixelMin.y()>pixelMax.y()) {
		error=true;
	}
	if (error==true) {
		EWOL_ERROR("Can not set a 'min Size' > 'max size' set nothing ...");
		return;
	}
	m_userMaxSize = _size;
	ewol::RequestUpdateSize();
}

void ewol::Widget::SetNoMaxSize(void)
{
	m_userMaxSize.Set(vec2(ULTIMATE_MAX_SIZE,ULTIMATE_MAX_SIZE),ewol::Dimension::Pixel);
}

void ewol::Widget::CheckMaxSize(void)
{
	vec2 pixelSize = m_userMaxSize.GetPixel();
	m_maxSize.setX(etk_min(m_maxSize.x(), pixelSize.x()));
	m_maxSize.setY(etk_min(m_maxSize.y(), pixelSize.y()));
}

vec2 ewol::Widget::GetSize(void)
{
	if (false==IsHide()) {
		return m_size;
	}
	return vec2(0,0);
}

void ewol::Widget::SetExpand(const bvec2& _newExpand)
{
	if(    m_userExpand.x() != _newExpand.x()
	    || m_userExpand.y() != _newExpand.y()) {
		m_userExpand = _newExpand;
		ewol::RequestUpdateSize();
		MarkToRedraw();
	}
}

bvec2 ewol::Widget::CanExpand(void)
{
	if (false==IsHide()) {
		return m_userExpand;
	}
	return bvec2(false,false);
}


void ewol::Widget::SetFill(const bvec2& _newFill)
{
	if(    m_userFill.x() != _newFill.x()
	    || m_userFill.y() != _newFill.y()) {
		m_userFill = _newFill;
		ewol::RequestUpdateSize();
		MarkToRedraw();
	}
}

const bvec2& ewol::Widget::CanFill(void)
{
	return m_userFill;
}

// ----------------------------------------------------------------------------------------------------------------
// -- Shortcut : management of the shortcut
// ----------------------------------------------------------------------------------------------------------------

void ewol::Widget::ShortCutAdd(const char * _descriptiveString, const char * _generateEventId, etk::UString _data, bool _broadcast)
{
	if(		NULL==_descriptiveString
		||	0==strlen(_descriptiveString))
	{
		EWOL_ERROR("try to add shortcut with no descriptive string ...");
		return;
	}
	
	EventShortCut* tmpElement = new EventShortCut();
	if (NULL == tmpElement) {
		EWOL_ERROR("allocation error ... Memory error ...");
		return;
	}
	tmpElement->broadcastEvent = _broadcast;
	tmpElement->generateEventId = _generateEventId;
	tmpElement->eventData = _data;
	// parsing of the string :
	//"ctrl+shift+alt+meta+s"
	const char * tmp = strstr(_descriptiveString, "ctrl");
	if(NULL != tmp) {
		tmpElement->specialKey.ctrl = true;
	}
	tmp = strstr(_descriptiveString, "shift");
	if(NULL != tmp) {
		tmpElement->specialKey.shift = true;
	}
	tmp = strstr(_descriptiveString, "alt");
	if(NULL != tmp) {
		tmpElement->specialKey.alt = true;
	}
	tmp = strstr(_descriptiveString, "meta");
	if(NULL != tmp) {
		tmpElement->specialKey.meta = true;
	}
	if(NULL != strstr(_descriptiveString, "F12") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardF12;
	} else if(NULL != strstr(_descriptiveString, "F11") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardF11;
	} else if(NULL != strstr(_descriptiveString, "F10") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardF10;
	} else if(NULL != strstr(_descriptiveString, "F9") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardF9;
	} else if(NULL != strstr(_descriptiveString, "F8") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardF8;
	} else if(NULL != strstr(_descriptiveString, "F7") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardF7;
	} else if(NULL != strstr(_descriptiveString, "F6") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardF6;
	} else if(NULL != strstr(_descriptiveString, "F5") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardF5;
	} else if(NULL != strstr(_descriptiveString, "F4") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardF4;
	} else if(NULL != strstr(_descriptiveString, "F3") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardF3;
	} else if(NULL != strstr(_descriptiveString, "F2") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardF2;
	} else if(NULL != strstr(_descriptiveString, "F1") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardF1;
	} else if(NULL != strstr(_descriptiveString, "LEFT") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardLeft;
	} else if(NULL != strstr(_descriptiveString, "RIGHT") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardRight;
	} else if(NULL != strstr(_descriptiveString, "UP") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardUp;
	} else if(NULL != strstr(_descriptiveString, "DOWN") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardDown;
	} else if(NULL != strstr(_descriptiveString, "PAGE_UP") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardPageUp;
	} else if(NULL != strstr(_descriptiveString, "PAGE_DOWN") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardPageDown;
	} else if(NULL != strstr(_descriptiveString, "START") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardStart;
	} else if(NULL != strstr(_descriptiveString, "END") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardEnd;
	} else if(NULL != strstr(_descriptiveString, "PRINT") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardPrint;
	} else if(NULL != strstr(_descriptiveString, "ARRET_DEFIL") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardStopDefil;
	} else if(NULL != strstr(_descriptiveString, "WAIT") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardWait;
	} else if(NULL != strstr(_descriptiveString, "INSERT") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardInsert;
	} else if(NULL != strstr(_descriptiveString, "CAPLOCK") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardCapLock;
	} else if(NULL != strstr(_descriptiveString, "CONTEXT_MENU") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardContextMenu;
	} else if(NULL != strstr(_descriptiveString, "NUM_LOCK") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardNumLock;
	} else {
		tmpElement->unicodeValue = _descriptiveString[strlen(_descriptiveString) -1];
	}
	// add it on the List ...
	m_localShortcut.PushBack(tmpElement);
}


void ewol::Widget::ShortCutClean(void)
{
	for (int32_t iii=0; iii<m_localShortcut.Size(); iii++) {
		if (NULL != m_localShortcut[iii]) {
			delete(m_localShortcut[iii]);
			m_localShortcut[iii]=NULL;
		}
	}
	m_localShortcut.Clear();
}


bool ewol::Widget::OnEventShortCut(ewol::SpecialKey& _special, uniChar_t _unicodeValue, ewol::keyEvent::keyboard_te _kbMove, bool _isDown)
{
	if (_unicodeValue >= 'A' && _unicodeValue <='Z') {
		_unicodeValue += 'a' - 'A';
	}
	//EWOL_INFO("Try to find generic shortcut ...");
	for(int32_t iii=m_localShortcut.Size()-1; iii>=0; iii--) {
		if(NULL != m_localShortcut[iii]) {
			if(    m_localShortcut[iii]->specialKey.shift == _special.shift
			    && m_localShortcut[iii]->specialKey.ctrl  == _special.ctrl
			    && m_localShortcut[iii]->specialKey.alt   == _special.alt
			    && m_localShortcut[iii]->specialKey.meta  == _special.meta
			    && (    (    m_localShortcut[iii]->keyboardMoveValue == ewol::keyEvent::keyboardUnknow
			              && m_localShortcut[iii]->unicodeValue == _unicodeValue)
			         || (    m_localShortcut[iii]->keyboardMoveValue == _kbMove
			              && m_localShortcut[iii]->unicodeValue == 0)
			       ) )
			{
				if (_isDown) {
					if (true == m_localShortcut[iii]->broadcastEvent) {
						// send message at all the widget (exepted this one)
						SendMultiCast(m_localShortcut[iii]->generateEventId, m_localShortcut[iii]->eventData);
					}
					// send message direct to the current widget (in every case, really useful for some generic windows shortcut)
					ewol::EMessage tmpMsg(this, m_localShortcut[iii]->generateEventId, m_localShortcut[iii]->eventData);
					OnReceiveMessage(tmpMsg);
				} // no else
				return true;
			}
		}
	}
	return false;
}


void ewol::Widget::GrabCursor(void)
{
	if (false == m_grabCursor) {
		eSystem::InputEventGrabPointer(this);
		m_grabCursor = true;
	}
}

void ewol::Widget::UnGrabCursor(void)
{
	if (true==m_grabCursor) {
		eSystem::InputEventUnGrabPointer();
		m_grabCursor = false;
	}
}


bool ewol::Widget::GetGrabStatus(void)
{
	return m_grabCursor;
}



void ewol::Widget::SetCursor(ewol::cursorDisplay_te _newCursor)
{
	EWOL_DEBUG("Change Cursor in " << _newCursor);
	m_cursorDisplay = _newCursor;
	guiInterface::SetCursor(m_cursorDisplay);
}

ewol::cursorDisplay_te ewol::Widget::GetCursor(void)
{
	return m_cursorDisplay;
}

bool ewol::Widget::LoadXML(TiXmlNode* _node)
{
	// Call EObject basic parser
	ewol::EObject::LoadXML(_node); // note : Load standard parameters (attribute in XML)
	MarkToRedraw();
	return true;
}

ewol::Widget* ewol::Widget::GetWidgetNamed(const etk::UString& _widgetName)
{
	if (GetName()==_widgetName) {
		return this;
	}
	return NULL;
}


bool ewol::Widget::SystemEventEntry(ewol::EventEntrySystem& _event)
{
	if (NULL != m_up) {
		if (true==m_up->SystemEventEntry(_event)) {
			return true;
		}
	}
	return OnEventEntry(_event.m_event);
}

bool ewol::Widget::SystemEventInput(ewol::EventInputSystem& _event)
{
	if (NULL != m_up) {
		if (true==m_up->SystemEventInput(_event)) {
			return true;
		}
	}
	return OnEventInput(_event.m_event);
}


void ewol::Widget::SetGravity(gravity_te _gravity)
{
	m_gravity = _gravity;
	MarkToRedraw();
}


bool ewol::Widget::OnSetConfig(const ewol::EConfig& _conf)
{
	if (true == ewol::EObject::OnSetConfig(_conf)) {
		return true;
	}
	if (_conf.GetConfig() == ewol::Widget::configFill) {
		SetFill(_conf.GetData());
		return true;
	}
	if (_conf.GetConfig() == ewol::Widget::configExpand) {
		SetExpand(_conf.GetData());
		return true;
	}
	if (_conf.GetConfig() == ewol::Widget::configHide) {
		if(true == _conf.GetData().ToBool()) {
			Hide();
		} else {
			Show();
		}
		return true;
	}
	if (_conf.GetConfig() == ewol::Widget::configFocus) {
		if(true == _conf.GetData().ToBool()) {
			KeepFocus();
		} else {
			//nothing to do ...
		}
		return true;
	}
	if (_conf.GetConfig() == ewol::Widget::configMinSize) {
		m_userMinSize = _conf.GetData();
		return true;
	}
	if (_conf.GetConfig() == ewol::Widget::configMaxSize) {
		m_userMaxSize = _conf.GetData();
		return true;
	}
	if (_conf.GetConfig() == ewol::Widget::configGravity) {
		m_gravity = StringToGravity(_conf.GetData());
		return true;
	}
	return false;
}

bool ewol::Widget::OnGetConfig(const char* _config, etk::UString& _result) const
{
	if (true == ewol::EObject::OnGetConfig(_config, _result)) {
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
		// TODO : Understand why it does not work : _result = m_hide;
		if (true==m_hide) {
			_result = "true";
		} else {
			_result = "false";
		}
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
		_result = GravityToString(m_gravity);
		return true;
	}
	return false;
}
