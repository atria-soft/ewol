/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */


#include <appl/debug.h>
#include <appl/TestCheckBox.h>

#include <ewol/widget/Button.h>
#include <ewol/widget/CheckBox.h>
#include <ewol/widget/Sizer.h>
#include <ewol/widget/Label.h>
#include <ewol/widget/Entry.h>
#include <ewol/widget/List.h>
#include <ewol/widget/ContextMenu.h>
#include <ewol/widget/PopUp.h>
#include <ewol/widget/Slider.h>
#include <ewol/widget/Composer.h>
#include <ewol/widget/Menu.h>
#include <ewol/widget/meta/FileChooser.h>
#include <ewol/widget/meta/Parameter.h>
#include <ewol/widget/Manager.h>

#undef __class__
#define __class__	"TestCheckBox"

appl::TestCheckBox::TestCheckBox() {
	addObjectType("appl::TestCheckBox");
}

void appl::TestCheckBox::init() {
	ewol::widget::Composer::init();
	
	APPL_INFO("Create " __class__ " (start)");
	
	std::string myDescription = std::string("")
	      + "<sizer mode='vert' fill='true' expand='true'>\n"
	      + "	<sizer mode='hori'>\n"
	      + "		<button name='[TEST]Button:Expand-x' toggle='true'>\n"
	      + "			<label>Expend X <br/> (false)</label>\n"
	      + "			<label>Expend X <br/> <b>(true)</b></label>\n"
	      + "		</button>\n"
	      + "		<button name='[TEST]Button:Expand-y' toggle='true'>\n"
	      + "			<label>Expend Y <br/> (false)</label>\n"
	      + "			<label>Expend Y <br/> <b>(true)</b></label>\n"
	      + "		</button>\n"
	      + "		<button name='[TEST]Button:TextToggle' toggle='true'>\n"
	      + "			<label>Text On toggle state <br/> (false)</label>\n"
	      + "			<label>Text On toggle state <br/> <b>(true)</b></label>\n"
	      + "		</button>\n"
	      + "	</sizer>\n"
	      + "	<sizer mode='hori'>\n"
	      + "		<button name='[TEST]Button:Fill-x' toggle='true'>\n"
	      + "			<label>Fill X <br/> (false)</label>\n"
	      + "			<label>Fill X <br/> <b>(true)</b></label>\n"
	      + "		</button>\n"
	      + "		<button name='[TEST]Button:Fill-y' toggle='true'>\n"
	      + "			<label>Fill Y <br/> (false)</label>\n"
	      + "			<label>Fill Y <br/> <b>(true)</b></label>\n"
	      + "		</button>\n"
	      + "		<button name='[TEST]Button:ChangeText'>\n"
	      + "			<label>Change Text<br/> and other ...</label>\n"
	      + "		</button>\n"
	      + "	</sizer>\n"
	      + "	<sizer mode='hori'>\n"
	      + "		<spacer expand='false,false' fill='true,false' min-size='10,10px' color='#F008'/>\n"
	      + "		<sizer mode='vert'>\n"
	      + "			<spacer expand='false,false' fill='false,true' min-size='10,10px' color='#FF08'/>\n"
	      + "			<CheckBox name='[TEST]TO-TEST' expand='false,false' fill='false,false' >\n"
	      + "				<label>My <font color='#FF0000'>CheckBox</font> <br/> And Some under line<br/> plop <br/> and an other super long line ...</label>\n"
	      + "			</CheckBox>\n"
	      + "			<spacer expand='false,false' fill='false,true' min-size='10,10px' color='#0FF8'/>\n"
	      + "		</sizer>\n"
	      + "		<spacer expand='false,false' fill='true,false' min-size='10,10px' color='#0F08'/>\n"
	      + "	</sizer>\n"
	      + "</sizer>";
	loadFromString(myDescription);
	setExpand(bvec2(true,true));
	setFill(bvec2(true,true));
	subBind(ewol::widget::Button, "[TEST]Button:Expand-x", signalValue, shared_from_this(), &appl::TestCheckBox::onCallbackExpendX);
	subBind(ewol::widget::Button, "[TEST]Button:Expand-y", signalValue, shared_from_this(), &appl::TestCheckBox::onCallbackExpendY);
	subBind(ewol::widget::Button, "[TEST]Button:Fill-x", signalValue, shared_from_this(), &appl::TestCheckBox::onCallbackFillX);
	subBind(ewol::widget::Button, "[TEST]Button:Fill-y", signalValue, shared_from_this(), &appl::TestCheckBox::onCallbackFillY);
	subBind(ewol::widget::Button, "[TEST]Button:TextToggle", signalValue, shared_from_this(), &appl::TestCheckBox::onCallbackTextToggle);
	subBind(ewol::widget::Button, "[TEST]Button:ChangeText", signalPressed, shared_from_this(), &appl::TestCheckBox::onCallbackChangeText);
	m_testWidget = std::dynamic_pointer_cast<ewol::widget::CheckBox>(getSubObjectNamed("[TEST]TO-TEST"));
	if (m_testWidget == nullptr) {
		APPL_CRITICAL("Can not get the pointer of the widget button pointer");
	}
	APPL_INFO("Create " __class__ " (end)");
}

void appl::TestCheckBox::onCallbackExpendX(const bool& _value) {
	if (m_testWidget == nullptr) {
		return;
	}
	m_testWidget->setExpand(bvec2(_value, m_testWidget->getExpand().y()));
}

void appl::TestCheckBox::onCallbackExpendY(const bool& _value) {
	if (m_testWidget == nullptr) {
		return;
	}
	m_testWidget->setExpand(bvec2(m_testWidget->getExpand().x(), _value));
}

void appl::TestCheckBox::onCallbackFillX(const bool& _value) {
	if (m_testWidget == nullptr) {
		return;
	}
	m_testWidget->setFill(bvec2(_value, m_testWidget->getFill().y()));
}

void appl::TestCheckBox::onCallbackFillY(const bool& _value) {
	if (m_testWidget == nullptr) {
		return;
	}
	m_testWidget->setFill(bvec2(m_testWidget->getFill().x(), _value));
}

void appl::TestCheckBox::onCallbackTextToggle(const bool& _value) {
	if (m_testWidget == nullptr) {
		return;
	}
	if (_value == true) {
		m_testWidget->setSubWidgetToggle(ewol::widget::Label::create("A stupid very long text on toggle <br/><br/> and on multiple lines"));
	} else {
		m_testWidget->setSubWidgetToggle(nullptr);
	}
}

void appl::TestCheckBox::onCallbackChangeText() {
	if (m_testWidget == nullptr) {
		return;
	}
	static int32_t countTextID = 1;
	switch (countTextID) {
		case 0:
			m_testWidget->setSubWidget(ewol::widget::Label::create("simple Text"));
			break;
		case 1:
			m_testWidget->setSubWidget(ewol::widget::Label::create("<left>Align Left</left>"));
			break;
		case 2:
			m_testWidget->setSubWidget(ewol::widget::Label::create("<right>Align right</right>"));
			break;
		case 3:
			m_testWidget->setSubWidget(ewol::widget::Label::create("<center>Align center</center>"));
			break;
		case 4:
			m_testWidget->setSubWidget(ewol::widget::Label::create("simple Text<br/> With Some Other Lines<br/> and more if you want ...<br/> plop"));
			break;
		case 5:
			m_testWidget->setSubWidget(ewol::widget::Label::create("simple <bold>Text</bold> with bold"));
			break;
		case 6:
			m_testWidget->setSubWidget(ewol::widget::Label::create("simple <italic>Text</italic> with italic"));
			break;
		case 7:
			m_testWidget->setSubWidget(ewol::widget::Label::create("simple <italic><bold>Text</bold></italic> with italic bold"));
			break;
		case 8:
			m_testWidget->setSubWidget(nullptr);
			countTextID=-1;
			break;
	}
	// set at the subWidget all the space they can ...
	if (m_testWidget->getSubWidget() != nullptr){
		m_testWidget->getSubWidget()->setFill(bvec2(true,true));
		m_testWidget->getSubWidget()->setExpand(bvec2(true,true));
	}
	countTextID++;
}



