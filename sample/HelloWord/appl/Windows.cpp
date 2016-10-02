/** @file
 * @author Edouard DUPIN
 * @copyright 2010, Edouard DUPIN, all right reserved
 * @license GPL v3 (see license file)
 */

#include <ewol/ewol.hpp>
#include <appl/debug.hpp>
#include <appl/Windows.hpp>
#include <ewol/widget/Label.hpp>

appl::Windows::Windows() {
	addObjectType("appl::Windows");
	//! [ewol_sample_HW_windows_title]
	propertyTitle.setDirectCheck(std::string("sample ") + PROJECT_NAME);
	//! [ewol_sample_HW_windows_title]
}

void appl::Windows::init() {
	//! [ewol_sample_HW_windows_init]
	ewol::widget::Windows::init();
	//! [ewol_sample_HW_windows_init]
	//! [ewol_sample_HW_windows_label]
	ewol::widget::LabelShared tmpWidget = ewol::widget::Label::create();
	if (tmpWidget == nullptr) {
		APPL_ERROR("Can not allocate widget ==> display might be in error");
	} else {
		tmpWidget->propertyValue.set("Hello <font color='blue'>World</font>");
		tmpWidget->propertyExpand.set(bvec2(true,true));
		// confidure the label as a windows sub-widget
		setSubWidget(tmpWidget);
	}
	//! [ewol_sample_HW_windows_label]
}


