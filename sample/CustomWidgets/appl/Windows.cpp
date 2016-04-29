/** @file
 * @author Edouard DUPIN
 * @copyright 2010, Edouard DUPIN, all right reserved
 * @license BSD 3 clauses (see license file)
 */

#include <ewol/ewol.h>
#include <appl/debug.h>
#include <appl/Windows.h>
#include <ewol/widget/Label.h>
#include <ewol/widget/Button.h>
#include <appl/widget/VectorDisplay.h>
#include <etk/tool.h>

appl::Windows::Windows() :
  m_composer(nullptr) {
	addObjectType("appl::Windows");
	propertyTitle.setDirectCheck(std::string("sample ") + PROJECT_NAME);
}

void appl::Windows::init() {
	ewol::widget::Windows::init();
	std::string composition = std::string("");
	composition += "<sizer mode='vert'>\n";
	composition += "	<sizer mode='hori'>\n";
	composition += "		<button name='bt-change'>\n";
	composition += "			<label>\n";
	composition += "				Change Data ...\n";
	composition += "			</label>\n";
	composition += "		</button>\n";
	composition += "		<button name='bt-auto'>\n";
	composition += "			<label>\n";
	composition += "				Auto generate\n";
	composition += "			</label>\n";
	composition += "		</button>\n";
	composition += "	</sizer>\n";
	composition += "	<VectorDisplay name='displayer' expand='true' fill='true'/>\n";
	composition += "</sizer>\n";
	
	m_composer = ewol::widget::Composer::create();
	if (m_composer == nullptr) {
		APPL_CRITICAL(" An error occured ... in the windows creatrion ...");
		return;
	}
	m_composer->loadFromString(composition);
	setSubWidget(m_composer);
	subBind(ewol::widget::Button, "bt-change", signalPressed, shared_from_this(), &appl::Windows::onCallbackChangeValues);
	subBind(ewol::widget::Button, "bt-auto", signalPressed, shared_from_this(), &appl::Windows::onCallbackAutoMode);
}

void appl::Windows::onCallbackChangeValues() {
	std::vector<float> tmp;
	for (int32_t iii=0; iii<2048; ++iii) {
		tmp.push_back(etk::tool::frand(-1.0, 1.0));
	}
	std::shared_ptr<appl::widget::VectorDisplay> tmpDisp = std::dynamic_pointer_cast<appl::widget::VectorDisplay>(getSubObjectNamed("displayer"));
	if (tmpDisp != nullptr) {
		tmpDisp->setValue(tmp);
	}
}

void appl::Windows::onCallbackAutoMode() {
	std::shared_ptr<appl::widget::VectorDisplay> tmpDisp = std::dynamic_pointer_cast<appl::widget::VectorDisplay>(getSubObjectNamed("displayer"));
	if (tmpDisp != nullptr) {
		tmpDisp->ToggleAuto();
	}
}

