/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/key.h>

static const char* statusDescriptionString[ewol::keyEvent::statusCount+1] = {
	"statusUnknow",
	"statusDown",
	"statusMove",
	"statusSingle",
	"statusDouble",
	"statusTriple",
	"statusQuad",
	"statusQuinte",
	"statusUp",
	"statusEnter",
	"statusLeave",
	"statusAbort",
	"statusTransfert",
	"statusCount"
};

etk::CCout& ewol::keyEvent::operator <<(etk::CCout& _os, const ewol::keyEvent::status_te _obj) {
	if (_obj >= 0 && _obj <ewol::keyEvent::statusCount) {
		_os << statusDescriptionString[_obj];
	} else {
		_os << "[ERROR]";
	}
	return _os;
}


static const char* keyboardDescriptionString[ewol::keyEvent::keyboardCount+1] = {
	"keyboardUnknow",
	"keyboardChar",
	"keyboardLeft",
	"keyboardRight",
	"keyboardUp",
	"keyboardDown",
	"keyboardPageUp",
	"keyboardPageDown",
	"keyboardStart",
	"keyboardEnd",
	"keyboardPrint",
	"keyboardStopDefil",
	"keyboardWait",
	"keyboardInsert",
	"keyboardF1",
	"keyboardF2",
	"keyboardF3",
	"keyboardF4",
	"keyboardF5",
	"keyboardF6",
	"keyboardF7",
	"keyboardF8",
	"keyboardF9",
	"keyboardF10",
	"keyboardF11",
	"keyboardF12",
	"keyboardCapLock",
	"keyboardShiftLeft",
	"keyboardShiftRight",
	"keyboardCtrlLeft",
	"keyboardCtrlRight",
	"keyboardMetaLeft",
	"keyboardMetaRight",
	"keyboardAlt",
	"keyboardAltGr",
	"keyboardContextMenu",
	"keyboardNumLock",
	"keyboardCount"
};

etk::CCout& ewol::keyEvent::operator <<(etk::CCout& _os, const ewol::keyEvent::keyboard_te _obj) {
	if (_obj >= 0 && _obj <ewol::keyEvent::keyboardCount) {
		_os << keyboardDescriptionString[_obj];
	} else {
		_os << "[ERROR]";
	}
	return _os;
}

static const char* typeDescriptionString[ewol::keyEvent::typeCount+1] = {
	"typeUnknow",
	"typeMouse",
	"typeFinger",
	"typeStylet",
	"typeCount"
};

etk::CCout& ewol::keyEvent::operator <<(etk::CCout& _os, const ewol::keyEvent::type_te _obj) {
	if (_obj >= 0 && _obj < ewol::keyEvent::typeCount) {
		_os << typeDescriptionString[_obj];
	} else {
		_os << "[ERROR]";
	}
	return _os;
}

ewol::SpecialKey::SpecialKey(void) :
  value(0) {
	
}

bool ewol::SpecialKey::isSetCapsLock(void) const {
	return capLock;
}

bool ewol::SpecialKey::isSetShift(void) const {
	return shift;
}

bool ewol::SpecialKey::isSetCtrl(void) const {
	return ctrl;
}

bool ewol::SpecialKey::isSetMeta(void) const {
	return meta;
}

bool ewol::SpecialKey::isSetAlt(void) const {
	return alt;
}

bool ewol::SpecialKey::isSetAltGr(void) const {
	return altGr;
}

bool ewol::SpecialKey::isSetNumLock(void) const {
	return numLock;
}
bool ewol::SpecialKey::isSetInsert(void) const {
	return insert;
}

etk::CCout& ewol::operator <<(etk::CCout& _os, const ewol::SpecialKey _obj) {
	_os << " capLock=" << _obj.capLock;
	_os << " shift=" << _obj.shift;
	_os << " ctrl=" << _obj.ctrl;
	_os << " meta=" << _obj.meta;
	_os << " alt=" << _obj.alt;
	_os << " altGr=" << _obj.altGr;
	_os << " verNum=" << _obj.numLock;
	_os << " insert=" << _obj.insert;
	return _os;
}
