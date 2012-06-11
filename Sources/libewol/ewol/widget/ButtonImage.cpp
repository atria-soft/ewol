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
 *    You can send me the bug-fix
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

ewol::ButtonImage::ButtonImage(etk::UString newLabel)
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
	for (int32_t iii=0; iii<NB_BOUBLE_BUFFER; iii++) {
		m_OOImage[iii] = NULL;
		m_OOImageBg1[iii] = NULL;
		m_OOImageBG2[iii] = NULL;
		m_resetNeeded[iii] = false;
	}
}


ewol::ButtonImage::~ButtonImage(void)
{
	
}

void ewol::ButtonImage::SetImage(etk::UString imageName)
{
	m_image = imageName;
	for (int32_t iii=0; iii<NB_BOUBLE_BUFFER; iii++) {
		m_resetNeeded[iii] = true;
	}
	MarkToReedraw();
}

void ewol::ButtonImage::SetImageBG(etk::UString imageName)
{
	m_imageBg1 = imageName;
	for (int32_t iii=0; iii<NB_BOUBLE_BUFFER; iii++) {
		m_resetNeeded[iii] = true;
	}
	MarkToReedraw();
}

void ewol::ButtonImage::SetImageSelected(etk::UString imageName)
{
	m_imageBg2 = imageName;
	for (int32_t iii=0; iii<NB_BOUBLE_BUFFER; iii++) {
		m_resetNeeded[iii] = true;
	}
	MarkToReedraw();
}

//!< EObject name :
extern const char * const ewol::TYPE_EOBJECT_WIDGET_BUTTON_IMAGE = "ButtonImage";

/**
 * @brief Check if the object has the specific type.
 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
 * @param[in] objectType type of the object we want to check
 * @return true if the object is compatible, otherwise false
 */
bool ewol::ButtonImage::CheckObjectType(const char * const objectType)
{
	if (NULL == objectType) {
		EWOL_ERROR("check error : \"" << ewol::TYPE_EOBJECT_WIDGET_BUTTON_IMAGE << "\" != NULL(pointer) ");
		return false;
	}
	if (objectType == ewol::TYPE_EOBJECT_WIDGET_BUTTON_IMAGE) {
		return true;
	} else {
		if(true == ewol::Widget::CheckObjectType(objectType)) {
			return true;
		}
		EWOL_ERROR("check error : \"" << ewol::TYPE_EOBJECT_WIDGET_BUTTON_IMAGE << "\" != \"" << objectType << "\"");
		return false;
	}
}

/**
 * @brief Get the current Object type of the EObject
 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
 * @param[in] objectType type description
 * @return true if the object is compatible, otherwise false
 */
const char * const ewol::ButtonImage::GetObjectType(void)
{
	return ewol::TYPE_EOBJECT_WIDGET_BUTTON_IMAGE;
}


void ewol::ButtonImage::SetValue(bool val)
{
	
}

bool ewol::ButtonImage::GetValue(void)
{
	return false;
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
		
		if (NULL != m_OOImageBG2[m_currentCreateId]) {
			m_OOImageBG2[m_currentCreateId]->Clear();
			if(    m_down == true
			    || m_over == true ) {
				m_OOImageBG2[m_currentCreateId]->Rectangle(tmpOriginX, tmpOriginY, tmpSizeX, tmpSizeY);
			}
		}
		float tmpval = tmpSizeX * 0.2;
		tmpSizeX -= tmpval;
		tmpOriginX += tmpval/2;
		tmpval = tmpSizeY * 0.2;
		tmpSizeY -= tmpval;
		tmpOriginY += tmpval/2;
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
			GenerateEventId(ewolEventButtonDown);
			m_down = true;
			m_value = true;
			MarkToReedraw();
		}
		if(ewol::EVENT_INPUT_TYPE_UP == typeEvent) {
			GenerateEventId(ewolEventButtonUp);
			m_down = false;
			m_value = false;
			MarkToReedraw();
		}
		if(    ewol::EVENT_INPUT_TYPE_SINGLE == typeEvent
		    || ewol::EVENT_INPUT_TYPE_DOUBLE == typeEvent
		    || ewol::EVENT_INPUT_TYPE_TRIPLE == typeEvent) {
			GenerateEventId(ewolEventButtonPressed);
			MarkToReedraw();
			return true;
		}
	} else if (0 == IdInput) {
		if(ewol::EVENT_INPUT_TYPE_MOVE == typeEvent) {
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
			} else {
				if(m_over != false) {
					m_over = false;
					MarkToReedraw();
				}
			}
		}
	}
	return false;
}


bool ewol::ButtonImage::OnEventKb(ewol::eventKbType_te typeEvent, uniChar_t unicodeData)
{
	//EWOL_DEBUG("BT PRESSED : \"" << UTF8_data << "\" size=" << strlen(UTF8_data));
	if(    typeEvent == ewol::EVENT_KB_TYPE_DOWN
	    && unicodeData == '\r') {
		GenerateEventId(ewolEventButtonEnter);
	}
	return false;
}


void ewol::ButtonImage::OnDraw(void)
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
			m_resetNeeded[m_currentCreateId] == false;
			
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
