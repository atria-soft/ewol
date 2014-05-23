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

ewol::object::Shared<ewol::Widget> ewol::widget::Layer::getWidgetAtPos(const vec2& _pos) {
	if (true == isHide()) {
		return nullptr;
	}
	// for all element in the sizer ...
	for (auto it : m_subWidget) {
		if (it != nullptr) {
			vec2 tmpSize = it->getSize();
			vec2 tmpOrigin = it->getOrigin();
			if(    (tmpOrigin.x() <= _pos.x() && tmpOrigin.x() + tmpSize.x() >= _pos.x())
			    && (tmpOrigin.y() <= _pos.y() && tmpOrigin.y() + tmpSize.y() >= _pos.y()) )
			{
				ewol::object::Shared<ewol::Widget> tmpWidget = it->getWidgetAtPos(_pos);
				if (nullptr != tmpWidget) {
					return tmpWidget;
				}
				// parse the next layer ...
			}
		}
	}
	return nullptr;
};

