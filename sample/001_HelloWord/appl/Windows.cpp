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
}
void appl::Windows::init() {
	setTitle("example 001_HelloWord");
	std::shared_ptr<ewol::widget::Label> tmpWidget = ewol::widget::Label::create();
	if (tmpWidget == nullptr) {
		APPL_ERROR("Can not allocate widget ==> display might be in error");
	} else {
		tmpWidget->setLabel("Hello <font color=\"blue\">Word</font>");
		tmpWidget->setExpand(bvec2(true,true));
		setSubWidget(tmpWidget);
	}
}


