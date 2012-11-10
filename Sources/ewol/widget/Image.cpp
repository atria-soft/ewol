/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/widget/Image.h>
#include <ewol/oObject/OObject.h>
#include <ewol/widget/WidgetManager.h>


extern const char * const ewolEventImagePressed    = "ewol-image-Pressed";


void ewol::WIDGET_ImageInit(void)
{
	
}

#undef __class__
#define __class__	"Image"


void ewol::Image::Init(void)
{
	AddEventId(ewolEventImagePressed);
	
	#ifdef __TARGET_OS__Android
		m_padding.y = 12;
		m_padding.x = 12;
	#else
		m_padding.y = 4;
		m_padding.x = 4;
	#endif
	
	m_textColorBg = draw::color::black;
	m_textColorBg.a = 0x00;
	m_imageSize = 32;
}


ewol::Image::Image(etk::UString dataFile, int32_t size)
{
	m_imageSelected = dataFile;
	Init();
	if (size>0) {
		m_imageSize = size;
	}
}


ewol::Image::~Image(void)
{
	
}


void ewol::Image::SetPadding(etk::Vector2D<float> newPadding)
{
	m_padding = newPadding;
}

bool ewol::Image::CalculateMinSize(void)
{
	m_minSize.x = m_padding.x*2 + m_imageSize;
	m_minSize.y = m_padding.y*2 + m_imageSize;
	MarkToRedraw();
	return true;
}


void ewol::Image::SetFile(etk::UString newFile)
{
	m_imageSelected = newFile;
	MarkToRedraw();
}


void ewol::Image::OnRegenerateDisplay(void)
{
	if (true == NeedRedraw()) {
		// clean the object list ...
		ClearOObjectList();
		
		int32_t tmpSizeX = m_minSize.x;
		int32_t tmpSizeY = m_minSize.y;
		int32_t tmpOriginX = (m_size.x - m_minSize.x) / 2;
		int32_t tmpOriginY = (m_size.y - m_minSize.y) / 2;
		
		if (true==m_userFill.x) {
			tmpSizeX = m_size.x;
			tmpOriginX = 0;
		}
		if (true==m_userFill.y) {
			tmpSizeY = m_size.y;
			tmpOriginY = 0;
		}
		tmpOriginX += m_padding.x;
		tmpOriginY += m_padding.y;
		tmpSizeX -= 2*m_padding.x;
		tmpSizeY -= 2*m_padding.y;
		
		
		ewol::OObject2DTextured * tmpImage = NULL;
		tmpImage = new ewol::OObject2DTextured(m_imageSelected, m_imageSize, m_imageSize);
		tmpImage->Rectangle(tmpOriginX, tmpOriginY, m_imageSize, m_imageSize);

		
		ewol::OObject2DColored * tmpOObjects = new ewol::OObject2DColored;
		tmpOObjects->SetColor(m_textColorBg);
		tmpOObjects->Rectangle( tmpOriginX, tmpOriginY, tmpSizeX, tmpSizeY);
		// add all needed objects ...
		if (NULL != tmpOObjects) {
			AddOObject(tmpOObjects);
		}
		if (NULL != tmpImage) {
			AddOObject(tmpImage);
		}
	}
}

bool ewol::Image::OnEventInput(ewol::inputType_te type, int32_t IdInput, eventInputType_te typeEvent, etk::Vector2D<float> pos)
{
	//EWOL_DEBUG("Event on BT ...");
	if (1 == IdInput) {
		if(    ewol::EVENT_INPUT_TYPE_SINGLE == typeEvent
		    || ewol::EVENT_INPUT_TYPE_DOUBLE == typeEvent
		    || ewol::EVENT_INPUT_TYPE_TRIPLE == typeEvent) {
			GenerateEventId(ewolEventImagePressed);
			return true;
		}
	}
	return false;
}

