/**
 *******************************************************************************
 * @file ewol/widget/Label.cpp
 * @brief ewol Label widget system (Sources)
 * @author Edouard DUPIN
 * @date 22/12/2011
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

#include <ewol/widget/Label.h>

#include <ewol/oObject/OObject.h>
#include <ewol/widget/WidgetManager.h>


extern const char * const ewolEventLabelPressed    = "ewol Label Pressed";



#undef __class__
#define __class__	"Label"


void ewol::Label::Init(void)
{
	AddEventId(ewolEventLabelPressed);
	m_textColorFg = draw::color::black;
	SetCanHaveFocus(false);
}

ewol::Label::Label(void)
{
	m_label = "---";
	Init();
}

ewol::Label::Label(etk::UString newLabel)
{
	m_label = newLabel;
	Init();
}


ewol::Label::~Label(void)
{
	
}


bool ewol::Label::CalculateMinSize(void)
{
	Vector2D<int32_t> minSize = m_oObjectText.GetSize(m_label);
	m_minSize.x = 3 + minSize.x;
	m_minSize.y = 3 + minSize.y;
	return true;
}


void ewol::Label::SetLabel(etk::UString newLabel)
{
	m_label = newLabel;
	MarkToRedraw();
}

void ewol::Label::OnDraw(DrawProperty& displayProp)
{
	m_oObjectText.Draw();
}


void ewol::Label::OnRegenerateDisplay(void)
{
	if (true == NeedRedraw()) {
		int32_t paddingSize = 3;
		
		int32_t tmpOriginX = 0;
		int32_t tmpOriginY = 0;
		
		if (true==m_userFillX) {
			tmpOriginX = (m_size.x - m_minSize.x) / 2;
		}
		if (true==m_userFillY) {
			tmpOriginY = (m_size.y - m_minSize.y) / 2;
		}
		tmpOriginX += paddingSize;
		tmpOriginY += paddingSize;
		
		
		Vector2D<float> textPos(tmpOriginX, tmpOriginY);
		clipping_ts drawClipping;
		drawClipping.x = paddingSize;
		drawClipping.y = paddingSize;
		drawClipping.w = m_size.x - 2*paddingSize;
		drawClipping.h = m_size.y - 2*paddingSize;
		m_oObjectText.Text(textPos/*, drawClipping*/, m_label);
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
bool ewol::Label::OnEventInput(ewol::inputType_te type, int32_t IdInput, eventInputType_te typeEvent, Vector2D<float> pos)
{
	//EWOL_DEBUG("Event on Label ...");
	if (1 == IdInput) {
		if (ewol::EVENT_INPUT_TYPE_SINGLE == typeEvent) {
			// nothing to do ...
			GenerateEventId(ewolEventLabelPressed);
			return true;
		}
	}
	return false;
}


