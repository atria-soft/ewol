/** @file
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#include <ewol/ewol.h>
#include <appl/debug.h>
#include <appl/WidgetDisplay.h>
#include <appl/Windows.h>

appl::Windows::Windows() {
	addObjectType("appl::Windows");
	propertyTitle.setDirectCheck(std::string("sample ") + PROJECT_NAME);
}

void appl::Windows::init() {
	ewol::widget::Windows::init();
	appl::WidgetDisplayShared tmpWidget = appl::WidgetDisplay::create();
	if (tmpWidget == nullptr) {
		APPL_ERROR("Can not allocate widget ==> display might be in error");
	} else {
		setSubWidget(tmpWidget);
		tmpWidget->propertyExpand.set(bvec2(true,true));
		tmpWidget->propertyFill.set(bvec2(true,true));
	}
}

appl::Windows::~Windows() {
	
}

