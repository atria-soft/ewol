/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */


#include <appl/debug.h>
#include <appl/TestImage.h>

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
#define __class__ "TestImage"

appl::TestImage::TestImage() {
	addObjectType("appl::TestImage");
}
void appl::TestImage::init() {
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
	      + "		<button name='[TEST]Button:Toggle' toggle='true'>\n"
	      + "			<label>Toggle <br/> (false)</label>\n"
	      + "			<label>Toggle <br/> <b>(true)</b></label>\n"
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
	      + "		<button name='[TEST]Button:ChangeImage'>\n"
	      + "			<label>Change Image<br/> and other ...</label>\n"
	      + "		</button>\n"
	      + "	</sizer>\n"
	      + "	<sizer mode='hori'>\n"
	      + "		<spacer expand='false,false' fill='true,false' min-size='10,10px' color='#F008'/>\n"
	      + "		<sizer mode='vert'>\n"
	      + "			<spacer expand='false,false' fill='false,true' min-size='10,10px' color='#FF08'/>\n"
	      + "			<image name='[TEST]TO-TEST' expand='false,false' fill='false,false' src='DATA:sphere.png'/>\n"
	      + "			<spacer expand='false,false' fill='false,true' min-size='10,10px' color='#0FF8'/>\n"
	      + "		</sizer>\n"
	      + "		<spacer expand='false,false' fill='true,false' min-size='10,10px' color='#0F08'/>\n"
	      + "	</sizer>\n"
	      + "</sizer>";
	loadFromString(myDescription);
	setExpand(bvec2(true,true));
	setFill(bvec2(true,true));
	subBind(ewol::widget::Button, "[TEST]Button:Expand-x", signalValue, shared_from_this(), &appl::TestImage::onCallbackExpendX);
	subBind(ewol::widget::Button, "[TEST]Button:Expand-y", signalValue, shared_from_this(), &appl::TestImage::onCallbackExpendY);
	subBind(ewol::widget::Button, "[TEST]Button:Fill-x", signalValue, shared_from_this(), &appl::TestImage::onCallbackFillX);
	subBind(ewol::widget::Button, "[TEST]Button:Fill-y", signalValue, shared_from_this(), &appl::TestImage::onCallbackFillY);
	subBind(ewol::widget::Button, "[TEST]Button:ChangeImage", signalPressed, shared_from_this(), &appl::TestImage::onCallbackChangeImage);
	m_testWidget = std::dynamic_pointer_cast<ewol::widget::Image>(getSubObjectNamed("[TEST]TO-TEST"));
	if (m_testWidget == nullptr) {
		APPL_CRITICAL("Can not get the pointer of the widget button pointer");
	}
	APPL_INFO("Create " __class__ " (end)");
}

void appl::TestImage::onCallbackExpendX(const bool& _value) {
	if (m_testWidget == nullptr) {
		return;
	}
	m_testWidget->setExpand(bvec2(_value, m_testWidget->getExpand().y()));
}

void appl::TestImage::onCallbackExpendY(const bool& _value) {
	if (m_testWidget == nullptr) {
		return;
	}
	m_testWidget->setExpand(bvec2(m_testWidget->getExpand().x(), _value));
}

void appl::TestImage::onCallbackFillX(const bool& _value) {
	if (m_testWidget == nullptr) {
		return;
	}
	m_testWidget->setFill(bvec2(_value, m_testWidget->getFill().y()));
}

void appl::TestImage::onCallbackFillY(const bool& _value) {
	if (m_testWidget == nullptr) {
		return;
	}
	m_testWidget->setFill(bvec2(m_testWidget->getFill().x(), _value));
}


void appl::TestImage::onCallbackChangeImage() {
	if (m_testWidget == nullptr) {
		return;
	}
	static int32_t countID = 1;
	switch (countID) {
		case 0:
			m_testWidget->setFile("DATA:stone.png");
			break;
		case 1:
			m_testWidget->setFile("DATA:grass.png");
			break;
		default:
			m_testWidget->setFile("DATA:sphere.png");
			countID=-1;
			break;
	}
	countID++;
}

