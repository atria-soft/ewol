/**
 *******************************************************************************
 * @file ewol/widgetMeta/Keyboard.cpp
 * @brief ewol keyboard meta widget system (Sources)
 * @author Edouard DUPIN
 * @date 07/01/2012
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

#include <ewol/widgetMeta/Keyboard.h>
#include <ewol/widget/Button.h>
#include <ewol/widget/SizerHori.h>
#include <ewol/widget/SizerVert.h>
#include <ewol/widget/Entry.h>
#include <ewol/widget/List.h>
#include <ewol/widget/Spacer.h>
#include <ewol/widget/Label.h>
#include <ewol/WidgetManager.h>
#include <etk/VectorType.h>
#include <ewol/ewol.h>

#include <ewol/base/gui.h>

extern "C" {
	// file browsing ...
	#include <dirent.h>
}

#undef __class__
#define __class__	"ewol::Keyboard"


extern const char * const ewolEventKeyboardHide   = "ewol event Keyboard request hide";
extern const char * const ewolEventKeyEvent = "ewol event internal key event";


ewol::Keyboard::Keyboard(void)
{
	/*
	// set event that can propagate outside ....
	AddEventId(ewolEventKeyboardHide);
	
	m_mode = KEYBOARD_MODE_TEXT;
	m_isHide = true;
	SetMode(m_mode);
	*/
}


ewol::Keyboard::~Keyboard(void)
{
	/*
	if (NULL != m_subWidget) {
		ewol::widgetManager::MarkWidgetToBeRemoved(m_subWidget);
		m_subWidget = NULL;
	}
	*/
}

#define ADD_BUTTON(upperWidget,widget,text,event)    do { \
                                                         (widget) = new ewol::Button((text)); \
                                                         (widget)->RegisterOnEvent(this, ewolEventButtonPressed, (event) ); \
                                                         (widget)->SetExpendX(true); \
                                                         (widget)->SetFillX(true); \
                                                         (widget)->SetCanHaveFocus(false); \
                                                         (widget)->SetPadding(newPadding); \
                                                         (upperWidget)->SubWidgetAdd((widget)); \
                                                     } while (0)



void ewol::Keyboard::SetMode(keyboardMode_te mode)
{
	ewol::SizerVert * mySizerVert = NULL;
	ewol::SizerHori * mySizerHori = NULL;
	ewol::Button * myButton = NULL;
	
	coord2D_ts newPadding;
	newPadding.y = 20;
	newPadding.x = 12;
	
	mySizerVert = new ewol::SizerVert();
		m_subWidget[m_currentCreateId] = mySizerVert;
		
		mySizerHori = new ewol::SizerHori();
			mySizerVert->SubWidgetAdd(mySizerHori);
			ADD_BUTTON(mySizerHori,myButton,"1",ewolEventKeyEvent);
			ADD_BUTTON(mySizerHori,myButton,"2",ewolEventKeyEvent);
			ADD_BUTTON(mySizerHori,myButton,"3",ewolEventKeyEvent);
			ADD_BUTTON(mySizerHori,myButton,"4",ewolEventKeyEvent);
			ADD_BUTTON(mySizerHori,myButton,"5",ewolEventKeyEvent);
			ADD_BUTTON(mySizerHori,myButton,"6",ewolEventKeyEvent);
			ADD_BUTTON(mySizerHori,myButton,"7",ewolEventKeyEvent);
			ADD_BUTTON(mySizerHori,myButton,"8",ewolEventKeyEvent);
			ADD_BUTTON(mySizerHori,myButton,"9",ewolEventKeyEvent);
			ADD_BUTTON(mySizerHori,myButton,"0",ewolEventKeyEvent);
			ADD_BUTTON(mySizerHori,myButton,"(",ewolEventKeyEvent);
			ADD_BUTTON(mySizerHori,myButton,")",ewolEventKeyEvent);
			ADD_BUTTON(mySizerHori,myButton,"{",ewolEventKeyEvent);
			ADD_BUTTON(mySizerHori,myButton,"}",ewolEventKeyEvent);
			ADD_BUTTON(mySizerHori,myButton,"+",ewolEventKeyEvent);
			ADD_BUTTON(mySizerHori,myButton,"=",ewolEventKeyEvent);
			ADD_BUTTON(mySizerHori,myButton,"DEL",ewolEventKeyEvent);
			ADD_BUTTON(mySizerHori,myButton,"SUPR",ewolEventKeyEvent);
		mySizerHori = new ewol::SizerHori();
			mySizerVert->SubWidgetAdd(mySizerHori);
			ADD_BUTTON(mySizerHori,myButton,"TAB",ewolEventKeyEvent);
			ADD_BUTTON(mySizerHori,myButton,"a",ewolEventKeyEvent);
			ADD_BUTTON(mySizerHori,myButton,"z",ewolEventKeyEvent);
			ADD_BUTTON(mySizerHori,myButton,"e",ewolEventKeyEvent);
			ADD_BUTTON(mySizerHori,myButton,"r",ewolEventKeyEvent);
			ADD_BUTTON(mySizerHori,myButton,"t",ewolEventKeyEvent);
			ADD_BUTTON(mySizerHori,myButton,"y",ewolEventKeyEvent);
			ADD_BUTTON(mySizerHori,myButton,"u",ewolEventKeyEvent);
			ADD_BUTTON(mySizerHori,myButton,"i",ewolEventKeyEvent);
			ADD_BUTTON(mySizerHori,myButton,"o",ewolEventKeyEvent);
			ADD_BUTTON(mySizerHori,myButton,"p",ewolEventKeyEvent);
			ADD_BUTTON(mySizerHori,myButton,"$",ewolEventKeyEvent);
			ADD_BUTTON(mySizerHori,myButton,"ENTER",ewolEventKeyEvent);
		mySizerHori = new ewol::SizerHori();
			mySizerVert->SubWidgetAdd(mySizerHori);
			ADD_BUTTON(mySizerHori,myButton,"CAPLOCK",ewolEventKeyEvent);
			ADD_BUTTON(mySizerHori,myButton,"q",ewolEventKeyEvent);
			ADD_BUTTON(mySizerHori,myButton,"s",ewolEventKeyEvent);
			ADD_BUTTON(mySizerHori,myButton,"d",ewolEventKeyEvent);
			ADD_BUTTON(mySizerHori,myButton,"f",ewolEventKeyEvent);
			ADD_BUTTON(mySizerHori,myButton,"g",ewolEventKeyEvent);
			ADD_BUTTON(mySizerHori,myButton,"h",ewolEventKeyEvent);
			ADD_BUTTON(mySizerHori,myButton,"j",ewolEventKeyEvent);
			ADD_BUTTON(mySizerHori,myButton,"k",ewolEventKeyEvent);
			ADD_BUTTON(mySizerHori,myButton,"l",ewolEventKeyEvent);
			ADD_BUTTON(mySizerHori,myButton,"m",ewolEventKeyEvent);
			ADD_BUTTON(mySizerHori,myButton,"%",ewolEventKeyEvent);
			ADD_BUTTON(mySizerHori,myButton,"*",ewolEventKeyEvent);
			ADD_BUTTON(mySizerHori,myButton,"ENTER",ewolEventKeyEvent);
		mySizerHori = new ewol::SizerHori();
			mySizerVert->SubWidgetAdd(mySizerHori);
			ADD_BUTTON(mySizerHori,myButton,"SHIFT",ewolEventKeyEvent);
			ADD_BUTTON(mySizerHori,myButton,"<",ewolEventKeyEvent);
			ADD_BUTTON(mySizerHori,myButton,">",ewolEventKeyEvent);
			ADD_BUTTON(mySizerHori,myButton,"w",ewolEventKeyEvent);
			ADD_BUTTON(mySizerHori,myButton,"x",ewolEventKeyEvent);
			ADD_BUTTON(mySizerHori,myButton,"c",ewolEventKeyEvent);
			ADD_BUTTON(mySizerHori,myButton,"v",ewolEventKeyEvent);
			ADD_BUTTON(mySizerHori,myButton,"b",ewolEventKeyEvent);
			ADD_BUTTON(mySizerHori,myButton,"n",ewolEventKeyEvent);
			ADD_BUTTON(mySizerHori,myButton,",",ewolEventKeyEvent);
			ADD_BUTTON(mySizerHori,myButton,";",ewolEventKeyEvent);
			ADD_BUTTON(mySizerHori,myButton,".",ewolEventKeyEvent);
			ADD_BUTTON(mySizerHori,myButton,":",ewolEventKeyEvent);
			ADD_BUTTON(mySizerHori,myButton,"/",ewolEventKeyEvent);
			ADD_BUTTON(mySizerHori,myButton,"!",ewolEventKeyEvent);
		mySizerHori = new ewol::SizerHori();
			mySizerVert->SubWidgetAdd(mySizerHori);
			ADD_BUTTON(mySizerHori,myButton,"Hide",ewolEventKeyboardHide);
			myButton->SetExpendX(false);
			ADD_BUTTON(mySizerHori,myButton,"Ctrl",ewolEventKeyEvent);
			myButton->SetExpendX(false);
			ADD_BUTTON(mySizerHori,myButton,"Pomme",ewolEventKeyEvent);
			myButton->SetExpendX(false);
			ADD_BUTTON(mySizerHori,myButton,"Alt",ewolEventKeyEvent);
			myButton->SetExpendX(false);
			ADD_BUTTON(mySizerHori,myButton," ",ewolEventKeyEvent);
			ADD_BUTTON(mySizerHori,myButton,"AltGr",ewolEventKeyEvent);
			myButton->SetExpendX(false);
			ADD_BUTTON(mySizerHori,myButton,"Ctrl",ewolEventKeyEvent);
			myButton->SetExpendX(false);
}


/**
 * @brief Receive a message from an other EObject with a specific eventId and data
 * @param[in] CallerObject Pointer on the EObject that information came from
 * @param[in] eventId Message registered by this class
 * @param[in] data Data registered by this class
 * @return ---
 */
void ewol::Keyboard::OnReceiveMessage(ewol::EObject * CallerObject, const char * eventId, etk::UString data)
{
	EWOL_INFO("Receive Event from the Keyboard ... : widget*=" << CallerObject << "\"" << eventId << "\" ==> data=\"" << data << "\"" );
	if (ewolEventKeyEvent == eventId) {
		if (NULL == CallerObject) {
			return;
		}
		ewol::Button * bt     = static_cast<ewol::Button *>(CallerObject);
		EWOL_DEBUG("kbevent : \"" << bt->GetLabel() << "\"");
		etk::UString data = bt->GetLabel();
		if (data == "DEL") {
			char tmppp[2] = {0x08, 0x00};
			data = tmppp;
		}
		if (data == "ENTER") {
			data = "\n";
		}
		if (data == "TAB") {
			data = "\t";
		}
		guiAbstraction::SendKeyboardEvent(true, data[0]);
		guiAbstraction::SendKeyboardEvent(false, data[0]);
		return;
	} else if (ewolEventKeyboardHide == eventId) {
		Hide();
		ewol::ForceRedrawAll();
	}
	//return GenEventInputExternal(eventExternId, x, y);
	return;
};


void ewol::Keyboard::SetMinSise(etkFloat_t x, etkFloat_t y)
{
	EWOL_ERROR("Pop-up can not have a user Minimum size (herited from under elements)");
}

void ewol::Keyboard::SetExpendX(bool newExpend)
{
	EWOL_ERROR("Pop-up can not have a user expend settings X (herited from under elements)");
}

void ewol::Keyboard::SetExpendY(bool newExpend)
{
	EWOL_ERROR("Pop-up can not have a user expend settings Y (herited from under elements)");
}


bool ewol::Keyboard::CalculateSize(etkFloat_t availlableX, etkFloat_t availlableY)
{
	//EWOL_DEBUG("CalculateSize(" << availlableX << "," << availlableY << ")");
	// pop-up fill all the display :
	m_size.x = availlableX;
	m_size.y = availlableY;
	
	if (NULL != m_subWidget[m_currentCreateId]) {
		coord2D_ts subWidgetSize;
		subWidgetSize = m_subWidget[m_currentCreateId]->GetMinSize();
		if (true == m_subWidget[m_currentCreateId]->CanExpentX()) {
			subWidgetSize.x = m_size.x;
		}
		if (true == m_subWidget[m_currentCreateId]->CanExpentY()) {
			subWidgetSize.y = m_size.y;
		}
		// force to be an integer ...
		subWidgetSize.x = (int32_t)subWidgetSize.x;
		subWidgetSize.y = (int32_t)subWidgetSize.y;
		
		m_subWidget[m_currentCreateId]->SetOrigin(m_origin.x, m_origin.y);
		m_subWidget[m_currentCreateId]->CalculateSize(subWidgetSize.x, subWidgetSize.y);
	}
	MarkToReedraw();
	return true;
}


bool ewol::Keyboard::CalculateMinSize(void)
{
	m_userExpendX=false;
	m_userExpendY=false;
	m_minSize.x = 50.0;
	m_minSize.y = 50.0;
	if (NULL != m_subWidget[m_currentCreateId]) {
		m_subWidget[m_currentCreateId]->CalculateMinSize();
		coord2D_ts tmpSize = m_subWidget[m_currentCreateId]->GetMinSize();
		m_minSize.x = tmpSize.x;
		m_minSize.y = tmpSize.y;
	}
	//EWOL_DEBUG("CalculateMinSize(" << m_minSize.x << "," << m_minSize.y << ")");
	return true;
}



bool ewol::Keyboard::OnDraw(void)
{
	if (NULL != m_subWidget[m_currentDrawId]) {
		m_subWidget[m_currentDrawId]->GenDraw();
	}
	return true;
}


void ewol::Keyboard::OnRegenerateDisplay(void)
{
	if (true == NeedRedraw()) {
		color_ts mycolor;
		mycolor.red   = 1.0;
		mycolor.green = 1.0;
		mycolor.blue  = 1.0;
		mycolor.alpha = 0.50;
		// generate a white background and take gray on other surfaces
		ClearOObjectList();
		ewol::OObject2DColored * BGOObjects = new ewol::OObject2DColored();
		BGOObjects->SetColor(mycolor);
		BGOObjects->Rectangle(0, 0, m_size.x, m_size.y);
		AddOObject(BGOObjects);
	}
	if (NULL != m_subWidget[m_currentCreateId]) {
		m_subWidget[m_currentCreateId]->OnRegenerateDisplay();
	}
}

bool ewol::Keyboard::OnEventInput(int32_t IdInput, eventInputType_te typeEvent, eventPosition_ts pos)
{
	if (NULL != m_subWidget[m_currentCreateId]) {
		return m_subWidget[m_currentCreateId]->GenEventInput(IdInput, typeEvent, pos.abs);
	}
	return false;
}



/**
 * @brief Inform object that an other object is removed ...
 * @param[in] removeObject Pointer on the EObject remeved ==> the user must remove all reference on this EObject
 * @note : Sub classes must call this class
 * @return ---
 */
void ewol::Keyboard::OnObjectRemove(ewol::EObject * removeObject)
{
	ewol::Drawable::OnObjectRemove(removeObject);
	if (removeObject == m_subWidget[m_currentCreateId]) {
		m_subWidget[m_currentCreateId] = NULL;
		m_needFlipFlop = true;
	}
}



void ewol::Keyboard::OnFlipFlopEvent(void)
{
	bool needFlipFlop = m_needFlipFlop;
	// call herited classes
	ewol::Drawable::OnFlipFlopEvent();
	// internal saving
	if (true == needFlipFlop) {
		m_subWidget[m_currentCreateId] = m_subWidget[m_currentDrawId];
	}
	// in every case, we propagate the flip-flop EVENT
	if (NULL != m_subWidget[m_currentDrawId]) {
		m_subWidget[m_currentDrawId]->OnFlipFlopEvent();
	}
}
