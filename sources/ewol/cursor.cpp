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

etk::CCout& ewol::operator <<(etk::CCout &os, const ewol::cursorDisplay_te obj)
{
	if (obj>=0 && obj <ewol::cursorCount) {
		os << cursorDescriptionString[obj];
	} else {
		os << "[ERROR]";
	}
	return os;
}


