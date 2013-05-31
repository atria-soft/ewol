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

widget::PopUp::PopUp(const etk::UString& _shaperName) :
	m_shaper(_shaperName)
{
	m_userExpand.setValue(false, false);
	SetMinSize(ewol::Dimension(vec2(80,80),ewol::Dimension::Pourcent));
}

widget::PopUp::~PopUp(void)
{
	
}

void widget::PopUp::SetShaperName(const etk::UString& _shaperName)
{
	m_shaper.SetSource(_shaperName);
	MarkToRedraw();
}


void widget::PopUp::CalculateSize(const vec2& _availlable)
{
	ewol::Widget::CalculateSize(_availlable);
	m_size = _availlable;
	if (NULL != m_subWidget) {
		vec2 subElementSize = m_minSize;
		vec2 subElementOrigin = m_origin + (m_size-m_minSize)/2.0f;
		
		vec2 subWidgetSize = m_subWidget->GetCalculateMinSize();
		if (true == m_subWidget->CanExpand().x()) {
			subWidgetSize.setX(m_minSize.x());
		}
		if (true == m_subWidget->CanExpand().y()) {
			subWidgetSize.setY(m_minSize.y());
		}
		// limit the size of the element :
		subWidgetSize.setMin(m_minSize);
		// posiition at a int32_t pos :
		subWidgetSize = vec2ClipInt32(subWidgetSize);
		
		// set config to the Sub-widget
		vec2 subWidgetOrigin = m_origin + (m_size-subWidgetSize)/2.0f;
		subWidgetOrigin = vec2ClipInt32(subWidgetOrigin);
		
		m_subWidget->SetOrigin(subWidgetOrigin);
		m_subWidget->CalculateSize(subWidgetSize);
	}
	MarkToRedraw();
}

void widget::PopUp::OnDraw(void)
{
	m_shaper.Draw();
}

#define BORDER_SIZE_TMP         (4)
void widget::PopUp::OnRegenerateDisplay(void)
{
	if (true == NeedRedraw()) {
		m_shaper.Clear();
		vec2 padding = m_shaper.GetPadding();
		if (NULL != m_subWidget) {
			vec2 tmpSize = m_subWidget->GetSize();
			tmpSize.setMax(m_minSize);
			vec2 tmpOrigin = m_origin + (m_size-tmpSize)/2.0f;
			tmpOrigin = vec2ClipInt32(tmpOrigin);
			
			m_shaper.SetOrigin(vec2ClipInt32(m_origin));
			m_shaper.SetSize(vec2ClipInt32(m_size));
			m_shaper.SetInsidePos(vec2ClipInt32(tmpOrigin-padding));
			m_shaper.SetInsideSize(vec2ClipInt32(tmpSize + padding*2.0f));
		} else {
			m_shaper.SetOrigin(vec2ClipInt32(m_origin));
			m_shaper.SetSize(vec2ClipInt32(m_size));
			m_shaper.SetInsidePos(vec2ClipInt32(m_origin+padding));
			m_shaper.SetInsideSize(vec2ClipInt32(m_size-padding*2.0f));
		}
	}
	// SUBwIDGET GENERATION ...
	if (NULL != m_subWidget) {
		m_subWidget->OnRegenerateDisplay();
	}
}

ewol::Widget* widget::PopUp::GetWidgetAtPos(const vec2& pos)
{
	ewol::Widget* val = widget::Container::GetWidgetAtPos(pos);
	if (NULL != val) {
		return val;
	}
	return this;
}


