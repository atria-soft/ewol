/**
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

#undef __class__
#define __class__ "Windows"


appl::Windows::Windows() {
	addObjectType("appl::Windows");
}

void appl::Windows::init() {
	ewol::widget::Windows::init();
	setTitle("example Wallpaper");
	std::shared_ptr<appl::WidgetDisplay> tmpWidget = appl::WidgetDisplay::create();
	if (tmpWidget == nullptr) {
		APPL_ERROR("Can not allocate widget ==> display might be in error");
	} else {
		setSubWidget(tmpWidget);
		tmpWidget->setExpand(bvec2(true,true));
		tmpWidget->setFill(bvec2(true,true));
	}
	setBackgroundColor(etk::Color<float>(0,0,0,0));
}

appl::Windows::~Windows() {
	
}

