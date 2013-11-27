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
#define __class__ "Layer"

static ewol::Widget* create(void) {
	return new widget::Layer();
}

void widget::Layer::init(ewol::WidgetManager& _widgetManager) {
	_widgetManager.addWidgetCreator(__class__,&create);
}

widget::Layer::Layer(void) {
	addObjectType("widget::Layer");
}

widget::Layer::~Layer(void) {
	EWOL_DEBUG("[" << getId() << "] Layer : destroy");
}

ewol::Widget* widget::Layer::getWidgetAtPos(const vec2& _pos) {
	if (true == isHide()) {
		return NULL;
	}
	// for all element in the sizer ...
	for (size_t iii=0; iii<m_subWidget.size(); iii++) {
		if (NULL != m_subWidget[iii]) {
			vec2 tmpSize = m_subWidget[iii]->getSize();
			vec2 tmpOrigin = m_subWidget[iii]->getOrigin();
			if(    (tmpOrigin.x() <= _pos.x() && tmpOrigin.x() + tmpSize.x() >= _pos.x())
			    && (tmpOrigin.y() <= _pos.y() && tmpOrigin.y() + tmpSize.y() >= _pos.y()) )
			{
				ewol::Widget * tmpWidget = m_subWidget[iii]->getWidgetAtPos(_pos);
				if (NULL != tmpWidget) {
					return tmpWidget;
				}
				// parse the next layer ...
			}
		}
	}
	return NULL;
};

