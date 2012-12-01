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
#include <ewol/ewol.h>


extern const char * const ewolEventButtonColorChange    = "ewol-Button-Color-Change";


// DEFINE for the shader display system :
#define STATUS_UP        (0)
#define STATUS_HOVER     (2)
#define STATUS_PRESSED   (1)
#define STATUS_DOWN      (3)


#undef __class__
#define __class__	"ButtonColor"

widget::ButtonColor::ButtonColor(draw::Color baseColor) :
	m_shaper("THEME:GUI:widgetButton.conf"),
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



bool widget::ButtonColor::CalculateMinSize(void)
{
	etk::Vector2D<float> padding = m_shaper.GetPadding();
	etk::UString label = draw::GetString(m_textColorFg);
	etk::Vector3D<int32_t> minSize = m_text.CalculateSize(label);
	m_minSize.x = padding.x*2 + minSize.x + 7;
	m_minSize.y = padding.y*2 + minSize.y;
	MarkToRedraw();
	return true;
}



void widget::ButtonColor::OnDraw(ewol::DrawProperty& displayProp)
{
	m_shaper.Draw();
	m_text.Draw();
}


void widget::ButtonColor::OnRegenerateDisplay(void)
{
	if (true == NeedRedraw()) {
		m_text.Clear();
		m_shaper.Clear();
		
		etk::Vector2D<float> padding = m_shaper.GetPadding();
		
		etk::UString label = draw::GetString(m_textColorFg);
		
		etk::Vector2D<int32_t> localSize = m_minSize;
		
		etk::Vector3D<float> tmpOrigin((m_size.x - m_minSize.x) / 2.0,
		                               (m_size.y - m_minSize.y) / 2.0,
		                               0.0);
		                               
		// no change for the text orogin : 
		etk::Vector3D<float> tmpTextOrigin((m_size.x - m_minSize.x) / 2.0,
		                                   (m_size.y - m_minSize.y) / 2.0,
		                                   0.0);
		
		if (true==m_userFill.x) {
			localSize.x = m_size.x;
			tmpOrigin.x = 0.0;
			tmpTextOrigin.x = 0.0;
		}
		if (true==m_userFill.y) {
			localSize.y = m_size.y;
		}
		tmpOrigin.x += padding.x;
		tmpOrigin.y += padding.y;
		tmpTextOrigin.x += padding.x;
		tmpTextOrigin.y += padding.y;
		localSize.x -= 2*padding.x;
		localSize.y -= 2*padding.y;
		
		// clean the element
		m_text.Reset();
		if(    m_textColorFg.r < 100
		    || m_textColorFg.g < 100
		    || m_textColorFg.b < 100) {
			m_text.SetColor(draw::color::white);
		} else {
			m_text.SetColor(draw::color::black);
		}
		m_text.SetPos(tmpTextOrigin);
		m_text.SetColorBg(m_textColorFg);
		m_text.SetTextAlignement(tmpTextOrigin.x, tmpTextOrigin.x+localSize.x, ewol::Text::alignCenter);
		m_text.Print(label);
		
		
		if (true==m_userFill.y) {
			tmpOrigin.y = padding.y;
		}
		
		// selection area :
		m_selectableAreaPos = etk::Vector2D<float>(tmpOrigin.x-padding.x, tmpOrigin.y-padding.y);
		m_selectableAreaSize = localSize + etk::Vector2D<float>(2,2)*padding;
		m_shaper.SetOrigin(m_selectableAreaPos );
		m_shaper.SetSize(m_selectableAreaSize);
		m_shaper.SetInsidePos(etk::Vector2D<float>(tmpTextOrigin.x, tmpTextOrigin.y) );
		etk::Vector3D<float> tmpp = m_text.CalculateSize(label);
		etk::Vector2D<float> tmpp2(tmpp.x, tmpp.y);
		m_shaper.SetInsideSize(tmpp2);
	}
}


bool widget::ButtonColor::OnEventInput(ewol::keyEvent::type_te type, int32_t IdInput, ewol::keyEvent::status_te typeEvent, etk::Vector2D<float> pos)
{
	bool previousHoverState = m_mouseHover;
	if(ewol::keyEvent::statusLeave == typeEvent) {
		m_mouseHover = false;
		m_buttonPressed = false;
	} else {
		etk::Vector2D<float> relativePos = RelativePosition(pos);
		// prevent error from ouside the button
		if(    relativePos.x < m_selectableAreaPos.x
		    || relativePos.y < m_selectableAreaPos.y
		    || relativePos.x > m_selectableAreaPos.x + m_selectableAreaSize.x
		    || relativePos.y > m_selectableAreaPos.y + m_selectableAreaSize.y ) {
			m_mouseHover = false;
			m_buttonPressed = false;
		} else {
			m_mouseHover = true;
		}
	}
	bool previousPressed = m_buttonPressed;
	//EWOL_DEBUG("Event on BT ... mouse position : " << m_mouseHover);
	if (true == m_mouseHover) {
		if (1 == IdInput) {
			if(ewol::keyEvent::statusDown == typeEvent) {
				m_buttonPressed = true;
				MarkToRedraw();
			}
			if(ewol::keyEvent::statusUp == typeEvent) {
				m_buttonPressed = false;
				MarkToRedraw();
			}
			if(ewol::keyEvent::statusSingle == typeEvent) {
				m_buttonPressed = false;
				m_mouseHover = false;
				// create a context menu : 
				m_widgetContextMenu = new widget::ContextMenu();
				if (NULL == m_widgetContextMenu) {
					EWOL_ERROR("Allocation Error");
					return true;
				}
				etk::Vector2D<float> tmpPos = m_origin + m_selectableAreaPos + m_selectableAreaSize;
				tmpPos.x -= m_minSize.x/2.0;
				m_widgetContextMenu->SetPositionMark(widget::CONTEXT_MENU_MARK_BOTTOM, tmpPos );
				
				widget::ColorChooser * myColorChooser = new widget::ColorChooser();
				myColorChooser->SetColor(m_textColorFg);
				// set it in the pop-up-system : 
				m_widgetContextMenu->SubWidgetSet(myColorChooser);
				myColorChooser->RegisterOnEvent(this, ewolEventColorChooserChange, ewolEventColorChooserChange);
				ewol::WindowsPopUpAdd(m_widgetContextMenu);
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


void widget::ButtonColor::SetValue(draw::Color color)
{
	m_textColorFg = color;
	MarkToRedraw();
}

draw::Color widget::ButtonColor::GetValue(void)
{
	return m_textColorFg;
}


void widget::ButtonColor::OnReceiveMessage(ewol::EObject * CallerObject, const char * eventId, etk::UString data)
{
	EWOL_INFO("Receive MSG : \"" << eventId << "\" ==> data=\"" << data << "\"" );
	if (eventId == ewolEventColorChooserChange) {
		draw::ParseColor(data.c_str(), m_textColorFg);
		GenerateEventId(ewolEventButtonColorChange, data);
		MarkToRedraw();
	}
}


void widget::ButtonColor::ChangeStatusIn(int32_t newStatusId)
{
	if (true == m_shaper.ChangeStatusIn(newStatusId) ) {
		PeriodicCallSet(true);
		MarkToRedraw();
	}
}



void widget::ButtonColor::PeriodicCall(int64_t localTime)
{
	if (false == m_shaper.PeriodicCall(localTime) ) {
		PeriodicCallSet(false);
	}
	MarkToRedraw();
}
