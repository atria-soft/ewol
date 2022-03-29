/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */


#include <ewol/ewol.hpp>
#include <ewol/widget/Container2.hpp>
#include <ewol/widget/Manager.hpp>

#include <etk/typeInfo.hpp>
ETK_DECLARE_TYPE(ewol::widget::Container2);

ewol::widget::Container2::Container2() :
  this.idWidgetDisplayed(0) {
	addObjectType("ewol::widget::Container2");
}

ewol::widget::Container2::~Container2() {
	subWidgetRemove();
	subWidgetRemoveToggle();
}




