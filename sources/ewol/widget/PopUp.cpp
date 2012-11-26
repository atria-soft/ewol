/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/widget/PopUp.h>
#include <ewol/widget/WidgetManager.h>
#include <ewol/compositing/Drawing.h>

#undef __class__
#define __class__	"PopUp"

widget::PopUp::PopUp(void) :
	m_subWidgetNext(NULL)
{
	m_userExpend.x = true;
	m_userExpend.y = true;
	
	m_colorBackGroung = draw::color::white;
	
	m_colorEmptyArea = draw::color::black;
	m_colorEmptyArea.a = 0x7F;
	
	m_colorBorder = draw::color::black;
	m_colorBorder.a = 0x7F;
	
	m_subWidget = 0;
}

widget::PopUp::~PopUp(void)
{
	SubWidgetRemove();
}


bool widget::PopUp::CalculateSize(float availlableX, float availlableY)
{
	//EWOL_DEBUG("CalculateSize(" << availlableX << "," << availlableY << ")");
	// pop-up fill all the display :
	m_size.x = availlableX;
	m_size.y = availlableY;
	
	if (NULL != m_subWidget) {
		etk::Vector2D<float> subWidgetSize;
		etk::Vector2D<float> subWidgetOrigin;
		subWidgetSize = m_subWidget->GetMinSize();
		if (true == m_subWidget->CanExpentX()) {
			subWidgetSize.x = m_size.x;
		}
		if (true == m_subWidget->CanExpentY()) {
			subWidgetSize.y = m_size.y;
		}
		if (m_displayRatio>0.1 && m_displayRatio<=1) {
			subWidgetSize.x = etk_max(m_size.x*m_displayRatio, subWidgetSize.x);
			subWidgetSize.y = etk_max(m_size.y*m_displayRatio, subWidgetSize.y);
		}
		// force to be an integer ...
		subWidgetSize.x = (int32_t)subWidgetSize.x;
		subWidgetSize.y = (int32_t)subWidgetSize.y;
		// set config to the Sub-widget
		subWidgetOrigin.x = (int32_t)(m_size.x - m_origin.x - subWidgetSize.x)/2 + m_origin.x;
		subWidgetOrigin.y = (int32_t)(m_size.y - m_origin.y - subWidgetSize.y)/2 + m_origin.y;
		
		m_subWidget->SetOrigin(subWidgetOrigin.x, subWidgetOrigin.y);
		m_subWidget->CalculateSize(subWidgetSize.x, subWidgetSize.y);
	}
	MarkToRedraw();
	return true;
}


bool widget::PopUp::CalculateMinSize(void)
{
	//EWOL_DEBUG("CalculateMinSize");
	m_userExpend.x=false;
	m_userExpend.y=false;
	m_minSize.x = 50.0;
	m_minSize.y = 50.0;
	if (NULL != m_subWidget) {
		m_subWidget->CalculateMinSize();
		etk::Vector2D<float> tmpSize = m_subWidget->GetMinSize();
		m_minSize.x = tmpSize.x;
		m_minSize.y = tmpSize.y;
	}
	//EWOL_DEBUG("CalculateMinSize(" << m_minSize.x << "," << m_minSize.y << ")");
	MarkToRedraw();
	return true;
}

void widget::PopUp::SetMinSise(float x, float y)
{
	EWOL_ERROR("Pop-up can not have a user Minimum size (herited from under elements)");
}

void widget::PopUp::SetExpendX(bool newExpend)
{
	EWOL_ERROR("Pop-up can not have a user expend settings X (herited from under elements)");
}

void widget::PopUp::SetExpendY(bool newExpend)
{
	EWOL_ERROR("Pop-up can not have a user expend settings Y (herited from under elements)");
}


void widget::PopUp::SubWidgetSet(ewol::Widget* newWidget)
{
	if (NULL == newWidget) {
		EWOL_ERROR("Try to set a sub wiget with NULL pointer ...");
		return;
	}
	SubWidgetRemove();
	m_subWidget = newWidget;
	//EWOL_DEBUG("SetSubWidget on Pop-Up : " << (int64_t)m_subWidget);
	MarkToRedraw();
}


void widget::PopUp::SubWidgetRemove(void)
{
	if (NULL != m_subWidget) {
		delete(m_subWidget);
		m_subWidget = NULL;
	}
	MarkToRedraw();
}


void widget::PopUp::OnDraw(ewol::DrawProperty& displayProp)
{
	// draw upper classes
	widget::Drawable::OnDraw(displayProp);
	if (NULL != m_subWidget) {
		m_subWidget->GenDraw(displayProp);
	}
}

#define BORDER_SIZE_TMP         (4)
void widget::PopUp::OnRegenerateDisplay(void)
{
	if (true == NeedRedraw()) {
	}
	// generate a white background and take gray on other surfaces
	ClearOObjectList();
	ewol::Drawing * BGOObjects = new ewol::Drawing();
	AddOObject(BGOObjects);
	
	BGOObjects->SetColor(m_colorEmptyArea);
	BGOObjects->SetPos(etk::Vector3D<float>(0,0,0));
	BGOObjects->RectangleWidth(etk::Vector3D<float>(m_size.x, m_size.y, 0));
	// set the area in white ...
	if (NULL != m_subWidget) {
		etk::Vector2D<float> tmpSize = m_subWidget->GetSize();
		etk::Vector2D<float> tmpOrigin = m_subWidget->GetOrigin();
		BGOObjects->SetColor(m_colorBorder);
		BGOObjects->SetPos(etk::Vector3D<float>(tmpOrigin.x-BORDER_SIZE_TMP, tmpOrigin.y-BORDER_SIZE_TMP,0) );
		BGOObjects->RectangleWidth(etk::Vector3D<float>(tmpSize.x+2*BORDER_SIZE_TMP, tmpSize.y+2*BORDER_SIZE_TMP, 0) );
		BGOObjects->SetColor(m_colorBackGroung);
		BGOObjects->SetPos(etk::Vector3D<float>(tmpOrigin.x, tmpOrigin.y,0) );
		BGOObjects->RectangleWidth(etk::Vector3D<float>(tmpSize.x, tmpSize.y, 0) );
	}
	if (NULL != m_subWidget) {
		m_subWidget->OnRegenerateDisplay();
	}
}


ewol::Widget * widget::PopUp::GetWidgetAtPos(etk::Vector2D<float> pos)
{
	// calculate relative position
	etk::Vector2D<float> relativePos = RelativePosition(pos);
	// for the element in the pop-up ...
	if (NULL != m_subWidget) {
		etk::Vector2D<float> tmpSize = m_subWidget->GetSize();
		etk::Vector2D<float> tmpOrigin = m_subWidget->GetOrigin();
		if(    (tmpOrigin.x <= relativePos.x && tmpOrigin.x + tmpSize.x >= relativePos.x)
		    && (tmpOrigin.y <= relativePos.y && tmpOrigin.y + tmpSize.y >= relativePos.y) )
		{
			return m_subWidget->GetWidgetAtPos(pos);
		} else {
			//EWOL_INFO("Event ouside the Pop-up");
		}
	}
	// otherwise the event go to this widget ...
	return this;
}


void widget::PopUp::SetDisplayRatio(float ratio)
{
	m_displayRatio = ratio;
	MarkToRedraw();
}


void widget::PopUp::OnObjectRemove(ewol::EObject * removeObject)
{
	// First step call parrent : 
	widget::Drawable::OnObjectRemove(removeObject);
	// second step find if in all the elements ...
	if(m_subWidget == removeObject) {
		EWOL_DEBUG("Remove pop-up sub Element ==> destroyed object");
		m_subWidget = NULL;
	}
}

