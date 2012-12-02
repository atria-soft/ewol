/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */


#include <ewol/ewol.h>
#include <ewol/widget/Button.h>
#include <ewol/widget/WidgetManager.h>


extern const char * const ewolEventButtonPressed    = "ewol-button-Pressed";
extern const char * const ewolEventButtonDown       = "ewol-button-down";
extern const char * const ewolEventButtonUp         = "ewol-button-up";
extern const char * const ewolEventButtonEnter      = "ewol-button-enter";
extern const char * const ewolEventButtonLeave      = "ewol-button-leave";
extern const char * const ewolEventButtonValue      = "ewol-button-value";

#undef __class__
#define __class__	"Button"

// DEFINE for the shader display system :
#define STATUS_UP        (0)
#define STATUS_HOVER     (2)
#define STATUS_PRESSED   (1)
#define STATUS_DOWN      (3)


widget::Button::Button(etk::UString newLabel, etk::UString shaperName) :
	m_shaper(shaperName),
	m_label(newLabel),
	m_toggleMode(false),
	m_value(false),
	m_mouseHover(false),
	m_buttonPressed(false),
	m_imageDisplaySize(32)
{
	AddEventId(ewolEventButtonPressed);
	AddEventId(ewolEventButtonDown);
	AddEventId(ewolEventButtonUp);
	AddEventId(ewolEventButtonEnter);
	AddEventId(ewolEventButtonLeave);
	AddEventId(ewolEventButtonValue);
	
	m_shaper.ChangeStatusIn(STATUS_UP);
	
	SetCanHaveFocus(true);
	// Limit event at 1:
	SetMouseLimit(1);
}


widget::Button::~Button(void)
{
	
}

void widget::Button::SetShaperName(etk::UString shaperName)
{
	m_shaper.SetSource(shaperName);
}

void widget::Button::SetImage(etk::UString imageName, draw::Color color)
{
	m_imageColor = color;
	m_displayImage.SetSource(imageName);
	MarkToRedraw();
	ewol::RequestUpdateSize();
}

void widget::Button::SetImageToggle(etk::UString imageName, draw::Color color)
{
	m_imageColorToggle = color;
	m_displayImageToggle.SetSource(imageName);
	MarkToRedraw();
	ewol::RequestUpdateSize();
}


bool widget::Button::CalculateMinSize(void)
{
	etk::Vector2D<float> padding = m_shaper.GetPadding();
	m_displayText.Clear();
	etk::Vector3D<int32_t> minSize = m_displayText.CalculateSizeDecorated(m_label);
	if(    true == m_toggleMode
	    && m_labelToggle.Size()!=0) {
		m_displayText.Clear();
		etk::Vector3D<int32_t> minSizeToggle = m_displayText.CalculateSizeDecorated(m_labelToggle);
		minSize.x = etk_max(minSize.x, minSizeToggle.x);
		minSize.y = etk_max(minSize.y, minSizeToggle.y);
		minSize.z = etk_max(minSize.z, minSizeToggle.z);
	}
	m_minSize.x = padding.x*2 + minSize.x;
	m_minSize.y = padding.y*2 + minSize.y;
	// Add the image element ...
	if(    true == m_displayImage.HasSources()
	    || true == m_displayImageToggle.HasSources()) {
		m_minSize.x += padding.x/2 + m_imageDisplaySize;
	}
	MarkToRedraw();
	return true;
}


void widget::Button::SetLabel(etk::UString newLabel)
{
	m_label = newLabel;
	MarkToRedraw();
	ewol::RequestUpdateSize();
}

etk::UString widget::Button::GetLabel(void)
{
	return m_label;
}

void widget::Button::SetLabelToggle(etk::UString newLabel)
{
	m_labelToggle = newLabel;
	MarkToRedraw();
	ewol::RequestUpdateSize();
}

etk::UString widget::Button::GetLabelToggle(void)
{
	return m_labelToggle;
}

void widget::Button::SetValue(bool val)
{
	if (m_value != val) {
		m_value = val;
		MarkToRedraw();
	}
}

bool widget::Button::GetValue(void)
{
	return m_value;
}

void widget::Button::SetToggleMode(bool togg)
{
	if (m_toggleMode != togg) {
		m_toggleMode = togg;
		if (m_value == true) {
			m_value = false;
			// TODO : Change display and send event ...
		}
		MarkToRedraw();
	}
}

void widget::Button::OnDraw(ewol::DrawProperty& displayProp)
{
	m_shaper.Draw();
	if(    false == m_toggleMode
	    || false == m_value) {
		m_displayImage.Draw();
	} else {
		m_displayImageToggle.Draw();
	}
	m_displayText.Draw();
}

void widget::Button::OnRegenerateDisplay(void)
{
	if (true == NeedRedraw()) {
		
		etk::Vector2D<float> padding = m_shaper.GetPadding();
		// to know the size of one Line : 
		etk::Vector3D<int32_t> minSize = m_displayText.CalculateSize('A');
		etk::Vector3D<int32_t> curentTextSize;
		if(    false == m_toggleMode
		    || false == m_value
		    || m_labelToggle.Size()==0) {
			curentTextSize = m_displayText.CalculateSizeDecorated(m_label);
		} else {
			curentTextSize = m_displayText.CalculateSizeDecorated(m_labelToggle);
		}
		
		m_displayImage.Clear();
		m_displayImageToggle.Clear();
		m_shaper.Clear();
		m_displayText.Clear();
		
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
		
		tmpTextOrigin.y += (m_minSize.y-2*padding.y) - minSize.y;
		
		etk::Vector2D<float> textPos(tmpTextOrigin.x, tmpTextOrigin.y);
		
		if(    true == m_displayImage.HasSources()
		    || true == m_displayImageToggle.HasSources()) {
			etk::Vector3D<int32_t> imagePos(tmpOrigin.x-padding.x/4,
			                                tmpOrigin.y-padding.x/4+(m_minSize.y-m_imageDisplaySize-2*padding.y)/2.0,
			                                0);
			etk::Vector2D<int32_t> imageSize(m_imageDisplaySize,
			                                 m_imageDisplaySize);
			if(    false==m_toggleMode
			    || false==m_value) {
				m_displayImage.SetPos(imagePos);
				m_displayImage.SetColor(m_imageColor);
				m_displayImage.Print(imageSize);
			} else {
				m_displayImageToggle.SetPos(imagePos);
				m_displayImageToggle.SetColor(m_imageColorToggle);
				m_displayImageToggle.Print(imageSize);
			}
			// update the text position ...
			tmpTextOrigin.x += padding.x/2 + m_imageDisplaySize;
		}
		
		etk::Vector3D<float> drawClippingPos(padding.x, padding.y, -0.5);
		etk::Vector3D<float> drawClippingSize((float)(m_size.x - padding.x),
		                                      (float)(m_size.y - padding.y),
		                                      (float)1.0);
		
		// clean the element
		m_displayText.Reset();
		m_displayText.SetPos(tmpTextOrigin);
		if(    true == m_displayImage.HasSources()
		    || true == m_displayImageToggle.HasSources()) {
			m_displayText.SetTextAlignement(tmpTextOrigin.x, tmpTextOrigin.x+localSize.x-m_imageDisplaySize, ewol::Text::alignCenter);
		} else {
			m_displayText.SetTextAlignement(tmpTextOrigin.x, tmpTextOrigin.x+localSize.x, ewol::Text::alignCenter);
		}
		m_displayText.SetClipping(drawClippingPos, drawClippingSize);
		if(    false == m_toggleMode
		    || false == m_value
		    || m_labelToggle.Size()==0) {
			m_displayText.PrintDecorated(m_label);
		} else {
			m_displayText.PrintDecorated(m_labelToggle);
		}
		//m_displayText.Translate(tmpOrigin);
		
		
		if (true==m_userFill.y) {
			tmpOrigin.y = padding.y;
		}
		
		// selection area :
		m_selectableAreaPos = etk::Vector2D<float>(tmpOrigin.x-padding.x, tmpOrigin.y-padding.y);
		m_selectableAreaSize = localSize + etk::Vector2D<float>(2,2)*padding;
		m_shaper.SetOrigin(m_selectableAreaPos );
		m_shaper.SetSize(m_selectableAreaSize);
		m_shaper.SetInsidePos(etk::Vector2D<float>(tmpTextOrigin.x, tmpTextOrigin.y) );
		etk::Vector3D<float> tmpp = m_displayText.CalculateSize(m_label);
		etk::Vector2D<float> tmpp2(tmpp.x, tmpp.y);
		m_shaper.SetInsideSize(tmpp2);
		
	}
}


bool widget::Button::OnEventInput(ewol::keyEvent::type_te type, int32_t IdInput, ewol::keyEvent::status_te typeEvent, etk::Vector2D<float> pos)
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
				GenerateEventId(ewolEventButtonDown);
				m_buttonPressed = true;
				MarkToRedraw();
			}
			if(ewol::keyEvent::statusUp == typeEvent) {
				GenerateEventId(ewolEventButtonUp);
				m_buttonPressed = false;
				MarkToRedraw();
			}
			if(ewol::keyEvent::statusSingle == typeEvent) {
				// inverse value :
				m_value = (m_value)?false:true;
				GenerateEventId(ewolEventButtonPressed);
				GenerateEventId(ewolEventButtonValue, m_value);
				if(    false == m_toggleMode
				    && true == m_value) {
					m_value = false;
					GenerateEventId(ewolEventButtonValue, m_value);
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
				if (true == m_value) {
					ChangeStatusIn(STATUS_DOWN);
				} else {
					ChangeStatusIn(STATUS_UP);
				}
			}
		}
	}
	return m_mouseHover;
}


bool widget::Button::OnEventKb(ewol::keyEvent::status_te typeEvent, uniChar_t unicodeData)
{
	//EWOL_DEBUG("BT PRESSED : \"" << UTF8_data << "\" size=" << strlen(UTF8_data));
	if(    typeEvent == ewol::keyEvent::statusDown
	    && unicodeData == '\r') {
		GenerateEventId(ewolEventButtonEnter);
	}
	return false;
}



void widget::Button::ChangeStatusIn(int32_t newStatusId)
{
	if (true == m_shaper.ChangeStatusIn(newStatusId) ) {
		PeriodicCallSet(true);
		MarkToRedraw();
	}
}


void widget::Button::PeriodicCall(int64_t localTime)
{
	if (false == m_shaper.PeriodicCall(localTime) ) {
		PeriodicCallSet(false);
	}
	MarkToRedraw();
}

void widget::Button::SetImageSize(int32_t size)
{
	MarkToRedraw();
	ewol::RequestUpdateSize();
	m_imageDisplaySize = size;
}



