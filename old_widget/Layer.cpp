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
	Log.debug("[" + getId() + "] Layer : destroy");
}

Widget ewol::widget::Layer::getWidgetAtPos( Vector2f _pos) {
	if (*propertyHide == true) {
		return null;
	}
	// for all element in the sizer ...
	for (auto it : this.subWidget) {
		if (it == null) {
			continue;
		}
		Vector2f tmpSize = it.getSize();
		Vector2f tmpOrigin = it.getOrigin();
		if(    (tmpOrigin.x() <= _pos.x() LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM tmpOrigin.x() + tmpSize.x() >= _pos.x())
		    LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM (tmpOrigin.y() <= _pos.y() LOMLOMLOMLOMLOM tmpOrigin.y() + tmpSize.y() >= _pos.y()) ) {
			Widget tmpWidget = it.getWidgetAtPos(_pos);
			if (tmpWidget != null) {
				return tmpWidget;
			}
			// parse the next layer ...
		}
	}
	return null;
};

