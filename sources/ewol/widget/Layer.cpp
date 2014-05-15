/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/widget/Layer.h>
#include <ewol/widget/Manager.h>


#undef __class__
#define __class__ "Layer"

static ewol::Widget* create() {
	return new ewol::widget::Layer();
}

void ewol::widget::Layer::init(ewol::widget::Manager& _widgetManager) {
	_widgetManager.addWidgetCreator(__class__,&create);
}

ewol::widget::Layer::Layer() {
	addObjectType("ewol::widget::Layer");
}

ewol::widget::Layer::~Layer() {
	EWOL_DEBUG("[" << getId() << "] Layer : destroy");
}

ewol::Widget* ewol::widget::Layer::getWidgetAtPos(const vec2& _pos) {
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

