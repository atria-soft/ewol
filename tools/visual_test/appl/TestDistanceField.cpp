/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */


#include <appl/debug.h>
#include <appl/TestDistanceField.h>

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
#define __class__	"TestDistanceField"

appl::TestDistanceField::TestDistanceField(){
	addObjectType("appl::TestDistanceField");
}

void appl::TestDistanceField::init() {
	ewol::Widget::init();
	APPL_INFO("Create " __class__ " (start)");
	setExpand(bvec2(true, true));
	setFill(bvec2(true, true));
	APPL_INFO("Create " __class__ " (end)");
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

