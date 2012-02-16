/**
 *******************************************************************************
 * @file ewol/widget/Menu.cpp
 * @brief ewol Menu widget system (Sources)
 * @author Edouard DUPIN
 * @date 16/02/2012
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



#include <ewol/widget/Menu.h>
#include <ewol/widget/Button.h>

#undef __class__
#define __class__	"ewol::Menu"

ewol::Menu::Menu(void)
{
	m_staticId = 0;
}

ewol::Menu::~Menu(void)
{
	Clear();
}

void ewol::Menu::SubWidgetRemoveAll(void)
{
	EWOL_ERROR("Not availlable");
}

void ewol::Menu::SubWidgetAdd(ewol::Widget* newWidget)
{
	EWOL_ERROR("Not availlable");
}

void ewol::Menu::SubWidgetRemove(ewol::Widget* newWidget)
{
	EWOL_ERROR("Not availlable");
}

void ewol::Menu::SubWidgetUnLink(ewol::Widget* newWidget)
{
	EWOL_ERROR("Not availlable");
}

void ewol::Menu::Clear(void)
{
	for( int32_t iii=0; iii < m_listElement.Size(); iii++) {
		if (m_listElement[iii] != NULL) {
			delete(m_listElement[iii]);
			m_listElement[iii] = NULL;
		}
	}
	m_listElement.Clear();
}

int32_t ewol::Menu::AddTitle(etk::UString label, etk::UString image, const char * generateEvent, const etk::UString message)
{
	return Add(-1, label, image, generateEvent, message);
}

int32_t ewol::Menu::Add(int32_t parent, etk::UString label, etk::UString image, const char * generateEvent, const etk::UString message)
{
	ewol::MenuElement * tmpObject = new ewol::MenuElement();
	if (NULL == tmpObject) {
		EWOL_ERROR("Allocation problem");
		return -1;
	}
	tmpObject->m_localId = m_staticId++;
	tmpObject->m_parentId = parent;
	tmpObject->m_label = label;
	tmpObject->m_image = image;
	tmpObject->m_generateEvent = generateEvent;
	tmpObject->m_message = message;
	m_listElement.PushBack(tmpObject);
	if (-1 == tmpObject->m_parentId) {
		ewol::Button * myButton = new ewol::Button(label);
		ewol::SizerHori::SubWidgetAdd(myButton);
		/*
		if (false == myButton->ExternLinkOnEvent(ewolEventButtonPressed, GetWidgetId(), "event ... ") ) {
			EDN_CRITICAL("link with an entry event");
		}
		*/
	}
	return tmpObject->m_localId;
}

void ewol::Menu::AddSpacer(void)
{
	EWOL_TODO("NOT now...");
}
