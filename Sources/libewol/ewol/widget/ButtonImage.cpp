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

#include <ewol/OObject.h>
#include <ewol/WidgetManager.h>

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

ewol::ButtonImage::ButtonImage(etk::UString imageName, color_ts col)
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
	for (int32_t iii=0; iii<NB_BOUBLE_BUFFER; iii++) {
		m_OOImage[iii] = NULL;
		m_OOImageBg1[iii] = NULL;
		m_OOImageBG2[iii] = NULL;
		m_resetNeeded[iii] = false;
	}
	m_toggleMode = false;
}


ewol::ButtonImage::~ButtonImage(void)
{
	
}

void ewol::ButtonImage::SetImage(etk::UString imageName, color_ts col)
{
	m_image = imageName;
	m_color = col;
	for (int32_t iii=0; iii<NB_BOUBLE_BUFFER; iii++) {
		m_resetNeeded[iii] = true;
	}
	MarkToReedraw();
}

void ewol::ButtonImage::SetImageBG(etk::UString imageName, color_ts col)
{
	m_imageBg1 = imageName;
	m_colorBg1 = col;
	for (int32_t iii=0; iii<NB_BOUBLE_BUFFER; iii++) {
		m_resetNeeded[iii] = true;
	}
	MarkToReedraw();
}

void ewol::ButtonImage::SetImageSelected(etk::UString imageName, color_ts col)
{
	m_imageBg2 = imageName;
	m_colorBg2 = col;
	for (int32_t iii=0; iii<NB_BOUBLE_BUFFER; iii++) {
		m_resetNeeded[iii] = true;
	}
	MarkToReedraw();
}


void ewol::ButtonImage::SetValue(bool val)
{
	if (true == m_toggleMode) {
		m_value = val;
		MarkToReedraw();
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
	MarkToReedraw();
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
		
		if (true==m_userFillX) {
			tmpSizeX = m_size.x;
			tmpOriginX = 0;
		}
		if (true==m_userFillY) {
			tmpSizeY = m_size.y;
			tmpOriginY = 0;
		}
		
		if (NULL == m_OOImageBG2[m_currentCreateId]) {
			if(m_imageBg2 != "") {
				m_OOImageBG2[m_currentCreateId] = new ewol::OObject2DTextured(m_imageBg2, tmpSizeX, tmpSizeY);
			}
		}
		if (NULL == m_OOImageBg1[m_currentCreateId]) {
			if(m_imageBg1 != "") {
				m_OOImageBg1[m_currentCreateId] = new ewol::OObject2DTextured(m_imageBg1, tmpSizeX, tmpSizeY);
			}
		}
		if (NULL == m_OOImage[m_currentCreateId]) {
			if(m_image != "") {
				m_OOImage[m_currentCreateId] = new ewol::OObject2DTextured(m_image, tmpSizeX, tmpSizeY);
			}
		}
		if (false == m_toggleMode) {
			float tmpval = 0.0;
			if (NULL != m_OOImageBG2[m_currentCreateId]) {
				m_OOImageBG2[m_currentCreateId]->Clear();
				if(    m_down == true
				    || m_over == true ) {
					m_OOImageBG2[m_currentCreateId]->Rectangle(tmpOriginX, tmpOriginY, tmpSizeX, tmpSizeY);
				}
				tmpval = tmpSizeX * 0.2;
				tmpSizeX -= tmpval;
				tmpOriginX += tmpval/2;
				tmpval = tmpSizeY * 0.2;
				tmpSizeY -= tmpval;
				tmpOriginY += tmpval/2;
			}
			if (NULL != m_OOImageBg1[m_currentCreateId]) {
				m_OOImageBg1[m_currentCreateId]->Clear();
				m_OOImageBg1[m_currentCreateId]->Rectangle(tmpOriginX, tmpOriginY, tmpSizeX, tmpSizeY);
				tmpval = tmpSizeX * 0.2;
				tmpSizeX -= tmpval;
				tmpOriginX += tmpval/2;
				tmpval = tmpSizeY * 0.2;
				tmpSizeY -= tmpval;
				tmpOriginY += tmpval/2;
			}
			if (NULL != m_OOImage[m_currentCreateId]) {
				m_OOImage[m_currentCreateId]->Clear();
				m_OOImage[m_currentCreateId]->Rectangle(tmpOriginX, tmpOriginY, tmpSizeX, tmpSizeY);
			}
		} else {
			if (NULL != m_OOImage[m_currentCreateId]) {
				m_OOImage[m_currentCreateId]->Clear();
			}
			if (NULL != m_OOImageBG2[m_currentCreateId]) {
				m_OOImageBG2[m_currentCreateId]->Clear();
			}
			if(m_value == true) {
				if (NULL != m_OOImage[m_currentCreateId]) {
					m_OOImage[m_currentCreateId]->Rectangle(tmpOriginX, tmpOriginY, tmpSizeX, tmpSizeY, m_color);
				}
			} else {
				if (NULL != m_OOImageBG2[m_currentCreateId]) {
					m_OOImageBG2[m_currentCreateId]->Rectangle(tmpOriginX, tmpOriginY, tmpSizeX, tmpSizeY, m_colorBg2);
				}
			}
		}
		m_needFlipFlop = true;
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
			MarkToReedraw();
		}
		if(ewol::EVENT_INPUT_TYPE_UP == typeEvent) {
			m_down = false;
			if (false == m_toggleMode) {
				m_value = false;
			}
			GenerateEventId(ewolEventButtonUp, (m_value)?"true":"false");
			MarkToReedraw();
		}
		if(    ewol::EVENT_INPUT_TYPE_SINGLE == typeEvent
		    || ewol::EVENT_INPUT_TYPE_DOUBLE == typeEvent
		    || ewol::EVENT_INPUT_TYPE_TRIPLE == typeEvent) {
			if (true == m_toggleMode) {
				m_value = (true==m_value) ? false : true;
			}
			GenerateEventId(ewolEventButtonPressed, (m_value)?"true":"false");
			MarkToReedraw();
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
			
			if (true==m_userFillX) {
				tmpSizeX = m_size.x;
				tmpOriginX = 0;
			}
			if (true==m_userFillY) {
				tmpSizeY = m_size.y;
				tmpOriginY = 0;
			}
			if(    relPos.x > tmpOriginX
			    && relPos.x < tmpOriginX+tmpSizeX
			    && relPos.y > tmpOriginY
			    && relPos.y < tmpOriginY+tmpSizeY ) {
				if (m_over != true) {
					m_over = true;
					MarkToReedraw();
				}
			}
		} else if(ewol::EVENT_INPUT_TYPE_LEAVE == typeEvent) {
			if(m_over != false) {
				m_over = false;
				MarkToReedraw();
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
	if (NULL != m_OOImageBG2[m_currentDrawId]) {
		m_OOImageBG2[m_currentDrawId]->Draw();
	}
	if (NULL != m_OOImageBg1[m_currentDrawId]) {
		m_OOImageBg1[m_currentDrawId]->Draw();
	}
	if (NULL != m_OOImage[m_currentDrawId]) {
		m_OOImage[m_currentDrawId]->Draw();
	}
}


/**
 * @brief Event generated to inform a flip-flop has occured on the current widget
 * @param ---
 * @return ---
 */
void ewol::ButtonImage::OnFlipFlopEvent(void)
{
	bool needFlipFlop = m_needFlipFlop;
	// call herited classes
	ewol::Widget::OnFlipFlopEvent();
	// internal saving
	if (true == needFlipFlop) {
		if (m_resetNeeded[m_currentCreateId] == true) {
			m_resetNeeded[m_currentCreateId] = false;
			if (NULL != m_OOImageBG2[m_currentCreateId]) {
				delete(m_OOImageBG2[m_currentCreateId]);
				m_OOImageBG2[m_currentCreateId] = NULL;
			}
			if (NULL != m_OOImageBg1[m_currentCreateId]) {
				delete(m_OOImageBg1[m_currentCreateId]);
				m_OOImageBg1[m_currentCreateId] = NULL;
			}
			if (NULL != m_OOImage[m_currentCreateId]) {
				delete(m_OOImage[m_currentCreateId]);
				m_OOImage[m_currentCreateId] = NULL;
			}
		}
	}
}
