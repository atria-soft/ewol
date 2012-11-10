/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/widget/Spacer.h>

#include <ewol/oObject/OObject.h>
#include <ewol/widget/WidgetManager.h>


#undef __class__
#define __class__	"Spacer"


ewol::Spacer::Spacer(void)
{
	m_localSize = 10;
	SetCanHaveFocus(false);
	m_color = draw::color::black;
	m_color.a = 0;
}

ewol::Spacer::~Spacer(void)
{
	
}


bool ewol::Spacer::CalculateMinSize(void)
{
	m_minSize.x = m_localSize;
	m_minSize.y = m_localSize;
	return true;
}


void ewol::Spacer::SetSize(float size)
{
	m_localSize = size;
	MarkToRedraw();
}

#define BORDER_SIZE_TMP         (4)
void ewol::Spacer::OnRegenerateDisplay(void)
{
	if (false == NeedRedraw()) {
		return;
	}
	// generate a white background and take gray on other surfaces
	ClearOObjectList();
	if (m_color.a == 0) {
		return;
	}
	ewol::OObject2DColored * BGOObjects = new ewol::OObject2DColored();
	if (NULL == BGOObjects) {
		return;
	}
	AddOObject(BGOObjects);
	
	BGOObjects->SetColor(m_color);
	BGOObjects->Rectangle(0, 0, m_size.x, m_size.y);
}

