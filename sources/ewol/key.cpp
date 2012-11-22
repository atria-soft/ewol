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

etk::CCout& ewol::keyEvent::operator <<(etk::CCout &os, const ewol::keyEvent::status_te obj)
{
	if (obj>=0 && obj <ewol::keyEvent::statusCount) {
		os << statusDescriptionString[obj];
	} else {
		os << "[ERROR]";
	}
	return os;
}


static const char* keyboardDescriptionString[ewol::keyEvent::keyboardCount+1] = {
	"keyboardUnknow",
	"keyboardLeft",
	"keyboardRight",
	"keyboardUp",
	"keyboardDown",
	"keyboardPageUp",
	"keyboardPageDown",
	"keyboardStart",
	"keyboardEnd",
	"keyboardCenter",
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
	"keyboardVerNum",
	"keyboardCount"
};

etk::CCout& ewol::keyEvent::operator <<(etk::CCout &os, const ewol::keyEvent::keyboard_te obj)
{
	if (obj>=0 && obj <ewol::keyEvent::keyboardCount) {
		os << keyboardDescriptionString[obj];
	} else {
		os << "[ERROR]";
	}
	return os;
}


static const char* typeDescriptionString[ewol::keyEvent::typeCount+1] = {
	"typeUnknow",
	"typeMouse",
	"typeFinger",
	"typeStylet",
	"typeCount"
};

etk::CCout& ewol::keyEvent::operator <<(etk::CCout &os, const ewol::keyEvent::type_te obj)
{
	if (obj>=0 && obj < ewol::keyEvent::typeCount) {
		os << typeDescriptionString[obj];
	} else {
		os << "[ERROR]";
	}
	return os;
}



