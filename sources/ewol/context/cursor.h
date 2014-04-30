/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_CURSOR_H__
#define __EWOL_CURSOR_H__

#include <ewol/debug.h>

namespace ewol {
	namespace context {
		enum cursorDisplay {
			cursorArrow, //  this is the normal arrow ...
			cursorLeftArrow,
			cursorInfo,
			cursorDestroy,
			cursorHelp,
			cursorCycle,
			cursorSpray,
			cursorWait,
			cursorText,
			cursorCrossHair,
			cursorSlideUpDown,      //!< change the position (slide) vertical
			cursorSlideLeftRight,   //!< change the position (slide) horizontal
			cursorResizeUp,
			cursorResizeDown,
			cursorResizeLeft,
			cursorResizeRight,
			cursorCornerTopLeft,
			cursorCornerTopRight,
			cursorCornerButtomLeft,
			cursorCornerButtomRight,
			cursorNone,
			// just for the count:
			cursorCount
		};
	};
	/**
	 * @brief Debug operator To display the curent element in a Human readable information
	 */
	std::ostream& operator <<(std::ostream& _os, const enum ewol::context::cursorDisplay _obj);
};

#endif


