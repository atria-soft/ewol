/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/cursor.h>

static const char* cursorDescriptionString[ewol::cursorCount+1] = {
	"cursorArrow",
	"cursorLeftArrow",
	"cursorInfo",
	"cursorDestroy",
	"cursorHelp",
	"cursorCycle",
	"cursorSpray",
	"cursorWait",
	"cursorText",
	"cursorCrossHair",
	"cursorSlideUpDown",
	"cursorSlideLeftRight",
	"cursorResizeUp",
	"cursorResizeDown",
	"cursorResizeLeft",
	"cursorResizeRight",
	"cursorCornerTopLeft",
	"cursorCornerTopRight",
	"cursorCornerButtomLeft",
	"cursorCornerButtomRight",
	"cursorNone",
	"cursorCount"
};

etk::CCout& ewol::operator <<(etk::CCout& _os, const ewol::cursorDisplay_te _obj)
{
	if (_obj>=0 && _obj <ewol::cursorCount) {
		_os << cursorDescriptionString[_obj];
	} else {
		_os << "[ERROR]";
	}
	return _os;
}


