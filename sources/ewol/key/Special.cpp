/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/key/Special.h>

#define EWOL_FLAG_KEY_CAPS_LOCK 0x00000001
#define EWOL_FLAG_KEY_SHIFT     0x00000002
#define EWOL_FLAG_KEY_CTRL      0x00000004
#define EWOL_FLAG_KEY_META      0x00000008
#define EWOL_FLAG_KEY_ALT       0x00000010
#define EWOL_FLAG_KEY_ALTGR     0x00000020
#define EWOL_FLAG_KEY_NUM_LOCK  0x00000040
#define EWOL_FLAG_KEY_INSERT    0x00000080


ewol::key::Special::Special(void) :
  m_value(0) {
	
}

bool ewol::key::Special::getCapsLock(void) const {
	if ((m_value & EWOL_FLAG_KEY_CAPS_LOCK) != 0) {
		return true;
	}
	return false;
}
void ewol::key::Special::setCapsLock(bool _value) {
	if ((m_value & EWOL_FLAG_KEY_CAPS_LOCK) != 0) {
		if (_value == false) {
			m_value -= EWOL_FLAG_KEY_CAPS_LOCK;
		}
	} else {
		if (_value == true) {
			m_value += EWOL_FLAG_KEY_CAPS_LOCK;
		}
	}
}

bool ewol::key::Special::getShift(void) const {
	if ((m_value & EWOL_FLAG_KEY_SHIFT) != 0) {
		return true;
	}
	return false;
}
void ewol::key::Special::setShift(bool _value) {
	if ((m_value & EWOL_FLAG_KEY_SHIFT) != 0) {
		if (_value == false) {
			m_value -= EWOL_FLAG_KEY_SHIFT;
		}
	} else {
		if (_value == true) {
			m_value += EWOL_FLAG_KEY_SHIFT;
		}
	}
}

bool ewol::key::Special::getCtrl(void) const {
	if ((m_value & EWOL_FLAG_KEY_CTRL) != 0) {
		return true;
	}
	return false;
}
void ewol::key::Special::setCtrl(bool _value) {
	if ((m_value & EWOL_FLAG_KEY_CTRL) != 0) {
		if (_value == false) {
			m_value -= EWOL_FLAG_KEY_CTRL;
		}
	} else {
		if (_value == true) {
			m_value += EWOL_FLAG_KEY_CTRL;
		}
	}
}

bool ewol::key::Special::getMeta(void) const {
	if ((m_value & EWOL_FLAG_KEY_META) != 0) {
		return true;
	}
	return false;
}
void ewol::key::Special::setMeta(bool _value) {
	if ((m_value & EWOL_FLAG_KEY_META) != 0) {
		if (_value == false) {
			m_value -= EWOL_FLAG_KEY_META;
		}
	} else {
		if (_value == true) {
			m_value += EWOL_FLAG_KEY_META;
		}
	}
}

bool ewol::key::Special::getAlt(void) const {
	if ((m_value & EWOL_FLAG_KEY_ALT) != 0) {
		return true;
	}
	return false;
}
void ewol::key::Special::setAlt(bool _value) {
	if ((m_value & EWOL_FLAG_KEY_ALT) != 0) {
		if (_value == false) {
			m_value -= EWOL_FLAG_KEY_ALT;
		}
	} else {
		if (_value == true) {
			m_value += EWOL_FLAG_KEY_ALT;
		}
	}
}

bool ewol::key::Special::getAltGr(void) const {
	if ((m_value & EWOL_FLAG_KEY_ALTGR) != 0) {
		return true;
	}
	return false;
}
void ewol::key::Special::setAltGr(bool _value) {
	if ((m_value & EWOL_FLAG_KEY_ALTGR) != 0) {
		if (_value == false) {
			m_value -= EWOL_FLAG_KEY_ALTGR;
		}
	} else {
		if (_value == true) {
			m_value += EWOL_FLAG_KEY_ALTGR;
		}
	}
}

bool ewol::key::Special::getNumLock(void) const {
	if ((m_value & EWOL_FLAG_KEY_NUM_LOCK) != 0) {
		return true;
	}
	return false;
}
void ewol::key::Special::setNumLock(bool _value) {
	if ((m_value & EWOL_FLAG_KEY_NUM_LOCK) != 0) {
		if (_value == false) {
			m_value -= EWOL_FLAG_KEY_NUM_LOCK;
		}
	} else {
		if (_value == true) {
			m_value += EWOL_FLAG_KEY_NUM_LOCK;
		}
	}
}

bool ewol::key::Special::getInsert(void) const {
	if ((m_value & EWOL_FLAG_KEY_INSERT) != 0) {
		return true;
	}
	return false;
}
void ewol::key::Special::setInsert(bool _value) {
	if ((m_value & EWOL_FLAG_KEY_INSERT) != 0) {
		if (_value == false) {
			m_value -= EWOL_FLAG_KEY_INSERT;
		}
	} else {
		if (_value == true) {
			m_value += EWOL_FLAG_KEY_INSERT;
		}
	}
}

etk::CCout& ewol::key::operator <<(etk::CCout& _os, const ewol::key::Special _obj) {
	_os << " capLock=" << _obj.getCapsLock();
	_os << " shift=" << _obj.getShift();
	_os << " ctrl=" << _obj.getCtrl();
	_os << " meta=" << _obj.getMeta();
	_os << " alt=" << _obj.getAlt();
	_os << " altGr=" << _obj.getAltGr();
	_os << " verNum=" << _obj.getNumLock();
	_os << " insert=" << _obj.getInsert();
	return _os;
}
