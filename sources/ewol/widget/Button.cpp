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

#undef __class__
#define __class__	"Button"


widget::Button::Button(etk::UString newLabel) :
	m_shaper("THEME:GUI:widgetButton.conf")
{
	m_label = newLabel;
	
	AddEventId(ewolEventButtonPressed);
	AddEventId(ewolEventButtonDown);
	AddEventId(ewolEventButtonUp);
	AddEventId(ewolEventButtonEnter);
	AddEventId(ewolEventButtonLeave);
	m_alignement = widget::TEXT_ALIGN_CENTER;
	
	m_textColorFg = draw::color::black;
	
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

void widget::Button::SetValue(bool val)
{
	
}

void widget::Button::SetAlignement(textAlignement_te typeAlign)
{
	m_alignement = typeAlign;
	MarkToRedraw();
}


bool widget::Button::GetValue(void)
{
	return false;
}


void widget::Button::OnDraw(ewol::DrawProperty& displayProp)
{
	m_shaper.Draw();
#warning generate the Toggle
	if (true) {
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
		
		int32_t tmpSizeX = m_minSize.x;
		int32_t tmpSizeY = m_minSize.y;
		etk::Vector3D<float> tmpOrigin((float)((m_size.x - m_minSize.x) / 2.0),
		                               (float)((m_size.y - m_minSize.y) / 2.0),
		                               (float)(0.0));
		// no change for the text orogin : 
		etk::Vector3D<float> tmpTextOrigin((float)((m_size.x - m_minSize.x) / 2.0 + padding.x),
		                                   (float)((m_size.y - m_minSize.y) / 2.0 + padding.y),
		                                   (float)(0.0));
		
		if (true==m_userFill.x) {
			tmpSizeX = m_size.x;
			tmpOrigin.x = 0.0;
			if (m_alignement == widget::TEXT_ALIGN_LEFT) {
				tmpTextOrigin.x = padding.x;
			}
		}
		if (true==m_userFill.y) {
			tmpSizeY = m_size.y;
			tmpOrigin.y = 0.0;
		}
		tmpOrigin.x += padding.x;
		tmpOrigin.x += padding.y;
		tmpSizeX -= 2*padding.x;
		tmpSizeY -= 2*padding.y;
		
		etk::Vector2D<float> textPos(tmpTextOrigin.x, tmpTextOrigin.x);
		
		if(    true == m_displayImage.HasSources()
		    || true == m_displayImageToggle.HasSources()) {
			etk::Vector3D<int32_t> minSize = m_displayText.CalculateSize(m_label);
			etk::Vector3D<int32_t> imagePos(tmpTextOrigin.x-padding.x/4, tmpTextOrigin.y-padding.x/4, 0);
			etk::Vector2D<int32_t> imageSize(minSize.y+padding.x/2, minSize.y+padding.x/2);
			m_displayImage.SetPos(imagePos);
			m_displayImage.Print(imageSize);
			m_displayImageToggle.SetPos(imagePos);
			m_displayImageToggle.Print(imageSize);
			// update the text position ...
			tmpTextOrigin.x += padding.x/2 + minSize.y;
		}
		
		etk::Vector3D<float> drawClippingPos(0.0, 0.0, -0.5);
		etk::Vector3D<float> drawClippingSize((float)(m_size.x - 2*padding.x),
		                                      (float)(m_size.y - 2*padding.y),
		                                      (float)1.0);
		
		// clean the element
		m_displayText.Clear();
		m_displayText.SetClipping(drawClippingPos, drawClippingSize);
		m_displayText.Print(m_label);
		m_displayText.Translate(tmpTextOrigin);
		
		
		m_shaper.SetSize(m_size);
		m_shaper.SetInsidePos(textPos);
		etk::Vector3D<float> tmpp = m_displayText.CalculateSize(m_label);
		etk::Vector2D<float> tmpp2(tmpp.x, tmpp.y);
		m_shaper.SetInsideSize(tmpp2);
		
	}
}


bool widget::Button::OnEventInput(ewol::keyEvent::type_te type, int32_t IdInput, ewol::keyEvent::status_te typeEvent, etk::Vector2D<float> pos)
{
	//EWOL_DEBUG("Event on BT ...");
	if(ewol::keyEvent::statusEnter == typeEvent) {
		ChangeStatusIn(2);
	}else if(ewol::keyEvent::statusLeave == typeEvent) {
		ChangeStatusIn(0);
	}
	if (1 == IdInput) {
		if(ewol::keyEvent::statusDown == typeEvent) {
			GenerateEventId(ewolEventButtonDown);
			ChangeStatusIn(1);
			MarkToRedraw();
		}
		if(ewol::keyEvent::statusUp == typeEvent) {
			GenerateEventId(ewolEventButtonUp);
			ChangeStatusIn(0);
			MarkToRedraw();
		}
		if(ewol::keyEvent::statusSingle == typeEvent) {
			GenerateEventId(ewolEventButtonPressed);
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
