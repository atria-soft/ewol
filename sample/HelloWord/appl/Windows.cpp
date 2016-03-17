/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#include <ewol/ewol.h>
#include <appl/debug.h>
#include <appl/Windows.h>
#include <ewol/widget/Label.h>

#undef __class__
#define __class__ "Windows"

appl::Windows::Windows() {
	addObjectType("appl::Windows");
	propertyTitle.setDirectCheck(std::string("sample ") + PROJECT_NAME);
}
void appl::Windows::init() {
	ewol::widget::Windows::init();
	ewol::widget::LabelShared tmpWidget = ewol::widget::Label::create();
	if (tmpWidget == nullptr) {
		APPL_ERROR("Can not allocate widget ==> display might be in error");
	} else {
		tmpWidget->propertyValue.set("Hello <font color='blue'>World</font>");
		tmpWidget->propertyExpand.set(bvec2(true,true));
		setSubWidget(tmpWidget);
	}
}


