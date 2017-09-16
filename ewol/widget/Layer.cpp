/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ewol/widget/Layer.hpp>
#include <ewol/widget/Manager.hpp>

#include <etk/typeInfo.hpp>
ETK_DECLARE_TYPE(ewol::widget::Layer);

ewol::widget::Layer::Layer() {
	addObjectType("ewol::widget::Layer");
}

ewol::widget::Layer::~Layer() {
	EWOL_DEBUG("[" << getId() << "] Layer : destroy");
}

ewol::WidgetShared ewol::widget::Layer::getWidgetAtPos(const vec2& _pos) {
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
			ewol::WidgetShared tmpWidget = it->getWidgetAtPos(_pos);
			if (tmpWidget != nullptr) {
				return tmpWidget;
			}
			// parse the next layer ...
		}
	}
	return nullptr;
};

