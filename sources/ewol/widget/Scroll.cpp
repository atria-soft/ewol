/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/widget/Scroll.h>
#include <ewol/ewol.h>
#include <ewol/widget/WidgetManager.h>
#include <ewol/debug.h>

#undef __class__
#define __class__	"Scroll"

static ewol::Widget* Create(void)
{
	return new widget::Scroll();
}

void widget::Scroll::Init(void)
{
	ewol::widgetManager::AddWidgetCreator(__class__,&Create);
}

void widget::Scroll::UnInit(void)
{
	ewol::widgetManager::AddWidgetCreator(__class__,NULL);
}

widget::Scroll::Scroll(void) :
	m_scrollOrigin(0,0),
	m_scrollSize(0,0)
{
	
}

widget::Scroll::~Scroll(void)
{
	
}


ewol::Widget* widget::Scroll::GetWidgetAtPos(const vec2& _pos)
{
	if (false==IsHide()) {
		return this;
	}
	return NULL;
}


bool widget::Scroll::OnEventInput(const ewol::EventInput& _event)
{
	EWOL_DEBUG("event from the scroll ... " << _event.GetType() << " idinput=" << _event.GetId() << " status=" << _event.GetStatus());
	return false;
}

