/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#include <ewol/widget/Layer.h>
#include <ewol/widget/Manager.h>


#undef __class__
#define __class__ "Layer"

ewol::widget::Layer::Layer() {
	addObjectType("ewol::widget::Layer");
}

ewol::widget::Layer::~Layer() {
	EWOL_DEBUG("[" << getId() << "] Layer : destroy");
}

std::shared_ptr<ewol::Widget> ewol::widget::Layer::getWidgetAtPos(const vec2& _pos) {
	if (*propertyHide == true) {
		return nullptr;
	}
	// for all element in the sizer ...
	for (auto &it : m_subWidget) {
		if (it == nullptr) {
			continue;
		}
		vec2 tmpSize = it->getSize();
		vec2 tmpOrigin = it->getOrigin();
		if(    (tmpOrigin.x() <= _pos.x() && tmpOrigin.x() + tmpSize.x() >= _pos.x())
		    && (tmpOrigin.y() <= _pos.y() && tmpOrigin.y() + tmpSize.y() >= _pos.y()) ) {
			std::shared_ptr<ewol::Widget> tmpWidget = it->getWidgetAtPos(_pos);
			if (tmpWidget != nullptr) {
				return tmpWidget;
			}
			// parse the next layer ...
		}
	}
	return nullptr;
};

