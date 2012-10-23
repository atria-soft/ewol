/**
 *******************************************************************************
 * @file ewol/widget/ButtonImage.cpp
 * @brief ewol Button Image widget system (Sources)
 * @author Edouard DUPIN
 * @date 06/06/2012
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
 *    You can send me the bug-fix>
 *
 * Term of the licence in in the file licence.txt.
 *
 *******************************************************************************
 */

#include <ewol/widget/ButtonImage.h>

#include <ewol/oObject/OObject.h>
#include <ewol/widget/WidgetManager.h>

/**
 * @brief Initilise the basic widget property ==> due to the android system
 * @note all widget that have template might have this initializer ...
 * @param ---
 * @return ---
 */
void ewol::WIDGET_ButtonImageInit(void)
{
	
}

#undef __class__
#define __class__	"ButtonImage"

ewol::ButtonImage::ButtonImage(etk::UString imageName, draw::Color col)
{
	AddEventId(ewolEventButtonPressed);
	AddEventId(ewolEventButtonDown);
	AddEventId(ewolEventButtonUp);
	AddEventId(ewolEventButtonEnter);
	AddEventId(ewolEventButtonLeave);
	SetMinSize(10,10);
	SetCanHaveFocus(true);
	m_over = false;
	m_down = false;
	m_value = false;
	m_image = imageName;
	m_color = col;
	m_OOImage = NULL;
	m_OOImageBg1 = NULL;
	m_OOImageBG2 = NULL;
	m_resetNeeded = false;
	m_toggleMode = false;
}


ewol::ButtonImage::~ButtonImage(void)
{
	
}

void ewol::ButtonImage::SetImage(etk::UString imageName, draw::Color col)
{
	m_image = imageName;
	m_color = col;
	m_resetNeeded = true;
	MarkToRedraw();
}

void ewol::ButtonImage::SetImageBG(etk::UString imageName, draw::Color col)
{
	m_imageBg1 = imageName;
	m_colorBg1 = col;
	m_resetNeeded = true;
	MarkToRedraw();
}

void ewol::ButtonImage::SetImageSelected(etk::UString imageName, draw::Color col)
{
	m_imageBg2 = imageName;
	m_colorBg2 = col;
	m_resetNeeded = true;
	MarkToRedraw();
}


void ewol::ButtonImage::SetValue(bool val)
{
	if (true == m_toggleMode) {
		m_value = val;
		MarkToRedraw();
	}
}

bool ewol::ButtonImage::GetValue(void)
{
	return m_value;
}

void ewol::ButtonImage::SetToggleMode(bool val)
{
	m_toggleMode = val;
	if (m_toggleMode == false) {
		m_value = false;
	}
	MarkToRedraw();
}

bool ewol::ButtonImage::GetToggleMode(void)
{
	return m_toggleMode;
}


void ewol::ButtonImage::OnRegenerateDisplay(void)
{
	if (true == NeedRedraw()) {
		
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
		
		if (NULL == m_OOImageBG2) {
			if(m_imageBg2 != "") {
				m_OOImageBG2 = new ewol::OObject2DTextured(m_imageBg2, tmpSizeX, tmpSizeY);
			}
		}
		if (NULL == m_OOImageBg1) {
			if(m_imageBg1 != "") {
				m_OOImageBg1 = new ewol::OObject2DTextured(m_imageBg1, tmpSizeX, tmpSizeY);
			}
		}
		if (NULL == m_OOImage) {
			if(m_image != "") {
				m_OOImage = new ewol::OObject2DTextured(m_image, tmpSizeX, tmpSizeY);
			}
		}
		if (false == m_toggleMode) {
			float tmpval = 0.0;
			if (NULL != m_OOImageBG2) {
				m_OOImageBG2->Clear();
				if(    m_down == true
				    || m_over == true ) {
					m_OOImageBG2->Rectangle(tmpOriginX, tmpOriginY, tmpSizeX, tmpSizeY);
				}
				tmpval = tmpSizeX * 0.2;
				tmpSizeX -= tmpval;
				tmpOriginX += tmpval/2;
				tmpval = tmpSizeY * 0.2;
				tmpSizeY -= tmpval;
				tmpOriginY += tmpval/2;
			}
			if (NULL != m_OOImageBg1) {
				m_OOImageBg1->Clear();
				m_OOImageBg1->Rectangle(tmpOriginX, tmpOriginY, tmpSizeX, tmpSizeY);
				tmpval = tmpSizeX * 0.2;
				tmpSizeX -= tmpval;
				tmpOriginX += tmpval/2;
				tmpval = tmpSizeY * 0.2;
				tmpSizeY -= tmpval;
				tmpOriginY += tmpval/2;
			}
			if (NULL != m_OOImage) {
				m_OOImage->Clear();
				m_OOImage->Rectangle(tmpOriginX, tmpOriginY, tmpSizeX, tmpSizeY);
			}
		} else {
			if (NULL != m_OOImage) {
				m_OOImage->Clear();
			}
			if (NULL != m_OOImageBG2) {
				m_OOImageBG2->Clear();
			}
			if(m_value == true) {
				if (NULL != m_OOImage) {
					m_OOImage->Rectangle(tmpOriginX, tmpOriginY, tmpSizeX, tmpSizeY, m_color);
				}
			} else {
				if (NULL != m_OOImageBG2) {
					m_OOImageBG2->Rectangle(tmpOriginX, tmpOriginY, tmpSizeX, tmpSizeY, m_colorBg2);
				}
			}
		}
	}
}

/**
 * @brief Event on an input of this Widget
 * @param[in] type Type of the input (ewol::INPUT_TYPE_MOUSE/ewol::INPUT_TYPE_FINGER ...)
 * @param[in] IdInput Id of the current Input (PC : left=1, right=2, middle=3, none=0 / Tactil : first finger=1 , second=2 (only on this widget, no knowledge at ouside finger))
 * @param[in] typeEvent ewol type of event like EVENT_INPUT_TYPE_DOWN/EVENT_INPUT_TYPE_MOVE/EVENT_INPUT_TYPE_UP/EVENT_INPUT_TYPE_SINGLE/EVENT_INPUT_TYPE_DOUBLE/...
 * @param[in] pos Absolute position of the event
 * @return true the event is used
 * @return false the event is not used
 */
bool ewol::ButtonImage::OnEventInput(ewol::inputType_te type, int32_t IdInput, eventInputType_te typeEvent, Vector2D<float> pos)
{
	//EWOL_DEBUG("Event on BT ...");
	if (1 == IdInput) {
		if(ewol::EVENT_INPUT_TYPE_DOWN == typeEvent) {
			m_down = true;
			if (false == m_toggleMode) {
				m_value = true;
			}
			
			GenerateEventId(ewolEventButtonDown, (m_value)?"true":"false");
			MarkToRedraw();
		}
		if(ewol::EVENT_INPUT_TYPE_UP == typeEvent) {
			m_down = false;
			if (false == m_toggleMode) {
				m_value = false;
			}
			GenerateEventId(ewolEventButtonUp, (m_value)?"true":"false");
			MarkToRedraw();
		}
		if(    ewol::EVENT_INPUT_TYPE_SINGLE == typeEvent
		    || ewol::EVENT_INPUT_TYPE_DOUBLE == typeEvent
		    || ewol::EVENT_INPUT_TYPE_TRIPLE == typeEvent) {
			if (true == m_toggleMode) {
				m_value = (true==m_value) ? false : true;
			}
			GenerateEventId(ewolEventButtonPressed, (m_value)?"true":"false");
			MarkToRedraw();
			return true;
		}
	} else if (0 == IdInput) {
		if(    ewol::EVENT_INPUT_TYPE_ENTER == typeEvent
		    || ewol::EVENT_INPUT_TYPE_MOVE == typeEvent) {
			Vector2D<float> relPos = RelativePosition(pos);
			
			// check if over : 
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
			if(    relPos.x > tmpOriginX
			    && relPos.x < tmpOriginX+tmpSizeX
			    && relPos.y > tmpOriginY
			    && relPos.y < tmpOriginY+tmpSizeY ) {
				if (m_over != true) {
					m_over = true;
					MarkToRedraw();
				}
			}
		} else if(ewol::EVENT_INPUT_TYPE_LEAVE == typeEvent) {
			if(m_over != false) {
				m_over = false;
				MarkToRedraw();
			}
		}
	}
	return false;
}


bool ewol::ButtonImage::OnEventKb(ewol::eventKbType_te typeEvent, uniChar_t unicodeData)
{
	//EWOL_DEBUG("BT PRESSED : \"" << UTF8_data << "\" size=" << strlen(UTF8_data));
	if (unicodeData == '\r') {
		if(typeEvent == ewol::EVENT_KB_TYPE_DOWN) {
			m_down = true;
			if (false == m_toggleMode) {
				m_value = true;
				GenerateEventId(ewolEventButtonEnter, (m_value)?"true":"false");
			}
		} else if(typeEvent == ewol::EVENT_KB_TYPE_UP) {
			m_down = false;
			if (false == m_toggleMode) {
				m_value = false;
				GenerateEventId(ewolEventButtonEnter, (m_value)?"true":"false");
			} else {
				m_value = (true==m_value) ? false : true;
				GenerateEventId(ewolEventButtonEnter, (m_value)?"true":"false");
			}
		}
	}
	return false;
}


void ewol::ButtonImage::OnDraw(DrawProperty& displayProp)
{
	if (NULL != m_OOImageBG2) {
		m_OOImageBG2->Draw();
	}
	if (NULL != m_OOImageBg1) {
		m_OOImageBg1->Draw();
	}
	if (NULL != m_OOImage) {
		m_OOImage->Draw();
	}
}

