/**
 *******************************************************************************
 * @file ewol/widget/Image.cpp
 * @brief ewol Image widget system (Sources)
 * @author Edouard DUPIN
 * @date 12/05/2012
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

#include <ewol/widget/Image.h>

#include <ewol/OObject.h>
#include <ewol/WidgetManager.h>


extern const char * const ewolEventImagePressed    = "ewol-image-Pressed";


/**
 * @brief Initilise the basic widget property ==> due to the android system
 * @note all widget that have template might have this initializer ...
 * @param ---
 * @return ---
 */
void ewol::WIDGET_ImageInit(void)
{
	
}

#undef __class__
#define __class__	"Image"


void ewol::Image::Init(void)
{
	AddEventId(ewolEventImagePressed);
	
	#ifdef __PLATFORM__Android
		m_padding.y = 12;
		m_padding.x = 12;
	#else
		m_padding.y = 4;
		m_padding.x = 4;
	#endif
	
	m_textColorBg = etk::color::color_Black;
	m_textColorBg.alpha = 0x00;
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


//!< EObject name :
extern const char * const ewol::TYPE_EOBJECT_WIDGET_IMAGE = "Image";

/**
 * @brief Check if the object has the specific type.
 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
 * @param[in] objectType type of the object we want to check
 * @return true if the object is compatible, otherwise false
 */
bool ewol::Image::CheckObjectType(const char * const objectType)
{
	if (NULL == objectType) {
		EWOL_ERROR("check error : \"" << ewol::TYPE_EOBJECT_WIDGET_IMAGE << "\" != NULL(pointer) ");
		return false;
	}
	if (objectType == ewol::TYPE_EOBJECT_WIDGET_IMAGE) {
		return true;
	} else {
		if(true == ewol::Drawable::CheckObjectType(objectType)) {
			return true;
		}
		EWOL_ERROR("check error : \"" << ewol::TYPE_EOBJECT_WIDGET_IMAGE << "\" != \"" << objectType << "\"");
		return false;
	}
}

/**
 * @brief Get the current Object type of the EObject
 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
 * @param[in] objectType type description
 * @return true if the object is compatible, otherwise false
 */
const char * const ewol::Image::GetObjectType(void)
{
	return ewol::TYPE_EOBJECT_WIDGET_IMAGE;
}


void ewol::Image::SetPadding(coord2D_ts newPadding)
{
	m_padding = newPadding;
}

bool ewol::Image::CalculateMinSize(void)
{
	m_minSize.x = m_padding.x*2 + m_imageSize;
	m_minSize.y = m_padding.y*2 + m_imageSize;
	MarkToReedraw();
	return true;
}


void ewol::Image::SetFile(etk::UString newFile)
{
	m_imageSelected = newFile;
	MarkToReedraw();
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
		
		if (true==m_userFillX) {
			tmpSizeX = m_size.x;
			tmpOriginX = 0;
		}
		if (true==m_userFillY) {
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

/**
 * @brief Event on an input of this Widget
 * @param[in] IdInput Id of the current Input (PC : left=1, right=2, middle=3, none=0 / Tactil : first finger=1 , second=2 (only on this widget, no knowledge at ouside finger))
 * @param[in] typeEvent ewol type of event like EVENT_INPUT_TYPE_DOWN/EVENT_INPUT_TYPE_MOVE/EVENT_INPUT_TYPE_UP/EVENT_INPUT_TYPE_SINGLE/EVENT_INPUT_TYPE_DOUBLE/...
 * @param[in] pos Absolute position of the event
 * @return true the event is used
 * @return false the event is not used
 */
bool ewol::Image::OnEventInput(int32_t IdInput, eventInputType_te typeEvent, coord2D_ts pos)
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

