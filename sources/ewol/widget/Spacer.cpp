/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/widget/Spacer.h>

#include <ewol/compositing/Drawing.h>
#include <ewol/widget/WidgetManager.h>


#undef __class__
#define __class__	"Spacer"


widget::Spacer::Spacer(void)
{
	m_localSize = 10;
	SetCanHaveFocus(false);
	m_color = draw::color::black;
	m_color.a = 0;
}

widget::Spacer::~Spacer(void)
{
	
}


bool widget::Spacer::CalculateMinSize(void)
{
	m_minSize.x = m_localSize;
	m_minSize.y = m_localSize;
	return true;
}


void widget::Spacer::SetSize(float size)
{
	m_localSize = size;
	MarkToRedraw();
}


void widget::Spacer::OnDraw(ewol::DrawProperty& displayProp)
{
	m_draw.Draw();
}


#define BORDER_SIZE_TMP         (4)
void widget::Spacer::OnRegenerateDisplay(void)
{
	if (false == NeedRedraw()) {
		return;
	}
	m_draw.Clear();
	
	if (m_color.a == 0) {
		return;
	}
	m_draw.SetColor(m_color);
	m_draw.SetPos(vec3(0, 0, 0) );
	m_draw.RectangleWidth(vec3(m_size.x, m_size.y) );
}
