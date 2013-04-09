/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/widget/ProgressBar.h>

#include <ewol/compositing/Drawing.h>
#include <ewol/widget/WidgetManager.h>

#undef __class__
#define __class__	"ProgressBar"

static ewol::Widget* Create(void)
{
	return new widget::ProgressBar();
}

void widget::ProgressBar::Init(void)
{
	ewol::widgetManager::AddWidgetCreator(__class__,&Create);
}

void widget::ProgressBar::UnInit(void)
{
	ewol::widgetManager::AddWidgetCreator(__class__,NULL);
}


const int32_t dotRadius = 6;

widget::ProgressBar::ProgressBar(void)
{
	m_value = 0.0;
	
	m_textColorFg = draw::color::black;
	
	m_textColorBgOn = 0x00FF00FF;
	
	m_textColorBgOff = draw::color::black;
	m_textColorBgOff.a = 0x3F;
	SetCanHaveFocus(true);
}

widget::ProgressBar::~ProgressBar(void)
{
	
}


void widget::ProgressBar::CalculateMinSize(void)
{
	m_minSize.setValue( etk_max(m_userMinSize.x(), 40),
	                    etk_max(m_userMinSize.y(), dotRadius*2) );
	MarkToRedraw();
}


void widget::ProgressBar::ValueSet(float val)
{
	m_value = etk_avg(0, val, 1);
	MarkToRedraw();
}


float widget::ProgressBar::ValueGet(void)
{
	return m_value;
}


void widget::ProgressBar::OnRegenerateDisplay(void)
{
	if (true == NeedRedraw()) {
		// clean the object list ...
		ClearOObjectList();
		
		ewol::Drawing * tmpDraw = new ewol::Drawing;
		
		tmpDraw->SetColor(m_textColorFg);
		
		int32_t tmpSizeX = m_size.x() - 10;
		int32_t tmpSizeY = m_size.y() - 10;
		int32_t tmpOriginX = 5;
		int32_t tmpOriginY = 5;
		tmpDraw->SetColor(m_textColorBgOn);
		tmpDraw->SetPos(vec3(tmpOriginX, tmpOriginY, 0) );
		tmpDraw->RectangleWidth(vec3(tmpSizeX*m_value, tmpSizeY, 0) );
		tmpDraw->SetColor(m_textColorBgOff);
		tmpDraw->SetPos(vec3(tmpOriginX+tmpSizeX*m_value, tmpOriginY, 0) );
		tmpDraw->RectangleWidth(vec3(tmpSizeX*(1.0-m_value), tmpSizeY, 0) );
		
		// TODO : Create a better progress Bar ...
		//tmpDraw->SetColor(m_textColorFg);
		//tmpDraw->RectangleBorder( tmpOriginX, tmpOriginY, tmpSizeX, tmpSizeY, 1);
		
		AddOObject(tmpDraw);
	}
}



