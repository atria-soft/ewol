/**
 *******************************************************************************
 * @file ewol/widget/Button.cpp
 * @brief ewol Button widget system (Sources)
 * @author Edouard DUPIN
 * @date 07/11/2011
 * @par Project
 * ewol
 *
 * @par Copyright
 * Copyright 2011 Edouard DUPIN, all right reserved
 *
 * This software is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY.
 *
 * Licence summary : 
 *    You can modify and redistribute the sources code and binaries.
 *    You can send me the bug-fix
 *
 * Term of the licence in in the file licence.txt.
 *
 *******************************************************************************
 */

#include <ewol/widget/Button.h>

#include <ewol/OObject.h>
#include <ewol/WidgetManager.h>


extern const char * const ewolEventButtonPressed    = "ewol-button-Pressed";
extern const char * const ewolEventButtonDown       = "ewol-button-down";
extern const char * const ewolEventButtonUp         = "ewol-button-up";
extern const char * const ewolEventButtonEnter      = "ewol-button-enter";
extern const char * const ewolEventButtonLeave      = "ewol-button-leave";


/**
 * @brief Initilise the basic widget property ==> due to the android system
 * @note all widget that have template might have this initializer ...
 * @param ---
 * @return ---
 */
void ewol::WIDGET_ButtonInit(void)
{
	
}

#undef __class__
#define __class__	"Button"


void ewol::Button::Init(void)
{
	AddEventId(ewolEventButtonPressed);
	AddEventId(ewolEventButtonDown);
	AddEventId(ewolEventButtonUp);
	AddEventId(ewolEventButtonEnter);
	AddEventId(ewolEventButtonLeave);
	m_hasAnImage = false;
	m_alignement = ewol::TEXT_ALIGN_CENTER;
	
	#ifdef __PLATFORM__Android
		m_padding.y = 12;
		m_padding.x = 12;
	#else
		m_padding.y = 4;
		m_padding.x = 4;
	#endif
	
	m_textColorFg = etk::color::color_Black;
	
	m_textColorBg = etk::color::color_Black;
	m_textColorBg.alpha = 0x3F;
	SetCanHaveFocus(true);
}

ewol::Button::Button(void)
{
	m_label = "No Label";
	Init();
}

ewol::Button::Button(etk::UString newLabel)
{
	m_label = newLabel;
	Init();
}


ewol::Button::~Button(void)
{
	
}

void ewol::Button::SetImage(etk::UString imageName)
{
	if (imageName == "") {
		m_hasAnImage = false;
	} else {
		m_imageSelected = imageName;
		m_hasAnImage = true;
	}
}

//!< EObject name :
extern const char * const ewol::TYPE_EOBJECT_WIDGET_BUTTON = "Button";

/**
 * @brief Check if the object has the specific type.
 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
 * @param[in] objectType type of the object we want to check
 * @return true if the object is compatible, otherwise false
 */
bool ewol::Button::CheckObjectType(const char * const objectType)
{
	if (NULL == objectType) {
		EWOL_ERROR("check error : \"" << ewol::TYPE_EOBJECT_WIDGET_BUTTON << "\" != NULL(pointer) ");
		return false;
	}
	if (objectType == ewol::TYPE_EOBJECT_WIDGET_BUTTON) {
		return true;
	} else {
		if(true == ewol::Drawable::CheckObjectType(objectType)) {
			return true;
		}
		EWOL_ERROR("check error : \"" << ewol::TYPE_EOBJECT_WIDGET_BUTTON << "\" != \"" << objectType << "\"");
		return false;
	}
}

/**
 * @brief Get the current Object type of the EObject
 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
 * @param[in] objectType type description
 * @return true if the object is compatible, otherwise false
 */
const char * const ewol::Button::GetObjectType(void)
{
	return ewol::TYPE_EOBJECT_WIDGET_BUTTON;
}


void ewol::Button::SetPadding(coord2D_ts newPadding)
{
	m_padding = newPadding;
}

bool ewol::Button::CalculateMinSize(void)
{
	int32_t fontId = GetDefaultFontId();
	int32_t minWidth = ewol::GetWidth(fontId, m_label);
	int32_t minHeight = ewol::GetHeight(fontId);
	m_minSize.x = m_padding.x*2 + minWidth;
	m_minSize.y = m_padding.y*2 + minHeight;
	// Add the image element ...
	if (true == m_hasAnImage) {
		//m_minSize.x += -m_padding.x + m_padding.y*2 + minHeight;
		//m_minSize.y += m_padding.y*2;
		m_minSize.x += m_padding.x + minHeight;
	}
	
	MarkToReedraw();
	return true;
}


void ewol::Button::SetLabel(etk::UString newLabel)
{
	m_label = newLabel;
}

void ewol::Button::SetValue(bool val)
{
	
}

void ewol::Button::SetAlignement(textAlignement_te typeAlign)
{
	m_alignement = typeAlign;
	MarkToReedraw();
}


bool ewol::Button::GetValue(void)
{
	return false;
}


void ewol::Button::OnRegenerateDisplay(void)
{
	if (true == NeedRedraw()) {
		// clean the object list ...
		ClearOObjectList();
		
		int32_t tmpSizeX = m_minSize.x;
		int32_t tmpSizeY = m_minSize.y;
		int32_t tmpOriginX = (m_size.x - m_minSize.x) / 2;
		int32_t tmpOriginY = (m_size.y - m_minSize.y) / 2;
		// no change for the text orogin : 
		int32_t tmpTextOriginX = (m_size.x - m_minSize.x) / 2 + m_padding.x;
		int32_t tmpTextOriginY = (m_size.y - m_minSize.y) / 2 + m_padding.y;
		
		if (true==m_userFillX) {
			tmpSizeX = m_size.x;
			tmpOriginX = 0;
			if (m_alignement == ewol::TEXT_ALIGN_LEFT) {
				tmpTextOriginX = m_padding.x;
			}
		}
		if (true==m_userFillY) {
			tmpSizeY = m_size.y;
			tmpOriginY = 0;
		}
		tmpOriginX += m_padding.x;
		tmpOriginY += m_padding.y;
		tmpSizeX -= 2*m_padding.x;
		tmpSizeY -= 2*m_padding.y;
		
		ewol::OObject2DText * tmpText = new ewol::OObject2DText("", -1, m_textColorFg);
		/*
		int32_t fontId = GetDefaultFontId();
		int32_t fontHeight = ewol::GetHeight(fontId);
		int32_t fontWidth = ewol::GetWidth(fontId, m_label.c_str());
		*/
		coord2D_ts textPos;
		textPos.x = tmpTextOriginX;
		textPos.y = tmpTextOriginY;
		
		ewol::OObject2DTextured * tmpImage = NULL;
		if (true == m_hasAnImage) {
			int32_t fontId = GetDefaultFontId();
			int32_t fontHeight = ewol::GetHeight(fontId);
			tmpImage = new ewol::OObject2DTextured(m_imageSelected, fontHeight, fontHeight);
			tmpImage->Rectangle(textPos.x, textPos.y, fontHeight, fontHeight);
			// update the text position ...
			textPos.x += m_padding.x + fontHeight;
		}
		clipping_ts drawClipping;
		drawClipping.x = m_padding.x;
		drawClipping.y = m_padding.y;
		drawClipping.w = m_size.x - 2*m_padding.x;
		drawClipping.h = m_size.y - 2*m_padding.y;
		tmpText->Text(textPos, drawClipping, m_label);
		
		ewol::OObject2DColored * tmpOObjects = new ewol::OObject2DColored;
		tmpOObjects->SetColor(m_textColorBg);
		tmpOriginX -= m_padding.x/2;
		tmpOriginY -= m_padding.y/2;
		tmpSizeX += m_padding.x/1;
		tmpSizeY += m_padding.y/1;
		tmpOObjects->Rectangle( tmpOriginX, tmpOriginY, tmpSizeX, tmpSizeY);
		// add all needed objects ...
		if (NULL != tmpOObjects) {
			AddOObject(tmpOObjects);
		}
		if (NULL != tmpImage) {
			AddOObject(tmpImage);
		}
		if (NULL != tmpText) {
			AddOObject(tmpText);
		}
	}
}

/**
 * @brief Event on an input of this Widget
 * @param[in] IdInput Id of the current Input (PC : left=1, right=2, middle=3, none=0 / Tactil : first finger=1 , second=2 (only on this widget, no knowledge at ouside finger))
 * @param[in] typeEvent ewol type of event like EVENT_INPUT_TYPE_DOWN/EVENT_INPUT_TYPE_MOVE/EVENT_INPUT_TYPE_UP/EVENT_INPUT_TYPE_SINGLE/EVENT_INPUT_TYPE_DOUBLE/...
 * @param[in] pos Absolute position of the event
 * @return true the event is used
 * @return false the event is not used
 */
bool ewol::Button::OnEventInput(int32_t IdInput, eventInputType_te typeEvent, coord2D_ts pos)
{
	//EWOL_DEBUG("Event on BT ...");
	if (1 == IdInput) {
		if(ewol::EVENT_INPUT_TYPE_DOWN == typeEvent) {
			GenerateEventId(ewolEventButtonDown);
		}
		if(ewol::EVENT_INPUT_TYPE_UP == typeEvent) {
			GenerateEventId(ewolEventButtonUp);
		}
		if(    ewol::EVENT_INPUT_TYPE_SINGLE == typeEvent
		    || ewol::EVENT_INPUT_TYPE_DOUBLE == typeEvent
		    || ewol::EVENT_INPUT_TYPE_TRIPLE == typeEvent) {
			GenerateEventId(ewolEventButtonPressed);
			MarkToReedraw();
			return true;
		}
	}
	return false;
}


bool ewol::Button::OnEventKb(ewol::eventKbType_te typeEvent, uniChar_t unicodeData)
{
	//EWOL_DEBUG("BT PRESSED : \"" << UTF8_data << "\" size=" << strlen(UTF8_data));
	if(    typeEvent == ewol::EVENT_KB_TYPE_DOWN
	    && unicodeData == '\r') {
		GenerateEventId(ewolEventButtonEnter);
	}
	return false;
}

