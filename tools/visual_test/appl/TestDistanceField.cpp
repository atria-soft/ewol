/** @file
 * @author Edouard DUPIN
 * @copyright 2010, Edouard DUPIN, all right reserved
 * @license BSD v3 (see license file)
 */


#include <appl/debug.hpp>
#include <appl/TestDistanceField.hpp>

#include <ewol/widget/Button.hpp>
#include <ewol/widget/CheckBox.hpp>
#include <ewol/widget/Sizer.hpp>
#include <ewol/widget/Label.hpp>
#include <ewol/widget/Entry.hpp>
#include <ewol/widget/List.hpp>
#include <ewol/widget/ContextMenu.hpp>
#include <ewol/widget/PopUp.hpp>
#include <ewol/widget/Slider.hpp>
#include <ewol/widget/Composer.hpp>
#include <ewol/widget/Menu.hpp>
#include <ewol/widget/meta/FileChooser.hpp>
#include <ewol/widget/meta/Parameter.hpp>
#include <ewol/widget/Manager.hpp>

appl::TestDistanceField::TestDistanceField(){
	addObjectType("appl::TestDistanceField");
}

void appl::TestDistanceField::init() {
	ewol::Widget::init();
	APPL_INFO("Create appl::TestDistanceField (start)");
	propertyExpand.set(bvec2(true, true));
	propertyFill.set(bvec2(true, true));
	APPL_INFO("Create appl::TestDistanceField (end)");
}


void appl::TestDistanceField::calculateSize(const vec2& _availlable) {
	// set minimal size
	m_size = _availlable;
}


void appl::TestDistanceField::calculateMinMaxSize() {
	m_minSize = vec2(256,256);
	markToRedraw();
}


void appl::TestDistanceField::onDraw() {
	m_text2.draw();
	
	m_text1.draw();
}



void appl::TestDistanceField::onRegenerateDisplay() {
	if (false == needRedraw()) {
		return;
	}
	APPL_WARNING("Regenerate...");
	m_text1.clear();
	m_text1.setPos(vec3(m_size.x()*0.5-20,m_size.y()*0.5+10,0));
	m_text1.printDecorated("Text To compare ... Avenue AAVVAA ... Normal generic test");
	
	m_text2.clear();
	m_text2.setPos(vec3(m_size.x()*0.5-20,m_size.y()*0.5-10,0));
	m_text2.printDecorated("Text To compare ... Avenue AAVVAA ... Test en distance field Mode");
	
}

bool appl::TestDistanceField::onEventInput(const ewol::event::Input& _event) {
	if (_event.getId() == 4) {
		setZoom(getZoom() + 0.01f);
	} else if (_event.getId() == 5) {
		setZoom(getZoom() - 0.01f);
	}
	return true;
}

