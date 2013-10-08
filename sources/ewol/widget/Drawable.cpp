/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/widget/Drawable.h>


widget::Drawable::Drawable(void) {
	// nothing to do ...
}

widget::Drawable::~Drawable(void) {
	clearOObjectList();
}


void widget::Drawable::addOObject(ewol::Compositing* newObject, int32_t pos) {
	if (NULL == newObject) {
		EWOL_ERROR("Try to add an empty object in the Widget generic display system");
		return;
	}
	if (pos < 0 || pos >= m_listOObject.size() ) {
		m_listOObject.pushBack(newObject);
	} else {
		m_listOObject.insert(pos, newObject);
	}
}


void widget::Drawable::clearOObjectList(void) {
	for (int32_t iii=0; iii<m_listOObject.size(); iii++) {
		delete(m_listOObject[iii]);
		m_listOObject[iii] = NULL;
	}
	m_listOObject.clear();
}

void widget::Drawable::onDraw(void) {
	for (int32_t iii=0; iii<m_listOObject.size(); iii++) {
		if (NULL != m_listOObject[iii]) {
			m_listOObject[iii]->draw();
		}
	}
}



