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
	m_imageDisplaySize(32),
	m_selectableAreaPos(0,0),
	m_selectableAreaSize(0,0)
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

void widget::Button::SetImage(etk::UString imageName, draw::Color color, int32_t size)
{
	m_imageColor = color;
	m_imageDisplaySize = size;
	m_displayImage.SetSource(imageName, size);
	MarkToRedraw();
	ewol::RequestUpdateSize();
}

void widget::Button::SetImageToggle(etk::UString imageName, draw::Color color, int32_t size)
{
	m_imageColorToggle = color;
	m_imageDisplaySize = size;
	m_displayImageToggle.SetSource(imageName, size);
	MarkToRedraw();
	ewol::RequestUpdateSize();
}


bool widget::Button::CalculateMinSize(void)
{
	vec2 padding = m_shaper.GetPadding();
	m_displayText.Clear();
	if(    m_label.Size()==0
	    && m_labelToggle.Size()==0
	    && (    true == m_displayImage.HasSources()
	         || true == m_displayImageToggle.HasSources()) ) {
		// special case of only one image display ==> certer it ...
		m_minSize.setX(padding.x()*2 + m_imageDisplaySize);
		m_minSize.setY(padding.y()*2 + m_imageDisplaySize);
	} else {
		vec3 minSize(0,0,0);
		//faster if no text set ...
		if (m_label.Size()!=0) {
			minSize = m_displayText.CalculateSizeDecorated(m_label);
		}
		if(    true == m_toggleMode
		    && m_labelToggle.Size()!=0) {
			m_displayText.Clear();
			vec3 minSizeToggle = m_displayText.CalculateSizeDecorated(m_labelToggle);
			minSize.setValue(etk_max(minSize.x(), minSizeToggle.x()),
			                 etk_max(minSize.y(), minSizeToggle.y()),
			                 etk_max(minSize.z(), minSizeToggle.z()));
		}
		m_minSize.setX(padding.x()*2 + minSize.x());
		m_minSize.setY(padding.y()*2 + minSize.y());
		// Add the image element ...
		if(    true == m_displayImage.HasSources()
		    || true == m_displayImageToggle.HasSources()) {
			m_minSize.setX(m_minSize.x()+ padding.x()/2 + m_imageDisplaySize);
		}
	}
	CheckMinSize();
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
		// clear the previous display :
		m_displayImage.Clear();
		m_displayImageToggle.Clear();
		m_shaper.Clear();
		m_displayText.Clear();
		
		// know the current padding
		vec2 padding = m_shaper.GetPadding();
		// to know the size of one Line : 
		vec3 minSize = m_displayText.CalculateSize('A');
		ivec2 localSize = m_minSize;
		
		vec3 sizeText(0,0,0);
		
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
		tmpOrigin += vec3(padding.x(),padding.y(),0);
		tmpTextOrigin += vec3(padding.x(), padding.y(), 0);
		localSize -= ivec2(2*padding.x(), 2*padding.y());
		
		if(    m_label.Size()==0
		    && m_labelToggle.Size()==0
		    && (    true == m_displayImage.HasSources()
		         || true == m_displayImageToggle.HasSources()) ) {
			vec3 imagePos(tmpOrigin.x()-padding.x()/4,
			              tmpOrigin.y()-padding.x()/4+(m_minSize.y()-m_imageDisplaySize-2*padding.y())/2.0,
			              0);
			vec2 imageSize(m_imageDisplaySize,
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
		} else {
			vec3 curentTextSize(0,0,0);
			if(    false == m_toggleMode
			    || false == m_value
			    || m_labelToggle.Size()==0) {
				curentTextSize = m_displayText.CalculateSizeDecorated(m_label);
			} else {
				curentTextSize = m_displayText.CalculateSizeDecorated(m_labelToggle);
			}
			
			tmpTextOrigin.setY(tmpTextOrigin.y()+ (m_minSize.y()-2*padding.y()) - minSize.y());
			
			vec2 textPos(tmpTextOrigin.x(), tmpTextOrigin.y());
			
			if(    true == m_displayImage.HasSources()
			    || true == m_displayImageToggle.HasSources()) {
				vec3 imagePos(tmpOrigin.x()-padding.x()/4,
				              tmpOrigin.y()-padding.x()/4+(m_minSize.y()-m_imageDisplaySize-2*padding.y())/2.0,
				              0);
				vec2 imageSize(m_imageDisplaySize,
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
				tmpTextOrigin.setX(tmpTextOrigin.x() + padding.x()/2 + m_imageDisplaySize);
			}
			
			vec3 drawClippingPos(padding.x(), padding.y(), -0.5);
			vec3 drawClippingSize((m_size.x() - padding.x()),
			                      (m_size.y() - padding.y()),
			                      1);
			
			// clean the element
			m_displayText.Reset();
			m_displayText.SetPos(tmpTextOrigin);
			if(    true == m_displayImage.HasSources()
			    || true == m_displayImageToggle.HasSources()) {
				m_displayText.SetTextAlignement(tmpTextOrigin.x(), tmpTextOrigin.x()+localSize.x()-m_imageDisplaySize, ewol::Text::alignCenter);
			} else {
				m_displayText.SetTextAlignement(tmpTextOrigin.x(), tmpTextOrigin.x()+localSize.x(), ewol::Text::alignCenter);
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
			sizeText = m_displayText.CalculateSize(m_label);
			
			if (true==m_userFill.y()) {
				tmpOrigin.setY(padding.y());
			}
		}
		// selection area :
		m_selectableAreaPos = vec2(tmpOrigin.x()-padding.x(), tmpOrigin.y()-padding.y());
		m_selectableAreaSize = localSize + vec2(2,2)*padding;
		m_shaper.SetOrigin(m_selectableAreaPos );
		m_shaper.SetSize(m_selectableAreaSize);
		m_shaper.SetInsidePos(vec2(tmpTextOrigin.x(), tmpTextOrigin.y()) );
		vec2 tmpp2(sizeText.x(), sizeText.y());
		m_shaper.SetInsideSize(tmpp2);
	}
}


bool widget::Button::OnEventInput(ewol::keyEvent::type_te type, int32_t IdInput, ewol::keyEvent::status_te typeEvent, vec2 pos)
{
	bool previousHoverState = m_mouseHover;
	if(    ewol::keyEvent::statusLeave == typeEvent
	    || ewol::keyEvent::statusAbort == typeEvent) {
		m_mouseHover = false;
		m_buttonPressed = false;
	} else {
		vec2 relativePos = RelativePosition(pos);
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



