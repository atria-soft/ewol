/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/widget/Image.h>
#include <ewol/compositing/Image.h>
#include <ewol/compositing/Drawing.h>
#include <ewol/widget/WidgetManager.h>


extern const char * const ewolEventImagePressed    = "ewol-image-Pressed";

#undef __class__
#define __class__	"Image"


void widget::Image::Init(void)
{
	AddEventId(ewolEventImagePressed);
	
	#ifdef __TARGET_OS__Android
		m_padding.setValue(12,12);
	#else
		m_padding.setValue(4,4);
	#endif
	
	m_textColorBg = draw::color::black;
	m_textColorBg.a = 0x00;
	m_imageSize = 32;
	// Limit event at 1:
	SetMouseLimit(1);
}


widget::Image::Image(etk::UString dataFile, int32_t size)
{
	m_imageSelected = dataFile;
	Init();
	if (size>0) {
		m_imageSize = size;
	}
}


widget::Image::~Image(void)
{
	
}


void widget::Image::SetPadding(vec2 newPadding)
{
	m_padding = newPadding;
}

bool widget::Image::CalculateMinSize(void)
{
	m_minSize.setValue(m_padding.x()*2 + m_imageSize,
	                   m_padding.y()*2 + m_imageSize );
	MarkToRedraw();
	return true;
}


void widget::Image::SetFile(etk::UString newFile)
{
	m_imageSelected = newFile;
	MarkToRedraw();
}


void widget::Image::OnRegenerateDisplay(void)
{
	if (true == NeedRedraw()) {
		// clean the object list ...
		ClearOObjectList();
		
		int32_t tmpSizeX = m_minSize.x();
		int32_t tmpSizeY = m_minSize.y();
		int32_t tmpOriginX = (m_size.x() - m_minSize.x()) / 2;
		int32_t tmpOriginY = (m_size.y() - m_minSize.y()) / 2;
		
		if (true==m_userFill.x()) {
			tmpSizeX = m_size.x();
			tmpOriginX = 0;
		}
		if (true==m_userFill.y()) {
			tmpSizeY = m_size.y();
			tmpOriginY = 0;
		}
		tmpOriginX += m_padding.x();
		tmpOriginY += m_padding.y();
		tmpSizeX -= 2*m_padding.x();
		tmpSizeY -= 2*m_padding.y();
		
		
		ewol::Image * tmpImage = NULL;
		tmpImage = new ewol::Image(m_imageSelected); // TODO : Check if it was possible later : , m_imageSize, m_imageSize);
		tmpImage->SetPos(vec3(tmpOriginX, tmpOriginY, 0) );
		tmpImage->Print(vec2(m_imageSize, m_imageSize));

		
		ewol::Drawing * tmpDraw = new ewol::Drawing();
		tmpDraw->SetColor(m_textColorBg);
		tmpDraw->SetPos(vec3(tmpOriginX, tmpOriginY, 0) );
		tmpDraw->RectangleWidth(vec3(tmpSizeX, tmpSizeY, 0) );
		// add all needed objects ...
		if (NULL != tmpDraw) {
			AddOObject(tmpDraw);
		}
		if (NULL != tmpImage) {
			AddOObject(tmpImage);
		}
	}
}

bool widget::Image::OnEventInput(ewol::keyEvent::type_te type, int32_t IdInput, ewol::keyEvent::status_te typeEvent, vec2 pos)
{
	//EWOL_DEBUG("Event on BT ...");
	if (1 == IdInput) {
		if(    ewol::keyEvent::statusSingle == typeEvent) {
			GenerateEventId(ewolEventImagePressed);
			return true;
		}
	}
	return false;
}

