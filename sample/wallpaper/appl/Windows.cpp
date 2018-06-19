/** @file
 * @author Edouard DUPIN
 * @copyright 2010, Edouard DUPIN, all right reserved
 * @license GPL v3 (see license file)
 */

#include <ewol/ewol.hpp>
#include <appl/debug.hpp>
#include <appl/WidgetDisplay.hpp>
#include <appl/Windows.hpp>

appl::Windows::Windows() {
	addObjectType("appl::Windows");
	propertyTitle.setDirectCheck(etk::String("sample ") + PROJECT_NAME);
}

void appl::Windows::init() {
	ewol::widget::Windows::init();
	appl::WidgetDisplayShared tmpWidget = appl::WidgetDisplay::create();
	if (tmpWidget == null) {
		APPL_ERROR("Can not allocate widget ==> display might be in error");
	} else {
		setSubWidget(tmpWidget);
		tmpWidget->propertyExpand.set(bvec2(true,true));
		tmpWidget->propertyFill.set(bvec2(true,true));
	}
}

appl::Windows::~Windows() {
	
}

