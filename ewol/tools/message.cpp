/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <etk/types.hpp>
#include <ewol/ewol.hpp>
#include <ewol/tools/message.hpp>
#include <ewol/context/Context.hpp>
#include <ewol/widget/Widget.hpp>
#include <ewol/widget/Windows.hpp>
#include <ewol/widget/Manager.hpp>
#include <ewol/widget/meta/StdPopUp.hpp>

void ewol::tools::message::create(enum ewol::tools::message::type _type, const std::string& _message) {
	ewol::widget::StdPopUpShared tmpPopUp = widget::StdPopUp::create();
	if (tmpPopUp == nullptr) {
		EWOL_ERROR("Can not create a simple pop-up");
		return;
	}
	switch(_type) {
		case ewol::tools::message::type::info:
			tmpPopUp->propertyTitle.set("<bold>_T{Info}</bold>");
			break;
		case ewol::tools::message::type::warning:
			tmpPopUp->propertyTitle.set("<bold><font color='orange'>_T{Warning}</font></bold>");
			break;
		case ewol::tools::message::type::error:
			tmpPopUp->propertyTitle.set("<bold><font color='red'>_T{Error}</font></bold>");
			break;
		case ewol::tools::message::type::critical:
			tmpPopUp->propertyTitle.set("<bold><font colorBg='red'>_T{Critical}</font></bold>");
			break;
	}
	tmpPopUp->propertyComment.set(_message);
	tmpPopUp->addButton("_T{close}", true);
	tmpPopUp->propertyCloseOutEvent.set(true);
	// get windows:
	ewol::Context& context = ewol::getContext();
	ewol::widget::WindowsShared windows = context.getWindows();
	if (windows == nullptr) {
		EWOL_ERROR("can not get the current windows ... ==> can not display message : " << _message);
		return;
	}
	windows->popUpWidgetPush(tmpPopUp);
}

void ewol::tools::message::displayInfo(const std::string& _message) {
	ewol::tools::message::create(ewol::tools::message::type::info, _message);
}

void ewol::tools::message::displayWarning(const std::string& _message) {
	ewol::tools::message::create(ewol::tools::message::type::warning, _message);
}

void ewol::tools::message::displayError(const std::string& _message) {
	ewol::tools::message::create(ewol::tools::message::type::error, _message);
}

void ewol::tools::message::displayCritical(const std::string& _message) {
	ewol::tools::message::create(ewol::tools::message::type::critical, _message);
}


