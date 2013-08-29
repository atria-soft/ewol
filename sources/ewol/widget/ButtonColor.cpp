/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */


#include <ewol/widget/ButtonColor.h>

#include <ewol/compositing/Drawing.h>
#include <ewol/widget/WidgetManager.h>
#include <ewol/widget/meta/ColorChooser.h>
#include <ewol/widget/Windows.h>
#include <ewol/ewol.h>
#include <ewol/renderer/os/eSystem.h>

extern const char * const ewolEventButtonColorChange    = "ewol-Button-Color-Change";


// DEFINE for the shader display system :
#define STATUS_UP        (0)
#define STATUS_HOVER     (2)
#define STATUS_PRESSED   (1)
#define STATUS_DOWN      (3)


#undef __class__
#define __class__	"ButtonColor"



static ewol::Widget* Create(void)
{
	return new widget::ButtonColor();
}

void widget::ButtonColor::Init(ewol::WidgetManager& _widgetManager)
{
	_widgetManager.AddWidgetCreator(__class__,&Create);
}

widget::ButtonColor::ButtonColor(etk::Color<> baseColor, etk::UString shaperName) :
	m_shaper(shaperName),
	m_textColorFg(baseColor),
	m_widgetContextMenu(NULL)
{
	AddEventId(ewolEventButtonColorChange);
	ChangeStatusIn(STATUS_UP);
	SetCanHaveFocus(true);
	// Limit event at 1:
	SetMouseLimit(1);
}


widget::ButtonColor::~ButtonColor(void)
{
	
}


void widget::ButtonColor::SetShaperName(etk::UString shaperName)
{
	m_shaper.SetSource(shaperName);
}


void widget::ButtonColor::CalculateMinMaxSize(void)
{
	vec2 padding = m_shaper.GetPadding();
	etk::UString label = m_textColorFg.GetString();
	vec3 minSize = m_text.CalculateSize(label);
	m_minSize.setX(padding.x()*2 + minSize.x() + 7);
	m_minSize.setY(padding.y()*2 + minSize.y() );
	MarkToRedraw();
}



void widget::ButtonColor::OnDraw(void)
{
	m_shaper.Draw();
	m_text.Draw();
}


void widget::ButtonColor::OnRegenerateDisplay(void)
{
	if (true == NeedRedraw()) {
		m_text.Clear();
		m_shaper.Clear();
		
		vec2 padding = m_shaper.GetPadding();
		
		etk::UString label = m_textColorFg.GetString();
		
		ivec2 localSize = m_minSize;
		
		vec3 tmpOrigin((m_size.x() - m_minSize.x()) / 2.0,
		               (m_size.y() - m_minSize.y()) / 2.0,
		               0);
		// no change for the text orogin : 
		vec3 tmpTextOrigin((m_size.x() - m_minSize.x()) / 2.0,
		                   (m_size.y() - m_minSize.y()) / 2.0,
		                   0);
		
		if (true==m_userFill.x()) {
			localSize.setX(m_size.x());
			tmpOrigin.setX(0);
			tmpTextOrigin.setX(0);
		}
		if (true==m_userFill.y()) {
			localSize.setY(m_size.y());
		}
		tmpOrigin += vec3(padding.x(), padding.y(), 0);
		tmpTextOrigin += vec3(padding.x(), padding.y(), 0);
		localSize -= ivec2(2*padding.x(), 2*padding.y());
		
		// clean the element
		m_text.Reset();
		if(    m_textColorFg.r() < 100
		    || m_textColorFg.g() < 100
		    || m_textColorFg.b() < 100) {
			m_text.SetColor(etk::color::white);
		} else {
			m_text.SetColor(etk::color::black);
		}
		m_text.SetPos(tmpTextOrigin);
		m_text.SetColorBg(m_textColorFg);
		m_text.SetTextAlignement(tmpTextOrigin.x(), tmpTextOrigin.x()+localSize.x(), ewol::Text::alignCenter);
		m_text.Print(label);
		
		
		if (true==m_userFill.y()) {
			tmpOrigin.setY(padding.y());
		}
		
		// selection area :
		m_selectableAreaPos = vec2(tmpOrigin.x()-padding.x(), tmpOrigin.y()-padding.y());
		m_selectableAreaSize = localSize + vec2(2,2)*padding;
		m_shaper.SetOrigin(m_selectableAreaPos );
		m_shaper.SetSize(m_selectableAreaSize);
		m_shaper.SetInsidePos(vec2(tmpTextOrigin.x(), tmpTextOrigin.y()) );
		vec3 tmpp = m_text.CalculateSize(label);
		vec2 tmpp2(tmpp.x(), tmpp.y());
		m_shaper.SetInsideSize(tmpp2);
	}
}


bool widget::ButtonColor::OnEventInput(const ewol::EventInput& _event)
{
	bool previousHoverState = m_mouseHover;
	if(ewol::keyEvent::statusLeave == _event.GetStatus()) {
		m_mouseHover = false;
		m_buttonPressed = false;
	} else {
		vec2 relativePos = RelativePosition(_event.GetPos());
		// prevent error from ouside the button
		if(    relativePos.x() < m_selectableAreaPos.x()
		    || relativePos.y() < m_selectableAreaPos.y()
		    || relativePos.x() > m_selectableAreaPos.x() + m_selectableAreaSize.x()
		    || relativePos.y() > m_selectableAreaPos.y() + m_selectableAreaSize.y() ) {
			m_mouseHover = false;
			m_buttonPressed = false;
		} else {
			m_mouseHover = true;
		}
	}
	bool previousPressed = m_buttonPressed;
	//EWOL_DEBUG("Event on BT ... mouse position : " << m_mouseHover);
	if (true == m_mouseHover) {
		if (1 == _event.GetId()) {
			if(ewol::keyEvent::statusDown == _event.GetStatus()) {
				m_buttonPressed = true;
				MarkToRedraw();
			}
			if(ewol::keyEvent::statusUp == _event.GetStatus()) {
				m_buttonPressed = false;
				MarkToRedraw();
			}
			if(ewol::keyEvent::statusSingle == _event.GetStatus()) {
				m_buttonPressed = false;
				m_mouseHover = false;
				// create a context menu : 
				m_widgetContextMenu = new widget::ContextMenu();
				if (NULL == m_widgetContextMenu) {
					EWOL_ERROR("Allocation Error");
					return true;
				}
				vec2 tmpPos = m_origin + m_selectableAreaPos + m_selectableAreaSize;
				tmpPos.setX( tmpPos.x() - m_minSize.x()/2.0);
				m_widgetContextMenu->SetPositionMark(widget::CONTEXT_MENU_MARK_BOTTOM, tmpPos );
				
				widget::ColorChooser * myColorChooser = new widget::ColorChooser();
				myColorChooser->SetColor(m_textColorFg);
				// set it in the pop-up-system : 
				m_widgetContextMenu->SetSubWidget(myColorChooser);
				myColorChooser->RegisterOnEvent(this, ewolEventColorChooserChange, ewolEventColorChooserChange);
				ewol::Windows* currentWindows = GetWindows();
				if (NULL == currentWindows) {
					EWOL_ERROR("Can not get the curent Windows...");
					delete(m_widgetContextMenu);
					m_widgetContextMenu=NULL;
				} else {
					currentWindows->PopUpWidgetPush(m_widgetContextMenu);
				}
				MarkToRedraw();
			}
		}
	}
	if(    m_mouseHover != previousHoverState
	    || m_buttonPressed != previousPressed) {
		if (true==m_buttonPressed) {
			ChangeStatusIn(STATUS_PRESSED);
		} else {
			if (true==m_mouseHover) {
				ChangeStatusIn(STATUS_HOVER);
			} else {
				ChangeStatusIn(STATUS_UP);
			}
		}
	}
	return m_mouseHover;
}


void widget::ButtonColor::SetValue(etk::Color<> _color)
{
	m_textColorFg = _color;
	MarkToRedraw();
}

etk::Color<> widget::ButtonColor::GetValue(void)
{
	return m_textColorFg;
}


void widget::ButtonColor::OnReceiveMessage(const ewol::EMessage& _msg)
{
	EWOL_INFO("Receive MSG : " <<  _msg.GetData());
	if (_msg.GetMessage() == ewolEventColorChooserChange) {
		m_textColorFg = _msg.GetData();
		GenerateEventId(ewolEventButtonColorChange, _msg.GetData());
		MarkToRedraw();
	}
}


void widget::ButtonColor::ChangeStatusIn(int32_t _newStatusId)
{
	if (true == m_shaper.ChangeStatusIn(_newStatusId) ) {
		PeriodicCallEnable();
		MarkToRedraw();
	}
}



void widget::ButtonColor::PeriodicCall(const ewol::EventTime& _event)
{
	if (false == m_shaper.PeriodicCall(_event) ) {
		PeriodicCallDisable();
	}
	MarkToRedraw();
}
