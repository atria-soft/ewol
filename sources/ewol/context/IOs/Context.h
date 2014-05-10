/**
 * @author Edouard DUPIN
 *
 * @copyright 2011, Edouard DUPIN, all right reserved
 *
 * @license BSD v3 (see license file)
 */

#ifndef __MAC_OS_CONTEXT_H__
#define __MAC_OS_CONTEXT_H__

#include <ewol/key/key.h>

namespace IOs {
	// Create and relaese ewol::Context interface:
	void createInterface(void);
	void releaseInterface(void);
	// return true if a flush is needed
	bool draw(bool _displayEveryTime);
	/**
	 * @brief The OS inform that the current windows has change his size.
	 * @param[in] _size new size of the windows.
	 */
	void resize(float _x, float _y);
	void setMouseState(int32_t _id, bool _isDown, float _x, float _y);
	void setMouseMotion(int32_t _id, float _x, float _y);
	void setInputState(int32_t _id, bool _isDown, float _x, float _y);
	void setInputMotion(int32_t _id, float _x, float _y);
	void setKeyboard(ewol::key::Special _keyboardMode, int32_t _unichar, bool _isDown, bool _isAReapeateKey);
	void setKeyboardMove(ewol::key::Special& _keyboardMode, enum ewol::key::keyboard _move, bool _isDown);
	void start(void);
	void stop(void);
	void foreground(void);
	void background(void);
	void resume(void);
	void suspend(void);
};


#endif