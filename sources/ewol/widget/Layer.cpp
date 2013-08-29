/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/widget/Layer.h>
#include <ewol/widget/WidgetManager.h>


#undef __class__
#define __class__	"Layer"

static ewol::Widget* Create(void)
{
	return new widget::Layer();
}

void widget::Layer::Init(ewol::WidgetManager& _widgetManager)
{
	_widgetManager.AddWidgetCreator(__class__,&Create);
}

widget::Layer::Layer(void)
{
	// nothing to do ...
}

widget::Layer::~Layer(void)
{
	EWOL_DEBUG("[" << GetId() << "] Layer : destroy");
}

ewol::Widget* widget::Layer::GetWidgetAtPos(const vec2& _pos)
{
	if (true == IsHide()) {
		return NULL;
	}
	// for all element in the sizer ...
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if (NULL != m_subWidget[iii]) {
			vec2 tmpSize = m_subWidget[iii]->GetSize();
			vec2 tmpOrigin = m_subWidget[iii]->GetOrigin();
			if(    (tmpOrigin.x() <= _pos.x() && tmpOrigin.x() + tmpSize.x() >= _pos.x())
			    && (tmpOrigin.y() <= _pos.y() && tmpOrigin.y() + tmpSize.y() >= _pos.y()) )
			{
				ewol::Widget * tmpWidget = m_subWidget[iii]->GetWidgetAtPos(_pos);
				if (NULL != tmpWidget) {
					return tmpWidget;
				}
				// parse the next layer ...
			}
		}
	}
	return NULL;
};

