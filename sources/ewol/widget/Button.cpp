/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */


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


widget::Button::Button(etk::UString newLabel) :
	m_shaper("THEME:GUI:widgetButton.conf"),
	m_label(newLabel),
	m_toggleMode(false),
	m_value(false)
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

void widget::Button::SetImage(etk::UString imageName)
{
	m_displayImage.SetSource(imageName);
	MarkToRedraw();
}

void widget::Button::SetImageToggle(etk::UString imageName)
{
	m_displayImageToggle.SetSource(imageName);
	MarkToRedraw();
}


bool widget::Button::CalculateMinSize(void)
{
	etk::Vector2D<float> padding = m_shaper.GetPadding();
	etk::Vector3D<int32_t> minSize = m_displayText.CalculateSize(m_label);
	if(    true == m_toggleMode
	    && m_labelToggle.Size()!=0) {
		etk::Vector3D<int32_t> minSizeToggle = m_displayText.CalculateSize(m_labelToggle);
		minSize.x = etk_max(minSize.x, minSizeToggle.x);
		minSize.y = etk_max(minSize.y, minSizeToggle.y);
		minSize.z = etk_max(minSize.z, minSizeToggle.z);
	}
	m_minSize.x = padding.x*2 + minSize.x;
	m_minSize.y = padding.y*2 + minSize.y;
	// Add the image element ...
	if(    true == m_displayImage.HasSources()
	    || true == m_displayImageToggle.HasSources()) {
		m_minSize.x += padding.x + minSize.y;
	}
	MarkToRedraw();
	return true;
}


void widget::Button::SetLabel(etk::UString newLabel)
{
	m_label = newLabel;
	MarkToRedraw();
}

etk::UString widget::Button::GetLabel(void)
{
	return m_label;
}

void widget::Button::SetLabelToggle(etk::UString newLabel)
{
	m_labelToggle = newLabel;
	MarkToRedraw();
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
#warning generate the Toggle
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
		
		m_displayImage.Clear();
		m_displayImageToggle.Clear();
		m_shaper.Clear();
		
		etk::Vector2D<int32_t> localSize = m_minSize;
		
		etk::Vector3D<float> tmpOrigin((float)((m_size.x - m_minSize.x) / 2.0),
		                               (float)((m_size.y - m_minSize.y) / 2.0),
		                               (float)(0.0));
		// no change for the text orogin : 
		etk::Vector3D<float> tmpTextOrigin((float)((m_size.x - m_minSize.x) / 2.0 + padding.x),
		                                   (float)((m_size.y - m_minSize.y) / 2.0 + padding.y),
		                                   (float)(0.0));
		
		if (true==m_userFill.x) {
			localSize.x = m_size.x;
			tmpOrigin.x = 0.0;
		}
		if (true==m_userFill.y) {
			localSize.y = m_size.y;
			tmpOrigin.y = 0.0;
		}
		tmpOrigin.x += padding.x;
		tmpOrigin.y += padding.y;
		localSize.x -= 2*padding.x;
		localSize.y -= 2*padding.y;
		
		etk::Vector2D<float> textPos(tmpTextOrigin.x, tmpTextOrigin.x);
		
		if(    true == m_displayImage.HasSources()
		    || true == m_displayImageToggle.HasSources()) {
			etk::Vector3D<int32_t> minSize = m_displayText.CalculateSize(m_label);
			etk::Vector3D<int32_t> imagePos(tmpTextOrigin.x-padding.x/4, tmpTextOrigin.y-padding.x/4, 0);
			etk::Vector2D<int32_t> imageSize(minSize.y+padding.x/2, minSize.y+padding.x/2);
			if(    false==m_toggleMode
			    || false==m_value) {
				m_displayImage.SetPos(imagePos);
				m_displayImage.Print(imageSize);
			} else {
				m_displayImageToggle.SetPos(imagePos);
				m_displayImageToggle.Print(imageSize);
			}
			// update the text position ...
			tmpTextOrigin.x += padding.x/2 + minSize.y;
		}
		
		etk::Vector3D<float> drawClippingPos(0.0, 0.0, -0.5);
		etk::Vector3D<float> drawClippingSize((float)(m_size.x - 2*padding.x),
		                                      (float)(m_size.y - 2*padding.y),
		                                      (float)1.0);
		
		// clean the element
		m_displayText.Clear();
		m_displayText.SetTextAlignement(0, localSize.x + 2*padding.x);
		m_displayText.SetClipping(drawClippingPos, drawClippingSize);
		if(    false == m_toggleMode
		    || false == m_value) {
			m_displayText.PrintDecorated(m_label);
		} else {
			m_displayText.PrintDecorated(m_labelToggle);
		}
		m_displayText.Translate(tmpOrigin);
		
		//m_shaper.SetOrigin(etk::Vector2D<float>(tmpTextOrigin.x-padding.x, tmpTextOrigin.y-padding.y) );
		localSize.x += 2*padding.x;
		localSize.y += 2*padding.y;
		m_shaper.SetSize(localSize);
		m_shaper.SetInsidePos(etk::Vector2D<float>(tmpTextOrigin.x, tmpTextOrigin.y) );
		etk::Vector3D<float> tmpp = m_displayText.CalculateSize(m_label);
		etk::Vector2D<float> tmpp2(tmpp.x, tmpp.y);
		m_shaper.SetInsideSize(tmpp2);
		
	}
}


bool widget::Button::OnEventInput(ewol::keyEvent::type_te type, int32_t IdInput, ewol::keyEvent::status_te typeEvent, etk::Vector2D<float> pos)
{
	//EWOL_DEBUG("Event on BT ...");
	if(ewol::keyEvent::statusEnter == typeEvent) {
		ChangeStatusIn(STATUS_HOVER);
	}else if(ewol::keyEvent::statusLeave == typeEvent) {
		ChangeStatusIn(STATUS_UP);
	}
	if (1 == IdInput) {
		if(ewol::keyEvent::statusDown == typeEvent) {
			GenerateEventId(ewolEventButtonDown);
			ChangeStatusIn(STATUS_PRESSED);
			MarkToRedraw();
		}
		if(ewol::keyEvent::statusUp == typeEvent) {
			GenerateEventId(ewolEventButtonUp);
			ChangeStatusIn(STATUS_UP);
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
			return true;
		}
	}
	return false;
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
